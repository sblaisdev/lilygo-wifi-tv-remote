#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"
#include <TFT_eSPI.h>

// ESP32-S3 Hardware Security & Crypto
#include "esp_efuse.h"
#include "esp_efuse_table.h"
#include "soc/esp32s3/esp_hmac.h"
#include "esp_random.h"
#include "mbedtls/aes.h"

#include "../include/config.h"
#include "webpage.h"

// USB HID objects
USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;

// Display object (ST7735 80x160)
TFT_eSPI tft = TFT_eSPI();

// HTTP Web Server on port 80 & WebSocket Server on port 81 (sub-millisecond keystroke stream)
WebServer server(80);
WebSocketsServer webSocket(81);

// Persistent Storage
Preferences prefs;

// Device runtime identity
String roomName = DEFAULT_ROOM_NAME;
String mdnsHostname = DEFAULT_MDNS_HOSTNAME;
String apPassword = "";
bool isApMode = false;

// Hardware HMAC & Encryption Key
hmac_key_id_t hmacKeySlot = HMAC_KEY_MAX;
bool hmacAvailable = false;
uint8_t derivedKey[32]; // AES-256 derived key

// Screen power management state
bool screenOn = false;
unsigned long screenTimer = 0;

// Button state machine for Reset & Sleep
int lastButtonReading = HIGH;
unsigned long buttonPressStartTime = 0;
bool buttonHeldPast10s = false;
enum SystemState {
  STATE_NORMAL,
  STATE_CONFIRM_RESET
};
SystemState currentState = STATE_NORMAL;
unsigned long resetPromptStartTime = 0;

// Forward declarations
void initCrypto();
bool getOrProvisionHmacKey(hmac_key_id_t &slot);
bool deriveKey(const char* context, uint8_t* outKey32);
String encryptCredential(const String& plain);
String decryptCredential(const String& cipherHex);
String generateRandomPassword(size_t length = 8);
void startAccessPoint();
void connectToSavedWifi();
void wakeScreen();
void sleepScreen();
void updateScreenContent();
void showResetPromptScreen();
void setupRoutes();
void handleKeyCommand(const String& key);
void handleTextCommand(const String& text);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

// =========================================================================
// Crypto & eFuse Key Management
// =========================================================================
void initCrypto() {
  if (getOrProvisionHmacKey(hmacKeySlot)) {
    hmacAvailable = true;
    Serial.printf("[SECURITY] HMAC Key active in slot KEY%d\n", (int)hmacKeySlot);
    // Derive AES-256 key with domain separation string
    if (deriveKey("project-wifi-v1", derivedKey)) {
      Serial.println("[SECURITY] AES-256 encryption key derived successfully from Hardware HMAC.");
    } else {
      Serial.println("[SECURITY] ERROR: Failed to derive key from Hardware HMAC peripheral!");
      hmacAvailable = false;
    }
  } else {
    Serial.println("[SECURITY] WARNING: No HMAC key available and could not allocate eFuse slot.");
  }
}

// Auto-Detect & Reuse eFuse HMAC key slot
bool getOrProvisionHmacKey(hmac_key_id_t &slot) {
  // Step 1: Scan KEY0 through KEY5 for an existing ESP_EFUSE_KEY_PURPOSE_HMAC_UP
  for (int i = 0; i < 6; i++) {
    esp_efuse_block_t blk = (esp_efuse_block_t)(EFUSE_BLK_KEY0 + i);
    esp_efuse_purpose_t purpose = esp_efuse_get_key_purpose(blk);
    if (purpose == ESP_EFUSE_KEY_PURPOSE_HMAC_UP) {
      slot = (hmac_key_id_t)i;
      Serial.printf("[SECURITY] Found existing Hardware HMAC key in eFuse block KEY%d. Reusing it.\n", i);
      return true;
    }
  }

  // Step 2: No existing HMAC slot found. Allocate first empty slot scanning KEY5 downwards
  for (int i = 5; i >= 0; i--) {
    esp_efuse_block_t blk = (esp_efuse_block_t)(EFUSE_BLK_KEY0 + i);
    if (esp_efuse_key_block_unused(blk)) {
      Serial.printf("[SECURITY] Provisioning new HMAC key into unused eFuse block KEY%d...\n", i);
      uint8_t randomKey[32];
      esp_fill_random(randomKey, sizeof(randomKey));
      esp_err_t err = esp_efuse_write_key(blk, ESP_EFUSE_KEY_PURPOSE_HMAC_UP, randomKey, sizeof(randomKey));
      if (err == ESP_OK) {
        slot = (hmac_key_id_t)i;
        Serial.printf("[SECURITY] Successfully burned Hardware HMAC key into KEY%d.\n", i);
        return true;
      } else {
        Serial.printf("[SECURITY] Error burning eFuse key to KEY%d: 0x%x\n", i, err);
      }
    }
  }

  return false;
}

// Derive 256-bit key using Hardware HMAC peripheral
bool deriveKey(const char* context, uint8_t* outKey32) {
  if (!hmacAvailable && hmacKeySlot == HMAC_KEY_MAX) return false;
  esp_err_t err = esp_hmac_calculate(hmacKeySlot, context, strlen(context), outKey32);
  return (err == ESP_OK);
}

// Encrypt string using AES-256 CTR mode. Output format: HEX(16-byte Nonce + Ciphertext)
String encryptCredential(const String& plain) {
  if (!hmacAvailable || plain.length() == 0) return plain;

  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, derivedKey, 256);

  uint8_t nonce[16];
  esp_fill_random(nonce, sizeof(nonce));

  uint8_t nonce_counter[16];
  memcpy(nonce_counter, nonce, 16);

  size_t nc_off = 0;
  uint8_t stream_block[16];
  memset(stream_block, 0, sizeof(stream_block));

  size_t len = plain.length();
  uint8_t* output = (uint8_t*)malloc(len);
  if (!output) {
    mbedtls_aes_free(&aes);
    return "";
  }

  mbedtls_aes_crypt_ctr(&aes, len, &nc_off, nonce_counter, stream_block, (const unsigned char*)plain.c_str(), output);
  mbedtls_aes_free(&aes);

  // Encode Nonce (16 bytes) + Output (len bytes) as Hex
  String hexResult = "";
  hexResult.reserve((16 + len) * 2);
  for (int i = 0; i < 16; i++) {
    char buf[3];
    sprintf(buf, "%02x", nonce[i]);
    hexResult += buf;
  }
  for (size_t i = 0; i < len; i++) {
    char buf[3];
    sprintf(buf, "%02x", output[i]);
    hexResult += buf;
  }

  free(output);
  return hexResult;
}

// Decrypt Hex(Nonce + Ciphertext) using AES-256 CTR
String decryptCredential(const String& cipherHex) {
  if (!hmacAvailable || cipherHex.length() < 34) {
    return cipherHex;
  }

  size_t totalBytes = cipherHex.length() / 2;
  if (totalBytes <= 16) return "";

  uint8_t* raw = (uint8_t*)malloc(totalBytes);
  if (!raw) return "";

  for (size_t i = 0; i < totalBytes; i++) {
    char byteStr[3] = { cipherHex[i * 2], cipherHex[i * 2 + 1], '\0' };
    raw[i] = (uint8_t)strtoul(byteStr, NULL, 16);
  }

  uint8_t nonce_counter[16];
  memcpy(nonce_counter, raw, 16);

  size_t cipherLen = totalBytes - 16;
  uint8_t* plaintext = (uint8_t*)malloc(cipherLen + 1);
  if (!plaintext) {
    free(raw);
    return "";
  }

  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, derivedKey, 256);

  size_t nc_off = 0;
  uint8_t stream_block[16];
  memset(stream_block, 0, sizeof(stream_block));

  mbedtls_aes_crypt_ctr(&aes, cipherLen, &nc_off, nonce_counter, stream_block, raw + 16, plaintext);
  mbedtls_aes_free(&aes);
  free(raw);

  plaintext[cipherLen] = '\0';
  String res = String((char*)plaintext);
  free(plaintext);
  return res;
}

// Cryptographically random 8-character password from Hardware TRNG
String generateRandomPassword(size_t length) {
  const char charset[] = "23456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnpqrstuvwxyz";
  const size_t charsetSize = sizeof(charset) - 1;
  String pwd = "";
  pwd.reserve(length);
  for (size_t i = 0; i < length; i++) {
    uint32_t r = esp_random() % charsetSize;
    pwd += charset[r];
  }
  return pwd;
}

// =========================================================================
// Network & Setup
// =========================================================================
void startAccessPoint() {
  isApMode = true;
  WiFi.mode(WIFI_AP);
  if (apPassword.length() == 0) {
    apPassword = generateRandomPassword(8);
  }
  WiFi.softAP(DEFAULT_AP_SSID, apPassword.c_str());
  Serial.printf("[AP] Access Point started!\nSSID: %s\nPassword: %s\nIP: %s\n",
                DEFAULT_AP_SSID, apPassword.c_str(), WiFi.softAPIP().toString().c_str());

  // Turn on screen immediately so user can see credentials
  wakeScreen();
}

void connectToSavedWifi() {
  prefs.begin("tvremote", true); // read-only
  String ssid = prefs.getString("ssid", "");
  String encPass = prefs.getString("enc_pass", "");
  roomName = prefs.getString("room", DEFAULT_ROOM_NAME);
  mdnsHostname = prefs.getString("mdns", DEFAULT_MDNS_HOSTNAME);
  prefs.end();

  if (ssid.length() == 0) {
    Serial.println("[WIFI] No stored Wi-Fi credentials found in NVS flash.");
    startAccessPoint();
    return;
  }

  String password = decryptCredential(encPass);
  Serial.printf("[WIFI] Connecting to saved network: %s\n", ssid.c_str());

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 25) {
    delay(400);
    Serial.print(".");
    attempts++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    isApMode = false;
    Serial.printf("[WIFI] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("[WIFI] Failed to connect to saved network. Launching fallback AP...");
    startAccessPoint();
  }
}

// =========================================================================
// Arduino Setup & Main Loop
// =========================================================================
void setup() {
  Serial.begin(115200);

  // Initialize button (active LOW with internal pull-up)
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // Initialize screen backlight pin and turn backlight ON immediately (Active LOW)
  pinMode(PIN_LCD_BL, OUTPUT);
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_ON);

  // Initialize TFT display
  tft.init();
  tft.setRotation(1); // Landscape orientation (160x80)
  tft.fillScreen(TFT_BLACK);
  screenOn = true;

  tft.fillRect(0, 0, 160, 16, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawString(roomName + " TV", 8, 2, 2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Starting...", 8, 30, 2);

  // Initialize USB HID
  Keyboard.begin();
  ConsumerControl.begin();
  USB.begin();

  // Initialize Hardware Security & Key Derivation
  initCrypto();

  // Connect to Wi-Fi or launch Fallback AP
  connectToSavedWifi();

  // Setup mDNS responder (http://<hostname>.local)
  if (MDNS.begin(mdnsHostname.c_str())) {
    Serial.printf("[mDNS] Responder started: http://%s.local\n", mdnsHostname.c_str());
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);
  }

  // Setup HTTP routes
  setupRoutes();
  server.begin();

  // Setup WebSocket Server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("[SYSTEM] Ready! Web and WebSocket remote control active.");
}

void loop() {
  server.handleClient();
  webSocket.loop();

  // ==========================================
  // Physical Button & Factory Reset State Machine
  // ==========================================
  int reading = digitalRead(PIN_BUTTON);

  if (currentState == STATE_NORMAL) {
    if (reading == LOW) {
      if (lastButtonReading == HIGH) {
        // Button just pressed down
        buttonPressStartTime = millis();
        buttonHeldPast10s = false;
      } else {
        // Button is being held down
        if (!buttonHeldPast10s && (millis() - buttonPressStartTime >= FACTORY_RESET_HOLD_MS)) {
          buttonHeldPast10s = true;
          currentState = STATE_CONFIRM_RESET;
          resetPromptStartTime = millis();
          showResetPromptScreen();
          Serial.println("[RESET] 10s hold reached! Confirmation prompt displayed.");
        }
      }
    } else {
      // Button is released
      if (lastButtonReading == LOW) {
        if (!buttonHeldPast10s) {
          // Short press: wake or sleep screen (keep screen on in AP mode)
          if (!screenOn) {
            wakeScreen();
          } else if (!isApMode) {
            sleepScreen();
          }
        }
      }
    }
  } else if (currentState == STATE_CONFIRM_RESET) {
    // Wait for user to release button from the 10s hold before accepting a confirmation click
    if (lastButtonReading == LOW && reading == HIGH) {
      // User released the 10s hold
    } else if (lastButtonReading == HIGH && reading == LOW) {
      // User clicked the button a second time to confirm reset!
      Serial.println("[RESET] Factory reset confirmed by second button click! Clearing NVS...");
      prefs.begin("tvremote", false);
      prefs.clear();
      prefs.end();

      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.drawString("Reset Complete!", 16, 26, 2);
      tft.drawString("Rebooting...", 30, 46, 2);
      delay(1500);
      ESP.restart();
    }

    // Timeout confirmation mode after RESET_CONFIRM_TIMEOUT (20s)
    if (millis() - resetPromptStartTime > RESET_CONFIRM_TIMEOUT) {
      Serial.println("[RESET] Confirmation timed out. Reset canceled.");
      currentState = STATE_NORMAL;
      sleepScreen();
    }
  }

  lastButtonReading = reading;

  // Auto screen timeout in normal station mode (keep screen ON continuously in AP mode for easy provisioning)
  if (screenOn && !isApMode && currentState == STATE_NORMAL && (millis() - screenTimer > SCREEN_TIMEOUT_MS)) {
    sleepScreen();
  }

  delay(2);
}

// =========================================================================
// Display Views
// =========================================================================
void wakeScreen() {
  screenOn = true;
  screenTimer = millis();
  updateScreenContent();
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_ON);
  Serial.println("[LCD] Screen turned ON");
}

void sleepScreen() {
  screenOn = false;
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_OFF);
  tft.fillScreen(TFT_BLACK);
  Serial.println("[LCD] Screen turned OFF");
}

void updateScreenContent() {
  tft.fillScreen(TFT_BLACK);

  // Title bar
  tft.fillRect(0, 0, 160, 16, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawString(roomName + " TV", 8, 2, 2);

  if (isApMode) {
    // Fallback AP Mode View (Physical Proximity Barrier)
    // WiFi SSID
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString("WiFi: " + String(DEFAULT_AP_SSID), 6, 18, 2);

    // Large high-visibility Password using Font 4
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString(apPassword, 6, 36, 4);

    // IP URL info at bottom
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("http://192.168.4.1", 6, 64, 2);
  } else {
    // Normal Wi-Fi Connected View
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("WiFi: " + WiFi.SSID(), 8, 18, 2);

    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString(WiFi.localIP().toString(), 8, 36, 2);

    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString("http://" + mdnsHostname + ".local", 8, 52, 1);

    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.drawString("Press btn to sleep", 8, 68, 1);
  }
}

void showResetPromptScreen() {
  screenOn = true;
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_ON);
  tft.fillScreen(TFT_BLACK);

  // Red Warning Header
  tft.fillRect(0, 0, 160, 18, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.drawString("! FACTORY RESET !", 10, 2, 2);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Press button to", 8, 24, 1);
  tft.drawString("reset to factory default.", 8, 36, 1);

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("Unplug to cancel.", 8, 52, 1);

  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString("Auto-cancel in 20s", 8, 68, 1);
}

void handleTextCommand(const String& text) {
  Serial.printf("[HID] Typing text: '%s' (len=%d)\n", text.c_str(), text.length());
  for (unsigned int i = 0; i < text.length(); i++) {
    size_t res = Keyboard.write(text[i]);
    Serial.printf("[HID] char '%c' (0x%02X) -> res: %u\n", text[i], (uint8_t)text[i], res);
    delay(10);
  }
}

void handleKeyCommand(const String& key) {
  Serial.printf("[HID] Key command: '%s'\n", key.c_str());
  size_t p = 0;
  if (key == "UP") {
    p = Keyboard.press(KEY_UP_ARROW);
    delay(40);
    Keyboard.release(KEY_UP_ARROW);
  } else if (key == "DOWN") {
    p = Keyboard.press(KEY_DOWN_ARROW);
    delay(40);
    Keyboard.release(KEY_DOWN_ARROW);
  } else if (key == "LEFT") {
    p = Keyboard.press(KEY_LEFT_ARROW);
    delay(40);
    Keyboard.release(KEY_LEFT_ARROW);
  } else if (key == "RIGHT") {
    p = Keyboard.press(KEY_RIGHT_ARROW);
    delay(40);
    Keyboard.release(KEY_RIGHT_ARROW);
  } else if (key == "ENTER") {
    p = Keyboard.press(KEY_RETURN);
    delay(40);
    Keyboard.release(KEY_RETURN);
  } else if (key == "BACK") {
    p = Keyboard.press(KEY_ESC);
    delay(40);
    Keyboard.release(KEY_ESC);
  } else if (key == "HOME") {
    p = Keyboard.press(KEY_LEFT_GUI);
    delay(40);
    Keyboard.release(KEY_LEFT_GUI);
  } else if (key == "BACKSPACE") {
    p = Keyboard.press(KEY_BACKSPACE);
    delay(40);
    Keyboard.release(KEY_BACKSPACE);
  } else if (key == "SPACE") {
    p = Keyboard.write(' ');
  } else if (key == "TAB") {
    p = Keyboard.press(KEY_TAB);
    delay(40);
    Keyboard.release(KEY_TAB);
  } else if (key == "VOL_UP") {
    p = ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT);
    delay(40);
    ConsumerControl.release();
  } else if (key == "VOL_DOWN") {
    p = ConsumerControl.press(CONSUMER_CONTROL_VOLUME_DECREMENT);
    delay(40);
    ConsumerControl.release();
  } else if (key == "MUTE") {
    p = ConsumerControl.press(CONSUMER_CONTROL_MUTE);
    delay(40);
    ConsumerControl.release();
  } else if (key == "PLAY_PAUSE") {
    p = ConsumerControl.press(CONSUMER_CONTROL_PLAY_PAUSE);
    delay(40);
    ConsumerControl.release();
  }
  Serial.printf("[HID] Key press sent, res: %u\n", p);
}

// WebSocket Event Handler (Live typing & real-time events)
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
      break;
    case WStype_TEXT: {
      if (length >= 2 && payload[0] == 'T' && payload[1] == ':') {
        String text;
        text.reserve(length - 2);
        for (size_t i = 2; i < length; i++) {
          text += (char)payload[i];
        }
        handleTextCommand(text);
      } else if (length >= 2 && payload[0] == 'K' && payload[1] == ':') {
        char kbuf[32];
        size_t klen = (length - 2 < sizeof(kbuf) - 1) ? length - 2 : sizeof(kbuf) - 1;
        memcpy(kbuf, payload + 2, klen);
        kbuf[klen] = '\0';
        handleKeyCommand(String(kbuf));
      }
      break;
    }
    default:
      break;
  }
}

void setupRoutes() {
  // Main Remote Interface
  server.on("/", HTTP_GET, []() {
    String html = FPSTR(PAGE_INDEX_TEMPLATE);
    html.replace("%ROOM_NAME%", roomName);
    server.send(200, "text/html", html);
  });

  // App Icon (SVG format for home screen and favicon)
  server.on("/icon.svg", HTTP_GET, []() {
    server.send(200, "image/svg+xml", PAGE_ICON_SVG);
  });

  // Serve Web App Manifest for Android / iOS Home Screen install
  server.on("/manifest.json", HTTP_GET, []() {
    String manifest = "{\n"
      "  \"name\": \"" + roomName + " TV Remote\",\n"
      "  \"short_name\": \"" + roomName + " TV\",\n"
      "  \"start_url\": \"/\",\n"
      "  \"display\": \"standalone\",\n"
      "  \"background_color\": \"#10131a\",\n"
      "  \"theme_color\": \"#10131a\",\n"
      "  \"icons\": [\n"
      "    {\n"
      "      \"src\": \"/icon.svg\",\n"
      "      \"sizes\": \"192x192 512x512\",\n"
      "      \"type\": \"image/svg+xml\",\n"
      "      \"purpose\": \"any maskable\"\n"
      "    }\n"
      "  ]\n"
      "}\n";
    server.send(200, "application/manifest+json", manifest);
  });

  // Setup Portal Page: scans available networks and renders setup UI
  server.on("/setup", HTTP_GET, []() {
    int n = WiFi.scanNetworks();
    String wifiOptions = "";
    if (n == 0) {
      wifiOptions = "<option value=\"\">No networks found</option>";
    } else {
      for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        int32_t rssi = WiFi.RSSI(i);
        wifiOptions += "<option value=\"" + ssid + "\">" + ssid + " (" + String(rssi) + " dBm)</option>\n";
      }
    }
    WiFi.scanDelete();

    String html = FPSTR(PAGE_SETUP_TEMPLATE);
    html.replace("%WIFI_OPTIONS%", wifiOptions);
    html.replace("%ROOM_NAME%", roomName);
    html.replace("%MDNS_HOSTNAME%", mdnsHostname);
    server.send(200, "text/html", html);
  });

  // Save Wi-Fi Credentials Endpoint
  server.on("/savewifi", HTTP_POST, []() {
    String ssid = (server.hasArg("ssid_custom") && server.arg("ssid_custom").length() > 0)
                  ? server.arg("ssid_custom") 
                  : server.arg("ssid");
    String password = server.arg("password");
    String newRoom = server.hasArg("room") && server.arg("room").length() > 0 ? server.arg("room") : DEFAULT_ROOM_NAME;
    String newMdns = server.hasArg("hostname") && server.arg("hostname").length() > 0 ? server.arg("hostname") : DEFAULT_MDNS_HOSTNAME;

    if (ssid.length() > 0) {
      // Encrypt Wi-Fi password before committing to NVS
      String encPass = encryptCredential(password);

      prefs.begin("tvremote", false);
      prefs.putString("ssid", ssid);
      prefs.putString("enc_pass", encPass);
      prefs.putString("room", newRoom);
      prefs.putString("mdns", newMdns);
      prefs.end();

      String html = FPSTR(PAGE_SAVED);
      html.replace("%MDNS_HOSTNAME%", newMdns);
      server.send(200, "text/html", html);

      Serial.println("[SETUP] Wi-Fi credentials encrypted & saved! Restarting in 2 seconds...");
      delay(2000);
      ESP.restart();
    } else {
      server.send(400, "text/plain", "SSID cannot be empty.");
    }
  });

  // Fallback HTTP endpoints for text and keys
  server.on("/sendtext", HTTP_POST, []() {
    if (server.hasArg("text")) {
      handleTextCommand(server.arg("text"));
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing text");
    }
  });

  server.on("/sendkey", HTTP_POST, []() {
    if (server.hasArg("key")) {
      handleKeyCommand(server.arg("key"));
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing key");
    }
  });
}
