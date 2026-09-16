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
#include "mbedtls/md.h"

#include "../include/config.h"
#include <LittleFS.h>
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "default_profile.h"
#include "webpage.h"
#include <Update.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>



// USB HID objects
USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;

// Display object (ST7735 80x160, Active-LOW backlight on GPIO 38)
TFT_eSPI tft = TFT_eSPI();

// HTTP Web Server on port 80 & WebSocket Server on port 81 (sub-millisecond keystroke stream)
WebServer server(80);
WebSocketsServer webSocket(81);

// Persistent Storage
Preferences prefs;

// Device runtime identity & network state
String roomName = DEFAULT_ROOM_NAME;
String mdnsHostname = DEFAULT_MDNS_HOSTNAME;
String apPassword = "";
String opMode = "sta"; // "sta" = home Wi-Fi station, "ap" = standalone access point
bool isApMode = false;
bool isConfigured = false;
bool authRequired = false; // User configurable device authorization toggle
bool testDeviceMode = false; // Test Device Mode toggle (beta & pre-releases)

// Hardware HMAC & Encryption Keys
hmac_key_id_t hmacKeySlot = HMAC_KEY_MAX;
bool hmacAvailable = false;
uint8_t derivedKey[32]; // AES-256 derived key for credential & in-transit keystroke encryption
uint8_t authKey[32];    // HMAC derived key for stateless token signing

// Client WebSocket Authentication Tracking
#define MAX_WS_CLIENTS 8
bool wsClientAuthed[MAX_WS_CLIENTS] = {false};

// Physical Button Pairing State Machine
bool pairingPending = false;
String pairingPendingDeviceId = "";
unsigned long pairingStartTime = 0;
bool pairingApproved = false;
String pairingApprovedDeviceId = "";

// Screen power management state
bool screenOn = false;
unsigned long screenTimer = 0;

// Button state machine for Reset & Sleep & Pairing
int lastButtonReading = HIGH;
unsigned long buttonPressStartTime = 0;
bool buttonHeldPast10s = false;
enum SystemState {
  STATE_NORMAL,
  STATE_CONFIRM_RESET
};
SystemState currentState = STATE_NORMAL;
unsigned long resetPromptStartTime = 0;

// Profile & Macro Management
String activeProfileId = DEFAULT_PROFILE_ID;
bool isSdCardAvailable = false;

// Forward declarations
void initCrypto();
bool getOrProvisionHmacKey(hmac_key_id_t &slot);
bool deriveKey(const char* context, uint8_t* outKey32);
String encryptCredential(const String& plain);
String decryptCredential(const String& cipherHex);
String generateRandomPassword(size_t length = 8);
String htmlEscape(const String& input);
String sanitizeHostname(const String& input);
String generateSignedToken(const String& deviceId);
bool verifyDeviceToken(const String& token);
String decryptPayload(const String& encPacket);
void startAccessPoint();
void connectToSavedWifi();
void wakeScreen();
void sleepScreen();
void updateScreenContent();
void showResetPromptScreen();
void showPairingPromptScreen();
void showPairingSuccessScreen();
void setupRoutes();
void handleKeyCommand(const String& key);
void handleTextCommand(const String& text);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

// Profile & Macro Prototypes
void initStorage();
String getActiveProfileJson();
String listProfilesJson();
bool setActiveProfile(const String& id);
bool saveProfile(const String& id, const String& jsonContent);
bool deleteProfile(const String& id);
uint8_t parseDuckyKey(const String& k);
uint16_t parseConsumerKey(const String& k);
void executeDuckyScript(const String& script);


// =========================================================================
// Input Sanitization & String Helpers (F-05 XSS Mitigation)
// =========================================================================
String htmlEscape(const String& input) {
  String out = "";
  out.reserve(input.length() + 16);
  for (size_t i = 0; i < input.length(); i++) {
    char c = input[i];
    switch (c) {
      case '&':  out += "&amp;"; break;
      case '<':  out += "&lt;"; break;
      case '>':  out += "&gt;"; break;
      case '"':  out += "&quot;"; break;
      case '\'': out += "&#39;"; break;
      default:   out += c; break;
    }
  }
  return out;
}

String sanitizeHostname(const String& input) {
  String out = "";
  out.reserve(input.length());
  for (size_t i = 0; i < input.length(); i++) {
    char c = tolower(input[i]);
    if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-') {
      out += c;
    }
  }
  if (out.length() == 0) out = DEFAULT_MDNS_HOSTNAME;
  if (out.length() > 32) out = out.substring(0, 32);
  return out;
}

// =========================================================================
// Crypto & eFuse Key Management (Hardware HMAC + AES-256 CTR)
// =========================================================================
void initCrypto() {
  if (getOrProvisionHmacKey(hmacKeySlot)) {
    hmacAvailable = true;
    Serial.printf("[SECURITY] Hardware HMAC Key active in eFuse slot KEY%d\n", (int)hmacKeySlot);

    // Derive AES-256 encryption key with domain separation
    if (deriveKey("project-wifi-v1", derivedKey)) {
      Serial.println("[SECURITY] AES-256 master key derived from Hardware HMAC.");
    } else {
      Serial.println("[SECURITY] ERROR: Failed to derive AES key!");
      hmacAvailable = false;
    }

    // Derive token signing key with distinct domain separation
    if (deriveKey("project-auth-v1", authKey)) {
      Serial.println("[SECURITY] Device authentication key derived from Hardware HMAC.");
    } else {
      Serial.println("[SECURITY] ERROR: Failed to derive Auth key!");
      hmacAvailable = false;
    }
  } else {
    Serial.println("[SECURITY] WARNING: No HMAC key available and could not allocate eFuse slot.");
    hmacAvailable = false;
  }
}

// Auto-Detect & Reuse eFuse HMAC key slot (GEMINI.md Rule)
bool getOrProvisionHmacKey(hmac_key_id_t &slot) {
  // Step 1: Scan KEY0 through KEY5 for an existing ESP_EFUSE_KEY_PURPOSE_HMAC_UP
  for (int i = 0; i < 6; i++) {
    esp_efuse_block_t blk = (esp_efuse_block_t)(EFUSE_BLK_KEY0 + i);
    esp_efuse_purpose_t purpose = esp_efuse_get_key_purpose(blk);
    if (purpose == ESP_EFUSE_KEY_PURPOSE_HMAC_UP) {
      slot = (hmac_key_id_t)i;
      Serial.printf("[SECURITY] Reusing existing Hardware HMAC key in eFuse KEY%d.\n", i);
      return true;
    }
  }

  // Step 2: No existing slot. Allocate first empty slot scanning from KEY5 downwards
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

// Encrypt string using AES-256 CTR mode. (Fail-Closed: returns "" on failure)
String encryptCredential(const String& plain) {
  if (!hmacAvailable || plain.length() == 0) return ""; // Fail-closed!

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

// Decrypt Hex(Nonce + Ciphertext) using AES-256 CTR (Fail-Closed: returns "" on failure)
String decryptCredential(const String& cipherHex) {
  if (!hmacAvailable || cipherHex.length() < 34) return ""; // Fail-closed!

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
  plaintext[cipherLen] = '\0';

  mbedtls_aes_free(&aes);
  free(raw);

  String result = (char*)plaintext;
  free(plaintext);
  return result;
}

// Decrypt In-Transit Keystroke Payload (Format: E:<nonce16_hex>:<cipher_hex>)
String decryptPayload(const String& encPacket) {
  if (!encPacket.startsWith("E:") || !hmacAvailable) return encPacket;

  int firstColon = encPacket.indexOf(':');
  int secondColon = encPacket.indexOf(':', firstColon + 1);
  if (firstColon < 0 || secondColon < 0) return "";

  String nonceHex = encPacket.substring(firstColon + 1, secondColon);
  String cipherHex = encPacket.substring(secondColon + 1);

  if (nonceHex.length() != 32 || cipherHex.length() == 0 || (cipherHex.length() % 2 != 0)) {
    return "";
  }

  uint8_t nonce[16];
  for (int i = 0; i < 16; i++) {
    char b[3] = { nonceHex[i * 2], nonceHex[i * 2 + 1], '\0' };
    nonce[i] = (uint8_t)strtoul(b, NULL, 16);
  }

  size_t cipherLen = cipherHex.length() / 2;
  uint8_t* cipherBytes = (uint8_t*)malloc(cipherLen);
  uint8_t* plainBytes = (uint8_t*)malloc(cipherLen + 1);
  if (!cipherBytes || !plainBytes) {
    if (cipherBytes) free(cipherBytes);
    if (plainBytes) free(plainBytes);
    return "";
  }

  for (size_t i = 0; i < cipherLen; i++) {
    char b[3] = { cipherHex[i * 2], cipherHex[i * 2 + 1], '\0' };
    cipherBytes[i] = (uint8_t)strtoul(b, NULL, 16);
  }

  uint8_t nonce_counter[16];
  memcpy(nonce_counter, nonce, 16);

  size_t nc_off = 0;
  uint8_t stream_block[16];
  memset(stream_block, 0, sizeof(stream_block));

  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, derivedKey, 256);
  mbedtls_aes_crypt_ctr(&aes, cipherLen, &nc_off, nonce_counter, stream_block, cipherBytes, plainBytes);
  mbedtls_aes_free(&aes);

  plainBytes[cipherLen] = '\0';
  String result = (char*)plainBytes;

  free(cipherBytes);
  free(plainBytes);
  return result;
}

// =========================================================================
// Authentication & Stateless Token Management (Method B)
// =========================================================================
String generateSignedToken(const String& deviceId) {
  uint8_t hmacOut[32];
  mbedtls_md_context_t ctx;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
  mbedtls_md_hmac_starts(&ctx, authKey, 32);
  mbedtls_md_hmac_update(&ctx, (const unsigned char*)deviceId.c_str(), deviceId.length());
  mbedtls_md_hmac_finish(&ctx, hmacOut);
  mbedtls_md_free(&ctx);

  String token = deviceId + "_";
  for (int i = 0; i < 16; i++) { // 16 bytes = 32 hex chars
    char buf[3];
    sprintf(buf, "%02x", hmacOut[i]);
    token += buf;
  }
  return token;
}

bool verifyDeviceToken(const String& token) {
  if (!authRequired) return true;
  if (token.length() < 34) return false;

  int underscoreIdx = token.indexOf('_');
  if (underscoreIdx <= 0) return false;

  String deviceId = token.substring(0, underscoreIdx);
  String expected = generateSignedToken(deviceId);

  // Constant-time comparison
  if (token.length() != expected.length()) return false;
  uint8_t diff = 0;
  for (size_t i = 0; i < token.length(); i++) {
    diff |= (token[i] ^ expected[i]);
  }
  return (diff == 0);
}

// Cryptographically random 8-character password from Hardware TRNG
String generateRandomPassword(size_t length) {
  const char charset[] = "23456789abcdefghjkmnpqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";
  size_t charsetSize = sizeof(charset) - 1;
  String pwd = "";
  pwd.reserve(length);
  for (size_t i = 0; i < length; i++) {
    uint32_t r = esp_random();
    pwd += charset[r % charsetSize];
  }
  return pwd;
}

// =========================================================================
// Network & Setup
// =========================================================================
void startAccessPoint() {
  isApMode = true;
  isConfigured = false;
  WiFi.mode(WIFI_AP);
  if (apPassword.length() == 0) {
    apPassword = generateRandomPassword(8);
  }
  WiFi.softAP(DEFAULT_AP_SSID, apPassword.c_str());
  Serial.printf("[AP] Setup Access Point started! SSID: %s\n", DEFAULT_AP_SSID);

  wakeScreen();
}

void connectToSavedWifi() {
  prefs.begin("tvremote", true);
  opMode = prefs.getString("op_mode", "sta");
  String ssid = prefs.getString("ssid", "");
  String encPass = prefs.getString("enc_pass", "");
  roomName = prefs.getString("room", DEFAULT_ROOM_NAME);
  mdnsHostname = prefs.getString("mdns", DEFAULT_MDNS_HOSTNAME);
  authRequired = prefs.getBool("auth_req", false);
  testDeviceMode = prefs.getBool("test_dev", false);
  prefs.end();

  if (ssid.length() == 0) {
    Serial.println("[WIFI] No stored credentials found in NVS flash. Launching setup AP...");
    startAccessPoint();
    return;
  }

  String password = decryptCredential(encPass);

  // Mode A: Standalone Private Access Point
  if (opMode == "ap") {
    isApMode = false;
    isConfigured = true;
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid.c_str(), password.c_str());
    Serial.printf("[WIFI] Standalone AP mode active! SSID: %s, IP: %s\n", ssid.c_str(), WiFi.softAPIP().toString().c_str());
    wakeScreen();
    return;
  }

  // Mode B: Station Mode (Connect to Home Wi-Fi)
  Serial.printf("[WIFI] Connecting to network: %s\n", ssid.c_str());
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
    isConfigured = true;
    Serial.printf("[WIFI] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
    wakeScreen();
  } else {
    Serial.println("[WIFI] Connection failed. Launching fallback AP...");
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

  // Initialize Dynamic Profile Storage (LittleFS / SD)
  initStorage();

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

  // Check pairing request timeout
  if (pairingPending && (millis() - pairingStartTime > PAIRING_TIMEOUT_MS)) {
    pairingPending = false;
    Serial.println("[SECURITY] Pairing request timed out.");
    updateScreenContent();
  }

  // Serial command handler (WebSerial USB Profile Deploy)
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.startsWith("PROFILE_UPLOAD:")) {
      String jsonContent = line.substring(15);
      JsonDocument doc;
      DeserializationError err = deserializeJson(doc, jsonContent);
      if (!err && doc.containsKey("id")) {
        String id = doc["id"].as<String>();
        if (saveProfile(id, jsonContent)) {
          setActiveProfile(id);
          Serial.println("[USB] Profile saved and set active: " + id);
        } else {
          Serial.println("[USB] Error saving profile to storage");
        }
      } else {
        Serial.println("[USB] Invalid JSON received");
      }
    }
  }

  // ==========================================
  // Physical Button & Factory Reset State Machine
  // ==========================================
  int reading = digitalRead(PIN_BUTTON);

  if (currentState == STATE_NORMAL) {
    if (reading == LOW) {
      if (lastButtonReading == HIGH) {
        buttonPressStartTime = millis();
        buttonHeldPast10s = false;
      } else {
        if (!buttonHeldPast10s && (millis() - buttonPressStartTime >= FACTORY_RESET_HOLD_MS)) {
          buttonHeldPast10s = true;
          currentState = STATE_CONFIRM_RESET;
          resetPromptStartTime = millis();
          showResetPromptScreen();
          Serial.println("[RESET] 10s hold reached! Confirmation prompt displayed.");
        }
      }
    } else {
      if (lastButtonReading == LOW) {
        if (!buttonHeldPast10s) {
          // Short button press
          if (pairingPending) {
            // Physical button clicked to approve pairing!
            pairingApproved = true;
            pairingApprovedDeviceId = pairingPendingDeviceId;
            pairingPending = false;
            showPairingSuccessScreen();
            Serial.println("[SECURITY] Device pairing approved via physical button press!");
          } else {
            // Toggle screen sleep/wake
            if (!screenOn) {
              wakeScreen();
            } else if (!isApMode) {
              sleepScreen();
            }
          }
        }
      }
    }
  } else if (currentState == STATE_CONFIRM_RESET) {
    if (lastButtonReading == HIGH && reading == LOW) {
      Serial.println("[RESET] Factory reset confirmed! Clearing NVS...");
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

    if (millis() - resetPromptStartTime > RESET_CONFIRM_TIMEOUT) {
      Serial.println("[RESET] Confirmation timed out. Reset canceled.");
      currentState = STATE_NORMAL;
      sleepScreen();
    }
  }

  lastButtonReading = reading;

  // Auto screen timeout in normal station mode (keep screen ON continuously in AP mode)
  if (screenOn && !isApMode && currentState == STATE_NORMAL && !pairingPending && (millis() - screenTimer > SCREEN_TIMEOUT_MS)) {
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
}

void sleepScreen() {
  screenOn = false;
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_OFF);
  tft.fillScreen(TFT_BLACK);
}

void updateScreenContent() {
  tft.fillScreen(TFT_BLACK);

  // Title bar
  tft.fillRect(0, 0, 160, 16, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawString(roomName + " TV", 8, 2, 2);

  if (isApMode && !isConfigured) {
    // Fallback AP Mode View (Physical Proximity Barrier)
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString("WiFi: " + String(DEFAULT_AP_SSID), 6, 18, 2);

    // Large high-visibility Password using Font 4
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString(apPassword, 6, 36, 4);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("http://192.168.4.1", 6, 64, 2);
  } else if (opMode == "ap") {
    // Standalone AP Mode
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("AP: Active", 8, 18, 2);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString("http://192.168.4.1", 8, 36, 2);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString("http://" + mdnsHostname + ".local", 8, 54, 2);
  } else {
    // Normal Wi-Fi Connected View
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("WiFi: " + WiFi.SSID(), 8, 18, 2);

    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString(WiFi.localIP().toString(), 8, 36, 2);

    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString("http://" + mdnsHostname + ".local", 8, 54, 2);
  }
}

void showResetPromptScreen() {
  screenOn = true;
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_ON);
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, 160, 18, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.drawString("! FACTORY RESET !", 10, 2, 2);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Press button to reset", 6, 24, 2);
  tft.drawString("to factory default.", 6, 42, 2);
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft.drawString("Unplug to cancel.", 14, 62, 2);
}

void showPairingPromptScreen() {
  screenOn = true;
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_ON);
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, 160, 18, TFT_ORANGE);
  tft.setTextColor(TFT_BLACK, TFT_ORANGE);
  tft.drawString("PAIR NEW PHONE?", 14, 2, 2);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Click button on stick", 6, 26, 2);
  tft.drawString("to approve device.", 16, 44, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Expires in 30s", 34, 64, 1);
}

void showPairingSuccessScreen() {
  screenOn = true;
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_ON);
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, 160, 18, TFT_GREEN);
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.drawString("DEVICE APPROVED!", 12, 2, 2);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Phone paired \u2713", 26, 32, 2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("Remote Active", 30, 52, 2);

  screenTimer = millis();
}

// =========================================================================
// Over-The-Air (OTA) Display Functions & Cloud Update Engine
// =========================================================================
void showOtaProgressScreen(const String& label, int percent) {
  screenOn = true;
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_ON);
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, 160, 18, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawString("FIRMWARE OTA", 24, 2, 2);

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString(label.substring(0, 15), 8, 24, 2);

  // Draw progress bar outline (width: 144, height: 14)
  tft.drawRect(8, 44, 144, 14, TFT_WHITE);
  int fillW = (percent * 140) / 100;
  if (fillW > 0) {
    tft.fillRect(10, 46, fillW, 10, TFT_GREEN);
  }

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString(String(percent) + "% - Do not unplug!", 8, 62, 1);
}

void showOtaSuccessScreen(const String& label) {
  screenOn = true;
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_ON);
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, 160, 18, TFT_GREEN);
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.drawString("UPDATE COMPLETE!", 12, 2, 2);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(label.substring(0, 15), 14, 28, 2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("Rebooting...", 32, 52, 2);
}

void showOtaErrorScreen(const String& err) {
  screenOn = true;
  digitalWrite(PIN_LCD_BL, LCD_BACKLIGHT_ON);
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, 160, 18, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.drawString("UPDATE FAILED", 22, 2, 2);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString(err.substring(0, 18), 8, 30, 1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Continuing current OS", 10, 52, 1);
}

bool performCloudUpdate(const String& url, const String& versionTag) {
  Serial.printf("[OTA] Starting cloud update to %s from URL: %s\n", versionTag.c_str(), url.c_str());
  showOtaProgressScreen(versionTag, 0);

  WiFiClientSecure client;
  client.setInsecure(); // GitHub CDN redirect storage uses wildcard certificates

  httpUpdate.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  httpUpdate.rebootOnUpdate(false);

  httpUpdate.onProgress([versionTag](size_t current, size_t total) {
    if (total > 0) {
      int pct = (current * 100) / total;
      static int lastPct = -1;
      if (pct != lastPct) {
        lastPct = pct;
        showOtaProgressScreen(versionTag, pct);
        Serial.printf("[OTA] Progress: %d%% (%u / %u bytes)\n", pct, current, total);
      }
    }
  });

  t_httpUpdate_return ret = httpUpdate.update(client, url);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("[OTA] HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      showOtaErrorScreen(httpUpdate.getLastErrorString());
      delay(4000);
      updateScreenContent();
      return false;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[OTA] HTTP_UPDATE_NO_UPDATES");
      return false;

    case HTTP_UPDATE_OK:
      Serial.println("[OTA] HTTP_UPDATE_OK! Rebooting into new firmware...");
      showOtaSuccessScreen(versionTag);
      delay(2000);
      ESP.restart();
      return true;
  }
  return false;
}

// =========================================================================
// Dynamic Profile Storage & File System Manager
// =========================================================================
void initStorage() {
  if (!LittleFS.begin(true)) {
    Serial.println("[STORAGE] LittleFS Mount Failed!");
  } else {
    Serial.println("[STORAGE] LittleFS Mounted successfully.");
  }

  // Check SD card on SPI pins
  SPIClass sdSpi(FSPI);
  sdSpi.begin(PIN_SD_CLK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
  if (SD.begin(PIN_SD_CS, sdSpi)) {
    isSdCardAvailable = true;
    Serial.println("[STORAGE] MicroSD card detected and mounted!");
    if (!SD.exists(PROFILES_DIR)) SD.mkdir(PROFILES_DIR);
  } else {
    isSdCardAvailable = false;
    Serial.println("[STORAGE] MicroSD card not present, using internal LittleFS.");
  }

  if (!LittleFS.exists(PROFILES_DIR)) {
    LittleFS.mkdir(PROFILES_DIR);
  }

  // Always sync default-tv.json with current firmware profile
  File fTv = LittleFS.open("/profiles/default-tv.json", "w");
  if (fTv) {
    fTv.print(FPSTR(DEFAULT_PROFILE_TV));
    fTv.close();
    Serial.println("[STORAGE] Synced built-in default-tv.json");
  }

  if (!LittleFS.exists("/profiles/pc-media.json")) {
    File f = LittleFS.open("/profiles/pc-media.json", "w");
    if (f) {
      f.print(FPSTR(DEFAULT_PROFILE_PC));
      f.close();
      Serial.println("[STORAGE] Installed built-in pc-media.json");
    }
  }

  if (LittleFS.exists(ACTIVE_PROFILE_FILE)) {
    File f = LittleFS.open(ACTIVE_PROFILE_FILE, "r");
    if (f) {
      String id = f.readStringUntil('\n');
      id.trim();
      if (id.length() > 0) activeProfileId = id;
      f.close();
    }
  } else {
    File f = LittleFS.open(ACTIVE_PROFILE_FILE, "w");
    if (f) {
      f.println(DEFAULT_PROFILE_ID);
      f.close();
    }
  }
  Serial.printf("[STORAGE] Active profile: %s\n", activeProfileId.c_str());
}

String getActiveProfileJson() {
  String path = "/profiles/" + activeProfileId + ".json";
  if (isSdCardAvailable && SD.exists(path)) {
    File f = SD.open(path, "r");
    if (f) {
      String content = f.readString();
      f.close();
      return content;
    }
  }
  if (LittleFS.exists(path)) {
    File f = LittleFS.open(path, "r");
    if (f) {
      String content = f.readString();
      f.close();
      return content;
    }
  }
  return FPSTR(DEFAULT_PROFILE_TV);
}

String listProfilesJson() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();

  File dir = LittleFS.open(PROFILES_DIR);
  if (dir && dir.isDirectory()) {
    File file = dir.openNextFile();
    while (file) {
      String fname = file.name();
      if (fname.endsWith(".json")) {
        String content = file.readString();
        JsonDocument pdoc;
        DeserializationError err = deserializeJson(pdoc, content);
        if (!err) {
          JsonObject item = arr.add<JsonObject>();
          String pid = pdoc["id"] | fname.substring(0, fname.length() - 5);
          item["id"] = pid;
          item["name"] = pdoc["name"] | pid;
          item["icon"] = pdoc["icon"] | "tv";
          item["deviceType"] = pdoc["deviceType"] | "tv";
          item["active"] = (pid == activeProfileId);
        }
      }
      file = dir.openNextFile();
    }
  }
  String out;
  serializeJson(arr, out);
  return out;
}

bool setActiveProfile(const String& id) {
  String safeId = sanitizeHostname(id);
  if (safeId.length() == 0) return false;
  String path = "/profiles/" + safeId + ".json";
  if (!LittleFS.exists(path) && (!isSdCardAvailable || !SD.exists(path))) {
    return false;
  }
  activeProfileId = safeId;
  File f = LittleFS.open(ACTIVE_PROFILE_FILE, "w");
  if (f) {
    f.println(safeId);
    f.close();
  }
  Serial.printf("[STORAGE] Switched active profile to: %s\n", safeId.c_str());
  return true;
}

bool saveProfile(const String& id, const String& jsonContent) {
  String safeId = sanitizeHostname(id);
  if (safeId.length() == 0 || jsonContent.length() == 0) return false;

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, jsonContent);
  if (err) {
    Serial.printf("[STORAGE] JSON parse error: %s\n", err.c_str());
    return false;
  }

  doc["id"] = safeId;
  String path = "/profiles/" + safeId + ".json";
  File f = LittleFS.open(path, "w");
  if (!f) return false;
  serializeJson(doc, f);
  f.close();
  Serial.printf("[STORAGE] Saved profile: %s\n", safeId.c_str());
  return true;
}

bool deleteProfile(const String& id) {
  String safeId = sanitizeHostname(id);
  if (safeId == DEFAULT_PROFILE_ID || safeId == "default-tv") {
    return false;
  }
  String path = "/profiles/" + safeId + ".json";
  if (LittleFS.exists(path)) {
    LittleFS.remove(path);
  }
  if (isSdCardAvailable && SD.exists(path)) {
    SD.remove(path);
  }
  if (activeProfileId == safeId) {
    setActiveProfile(DEFAULT_PROFILE_ID);
  }
  return true;
}

// =========================================================================
// DuckyScript Parser & Consumer Control Mapping
// =========================================================================
uint8_t parseDuckyKey(const String& k) {
  String u = k;
  u.toUpperCase();
  u.trim();
  if (u == "ENTER" || u == "RETURN" || u == "OK") return KEY_RETURN;
  if (u == "ESC" || u == "ESCAPE" || u == "BACK") return KEY_ESC;
  if (u == "BACKSPACE" || u == "BKSP") return KEY_BACKSPACE;
  if (u == "TAB" || u == "INFO") return KEY_TAB;
  if (u == "SPACE") return ' ';
  if (u == "CAPSLOCK" || u == "CAPS") return KEY_CAPS_LOCK;
  if (u == "DELETE" || u == "DEL") return KEY_DELETE;
  if (u == "INSERT" || u == "INS") return KEY_INSERT;
  if (u == "UP" || u == "UPARROW") return KEY_UP_ARROW;
  if (u == "DOWN" || u == "DOWNARROW") return KEY_DOWN_ARROW;
  if (u == "LEFT" || u == "LEFTARROW") return KEY_LEFT_ARROW;
  if (u == "RIGHT" || u == "RIGHTARROW") return KEY_RIGHT_ARROW;
  if (u == "PAGEUP" || u == "PAGE_UP") return KEY_PAGE_UP;
  if (u == "PAGEDOWN" || u == "PAGE_DOWN") return KEY_PAGE_DOWN;
  if (u == "HOME") return KEY_LEFT_GUI;
  if (u == "END") return KEY_END;
  if (u == "PRINTSCREEN" || u == "PRTSCN") return 0xCE;
  if (u == "SCROLLLOCK") return 0xCF;
  if (u == "PAUSE" || u == "BREAK") return 0xD0;
  if (u == "F1") return KEY_F1;
  if (u == "F2") return KEY_F2;
  if (u == "F3") return KEY_F3;
  if (u == "F4") return KEY_F4;
  if (u == "F5") return KEY_F5;
  if (u == "F6") return KEY_F6;
  if (u == "F7") return KEY_F7;
  if (u == "F8") return KEY_F8;
  if (u == "F9") return KEY_F9;
  if (u == "F10") return KEY_F10;
  if (u == "F11") return KEY_F11;
  if (u == "F12") return KEY_F12;
  if (u.startsWith("KEY_") && u.length() == 5) return (uint8_t)u[4];
  if (u.length() == 1) return (uint8_t)u[0];
  return 0;
}

uint16_t parseConsumerKey(const String& k) {
  String u = k;
  u.toUpperCase();
  u.trim();
  if (u == "VOL_UP" || u == "VOLUMEUP" || u == "VOLUP") return CONSUMER_CONTROL_VOLUME_INCREMENT;
  if (u == "VOL_DOWN" || u == "VOLUMEDOWN" || u == "VOLDOWN") return CONSUMER_CONTROL_VOLUME_DECREMENT;
  if (u == "MUTE") return CONSUMER_CONTROL_MUTE;
  if (u == "PLAY_PAUSE" || u == "PLAY" || u == "PAUSE") return CONSUMER_CONTROL_PLAY_PAUSE;
  if (u == "NEXT" || u == "NEXT_TRACK") return CONSUMER_CONTROL_SCAN_NEXT;
  if (u == "PREV" || u == "PREV_TRACK") return CONSUMER_CONTROL_SCAN_PREVIOUS;
  if (u == "STOP") return CONSUMER_CONTROL_STOP;
  if (u == "POWER") return CONSUMER_CONTROL_POWER;
  return 0;
}


void executeDuckyScript(const String& script) {
  wakeScreen();
  int start = 0;
  int len = script.length();
  String lastCommand = "";

  Serial.printf("[MACRO] Executing DuckyScript (%d bytes)...\n", len);

  while (start < len) {
    int end = script.indexOf('\n', start);
    if (end == -1) end = len;
    String line = script.substring(start, end);
    line.trim();
    start = end + 1;

    if (line.length() == 0 || line.startsWith("REM ") || line == "REM") continue;

    if (line.startsWith("DELAY ")) {
      int ms = line.substring(6).toInt();
      if (ms > 0 && ms <= 10000) delay(ms);
      continue;
    }

    if (line.startsWith("STRING ")) {
      String text = line.substring(7);
      Keyboard.print(text);
      lastCommand = line;
      continue;
    }

    if (line.startsWith("REPEAT ")) {
      int count = line.substring(7).toInt();
      if (count > 0 && count <= 30 && lastCommand.length() > 0) {
        for (int i = 0; i < count; i++) {
          executeDuckyScript(lastCommand);
        }
      }
      continue;
    }

    // Check modifier combos
    bool hasGui = false, hasCtrl = false, hasAlt = false, hasShift = false;
    String remaining = line;
    bool matchedMod = true;
    while (matchedMod && remaining.length() > 0) {
      matchedMod = false;
      if (remaining.startsWith("GUI ") || remaining.startsWith("WINDOWS ") || remaining.startsWith("COMMAND ")) {
        hasGui = true;
        remaining = remaining.substring(remaining.indexOf(' ') + 1);
        remaining.trim();
        matchedMod = true;
      } else if (remaining.startsWith("CTRL ") || remaining.startsWith("CONTROL ")) {
        hasCtrl = true;
        remaining = remaining.substring(remaining.indexOf(' ') + 1);
        remaining.trim();
        matchedMod = true;
      } else if (remaining.startsWith("ALT ")) {
        hasAlt = true;
        remaining = remaining.substring(remaining.indexOf(' ') + 1);
        remaining.trim();
        matchedMod = true;
      } else if (remaining.startsWith("SHIFT ")) {
        hasShift = true;
        remaining = remaining.substring(remaining.indexOf(' ') + 1);
        remaining.trim();
        matchedMod = true;
      }
    }

    if (hasGui || hasCtrl || hasAlt || hasShift) {
      if (hasGui) Keyboard.press(KEY_LEFT_GUI);
      if (hasCtrl) Keyboard.press(KEY_LEFT_CTRL);
      if (hasAlt) Keyboard.press(KEY_LEFT_ALT);
      if (hasShift) Keyboard.press(KEY_LEFT_SHIFT);

      if (remaining.length() > 0) {
        uint8_t k = parseDuckyKey(remaining);
        if (k != 0) Keyboard.press(k);
        else if (remaining.length() == 1) Keyboard.press(remaining[0]);
      }
      delay(50);
      Keyboard.releaseAll();
      lastCommand = line;
      continue;
    }

    // Check consumer keys
    uint16_t ck = parseConsumerKey(line);
    if (ck != 0) {
      ConsumerControl.press(ck);
      delay(50);
      ConsumerControl.release();
      lastCommand = line;
      continue;
    }

    // Check single key
    uint8_t dk = parseDuckyKey(line);
    if (dk != 0) {
      Keyboard.press(dk);
      delay(40);
      Keyboard.release(dk);
      lastCommand = line;
      continue;
    }

    // Fallback single character
    if (line.length() == 1) {
      Keyboard.write(line[0]);
      lastCommand = line;
      continue;
    }
  }
}

// =========================================================================
// Command Execution & Logging Scrubbing (F-06)
// =========================================================================
void handleTextCommand(const String& text) {
  if (text.length() == 0) return;
  wakeScreen();
  Keyboard.print(text);
  // F-06: Log character length only; never print raw keystroke text to serial
  Serial.printf("[HID] Dispatched text string (%d characters)\n", (int)text.length());
}

void handleKeyCommand(const String& key) {
  wakeScreen();
  size_t p = 0;
  uint16_t ck = parseConsumerKey(key);
  if (ck != 0) {
    p = ConsumerControl.press(ck);
    delay(40);
    ConsumerControl.release();
    Serial.printf("[HID] Dispatched consumer key: %s\n", key.c_str());
    return;
  }

  if (key == "CLEAR") {
    // Smart Clear: Ctrl+A -> Backspace + rapid burst of backspaces
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('a');
    delay(30);
    Keyboard.releaseAll();
    delay(20);
    Keyboard.press(KEY_BACKSPACE);
    delay(20);
    Keyboard.release(KEY_BACKSPACE);
    delay(10);
    for (int i = 0; i < 25; i++) {
      Keyboard.press(KEY_BACKSPACE);
      delay(5);
      Keyboard.release(KEY_BACKSPACE);
      delay(5);
    }
    Serial.println("[HID] Dispatched Smart Clear sequence to TV");
    return;
  }

  if (key == "SPACE") {
    p = Keyboard.write(' ');
  } else {
    uint8_t dk = parseDuckyKey(key);
    if (dk != 0) {
      p = Keyboard.press(dk);
      delay(40);
      Keyboard.release(dk);
    }
  }
  Serial.printf("[HID] Dispatched key: %s (status: %u)\n", key.c_str(), (unsigned)p);
}

// WebSocket Event Handler (In-transit Decryption & Authentication)
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (num >= MAX_WS_CLIENTS) return;

  switch (type) {
    case WStype_DISCONNECTED:
      wsClientAuthed[num] = false;
      break;

    case WStype_CONNECTED:
      // If auth is not required, client is automatically authorized
      wsClientAuthed[num] = !authRequired;
      break;

    case WStype_TEXT: {
      String msg = "";
      msg.reserve(length);
      for (size_t i = 0; i < length; i++) msg += (char)payload[i];

      // Handle Authentication Handshake
      if (msg.startsWith("AUTH:")) {
        String token = msg.substring(5);
        if (verifyDeviceToken(token)) {
          wsClientAuthed[num] = true;
          webSocket.sendTXT(num, "AUTH_OK");
          Serial.printf("[SECURITY] WS Client #%u authenticated successfully.\n", num);
        } else {
          wsClientAuthed[num] = false;
          webSocket.sendTXT(num, "AUTH_REQUIRED");
          Serial.printf("[SECURITY] WS Client #%u presented invalid auth token!\n", num);
        }
        return;
      }

      // If auth is required and client is not authed, reject message
      if (authRequired && !wsClientAuthed[num]) {
        webSocket.sendTXT(num, "AUTH_REQUIRED");
        return;
      }

      // Handle In-Transit Decryption (Item 1 Option A)
      if (msg.startsWith("E:")) {
        msg = decryptPayload(msg);
      }

      if (msg.startsWith("T:")) {
        handleTextCommand(msg.substring(2));
      } else if (msg.startsWith("K:")) {
        handleKeyCommand(msg.substring(2));
      } else if (msg.startsWith("M:")) {
        executeDuckyScript(msg.substring(2));
      }
      break;
    }


    default:
      break;
  }
}

// =========================================================================
// HTTP Routes & Security Endpoints
// =========================================================================
void setupRoutes() {
  server.enableCORS(true);

  // Main Remote Interface (Isolated in unconfigured AP setup mode)
  server.on("/", HTTP_GET, []() {
    if (isApMode && !isConfigured) {
      // AP Setup Mode Isolation: Force setup page only
      server.sendHeader("Location", "/setup");
      server.send(302, "text/plain", "Redirecting to setup portal...");
      return;
    }

    String html = FPSTR(PAGE_INDEX_TEMPLATE);
    html.replace("%ROOM_NAME%", htmlEscape(roomName));
    html.replace("%AUTH_REQUIRED%", authRequired ? "true" : "false");
    html.replace("%ACTIVE_PROFILE_JSON%", getActiveProfileJson());
    server.send(200, "text/html", html);
  });

  // App Icon (SVG format for home screen and favicon)
  server.on("/icon.svg", HTTP_GET, []() {
    server.send(200, "image/svg+xml", PAGE_ICON_SVG);
  });

  // Serve Web App Manifest
  server.on("/manifest.json", HTTP_GET, []() {
    String manifest = "{\n"
      "  \"name\": \"" + htmlEscape(roomName) + " TV Remote\",\n"
      "  \"short_name\": \"" + htmlEscape(roomName) + " TV\",\n"
      "  \"start_url\": \"/\",\n"
      "  \"display\": \"standalone\",\n"
      "  \"background_color\": \"#10131a\",\n"
      "  \"theme_color\": \"#10131a\",\n"
      "  \"icons\": [\n"
      "    {\n"
      "      \"src\": \"/icon.svg\",\n"
      "      \"sizes\": \"512x512\",\n"
      "      \"type\": \"image/svg+xml\"\n"
      "    }\n"
      "  ]\n"
      "}\n";
    server.send(200, "application/manifest+json", manifest);
  });

  // Companion Layout & Macro Designer Studio (Hosted on GitHub Pages)
  server.on("/designer", HTTP_GET, []() {
    server.sendHeader("Location", "https://sblaisdev.github.io/lilygo-wifi-tv-remote/", true);
    server.send(302, "text/plain", "Redirecting to GitHub Hosted Designer...");
  });

  // Setup Portal Page

  server.on("/setup", HTTP_GET, []() {
    int n = WiFi.scanNetworks();
    String wifiOptions = "";
    if (n == 0) {
      wifiOptions = "<option value=\"\">No networks found</option>";
    } else {
      for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        int32_t rssi = WiFi.RSSI(i);
        String escapedSsid = htmlEscape(ssid);
        wifiOptions += "<option value=\"" + escapedSsid + "\">" + escapedSsid + " (" + String(rssi) + " dBm)</option>\n";
      }
    }
    WiFi.scanDelete();

    String html = FPSTR(PAGE_SETUP_TEMPLATE);
    html.replace("%WIFI_OPTIONS%", wifiOptions);
    html.replace("%ROOM_NAME%", htmlEscape(roomName));
    html.replace("%MDNS_HOSTNAME%", htmlEscape(mdnsHostname));
    html.replace("%AP_SSID%", htmlEscape(roomName + "-TV-Remote"));
    html.replace("%MODE_STA_SELECTED%", opMode == "ap" ? "" : "selected");
    html.replace("%MODE_AP_SELECTED%", opMode == "ap" ? "selected" : "");
    html.replace("%AUTH_CHECKED%", authRequired ? "checked" : "");
    html.replace("%FIRMWARE_VERSION%", FIRMWARE_VERSION);
    html.replace("%GITHUB_REPO%", GITHUB_REPO);
    html.replace("%TEST_DEVICE_CHECKED%", testDeviceMode ? "checked" : "");
    html.replace("%TEST_DEVICE_BADGE_DISPLAY%", testDeviceMode ? "block" : "none");
    html.replace("%CUSTOM_URL_DISPLAY%", testDeviceMode ? "block" : "none");

    // Profile Options for Quick-Switcher
    String profileOpts = "";
    JsonDocument pListDoc;
    deserializeJson(pListDoc, listProfilesJson());
    JsonArray pArr = pListDoc.as<JsonArray>();
    for (JsonObject p : pArr) {
      String pid = p["id"].as<String>();
      String pname = p["name"].as<String>();
      bool isAct = p["active"].as<bool>();
      profileOpts += "<option value=\"" + htmlEscape(pid) + "\"" + (isAct ? " selected" : "") + ">" + htmlEscape(pname) + "</option>\n";
    }
    html.replace("%PROFILE_OPTIONS%", profileOpts);

    // Dynamic Crypto Status Badge (F-11 Remediation)
    if (hmacAvailable) {
      html.replace("%CRYPTO_STATUS_BADGE%", "<span class=\"badge\">&#x1F512; Hardware eFuse HMAC Active</span>");
    } else {
      html.replace("%CRYPTO_STATUS_BADGE%", "<span class=\"badge\" style=\"background:#ef444430;color:#f87171;border-color:#ef444460;\">&#x26A0; Hardware Encryption Offline</span>");
    }

    // Back Link (hide in unconfigured AP setup mode)
    if (isConfigured) {
      html.replace("%BACK_LINK%", "<a href=\"/\" class=\"back-link\">&#x2190; Back to Remote Control</a>");
    } else {
      html.replace("%BACK_LINK%", "");
    }

    server.send(200, "text/html", html);
  });

  // Save Wi-Fi Credentials Endpoint (Fail-Closed F-04 / Stored XSS F-05 / Bug F-12)
  server.on("/savewifi", HTTP_POST, []() {
    String selectedMode = server.hasArg("op_mode") ? server.arg("op_mode") : "sta";
    String ssid = "";

    if (selectedMode == "ap") {
      ssid = server.hasArg("ap_ssid") && server.arg("ap_ssid").length() > 0 ? server.arg("ap_ssid") : (roomName + "-TV-Remote");
    } else {
      ssid = (server.hasArg("ssid_custom") && server.arg("ssid_custom").length() > 0)
                    ? server.arg("ssid_custom") 
                    : server.arg("ssid");
    }

    String password = server.arg("password");
    String newRoom = server.hasArg("room") && server.arg("room").length() > 0 ? server.arg("room") : DEFAULT_ROOM_NAME;
    String newMdns = server.hasArg("hostname") && server.arg("hostname").length() > 0 ? sanitizeHostname(server.arg("hostname")) : DEFAULT_MDNS_HOSTNAME;
    bool newAuthRequired = server.hasArg("auth_required") && (server.arg("auth_required") == "1");

    if (ssid.length() == 0) {
      server.send(400, "text/plain", "SSID cannot be empty.");
      return;
    }

    // Fail-Closed Cryptography Enforcement (F-04)
    String encPass = encryptCredential(password);
    if (password.length() > 0 && encPass.length() == 0) {
      Serial.println("[SECURITY ERROR] Hardware encryption failed. Aborting commit to flash!");
      tft.fillScreen(TFT_RED);
      tft.setTextColor(TFT_WHITE, TFT_RED);
      tft.drawString("! CRYPT ERROR !", 10, 20, 2);
      tft.drawString("HMAC Failure", 10, 45, 2);
      server.send(500, "text/plain", "Security Error: Hardware HMAC encryption failed. Credentials not saved.");
      return;
    }

    prefs.begin("tvremote", false);
    prefs.putString("op_mode", selectedMode);
    prefs.putString("ssid", ssid);
    prefs.putString("enc_pass", encPass);
    prefs.putString("room", newRoom);
    prefs.putString("mdns", newMdns);
    prefs.putBool("auth_req", newAuthRequired);
    prefs.end();

    String html = FPSTR(PAGE_SAVED);
    html.replace("%MDNS_HOSTNAME%", htmlEscape(newMdns));
    server.send(200, "text/html", html);

    Serial.println("[SETUP] Settings encrypted & saved! Rebooting in 2 seconds...");
    delay(2000);
    ESP.restart();
  });

  // Pairing Request Endpoint (Method B)
  server.on("/api/pair_request", HTTP_POST, []() {
    String devId = server.arg("device");
    if (devId.length() == 0) {
      server.send(400, "application/json", "{\"error\":\"Missing device ID\"}");
      return;
    }

    // Initiate physical pairing confirmation on the dongle
    pairingPending = true;
    pairingPendingDeviceId = devId;
    pairingStartTime = millis();
    pairingApproved = false;
    showPairingPromptScreen();

    Serial.printf("[SECURITY] Pairing requested for device: %s. Awaiting physical button press...\n", devId.c_str());
    server.send(200, "application/json", "{\"status\":\"waiting\"}");
  });

  // Pairing Status Polling Endpoint
  server.on("/api/pair_status", HTTP_GET, []() {
    String devId = server.arg("device");
    if (devId.length() == 0) {
      server.send(400, "application/json", "{\"error\":\"Missing device ID\"}");
      return;
    }

    if (pairingApproved && pairingApprovedDeviceId == devId) {
      pairingApproved = false;
      String token = generateSignedToken(devId);
      server.send(200, "application/json", "{\"status\":\"approved\",\"token\":\"" + token + "\"}");
    } else if (millis() - pairingStartTime > PAIRING_TIMEOUT_MS) {
      server.send(200, "application/json", "{\"status\":\"expired\"}");
    } else {
      server.send(200, "application/json", "{\"status\":\"waiting\"}");
    }
  });

  // ==========================================
  // Profile & Macro API Endpoints (Mandatory Pairing Auth)
  // ==========================================
  server.on("/api/profiles", HTTP_GET, []() {
    server.send(200, "application/json", listProfilesJson());
  });

  server.on("/api/profiles/active", HTTP_GET, []() {
    server.send(200, "application/json", getActiveProfileJson());
  });

  server.on("/api/profiles/set_active", HTTP_POST, []() {
    String token = server.hasArg("token") ? server.arg("token") : server.header("X-Auth-Token");
    if (!verifyDeviceToken(token)) {
      server.send(401, "application/json", "{\"error\":\"Pairing authorization required to manage profiles\"}");
      return;
    }
    String id = server.arg("id");
    if (setActiveProfile(id)) {
      server.send(200, "application/json", "{\"status\":\"ok\",\"active\":\"" + activeProfileId + "\"}");
    } else {
      server.send(404, "application/json", "{\"error\":\"Profile not found\"}");
    }
  });

  server.on("/api/profiles/upload", HTTP_POST, []() {
    String token = server.hasArg("token") ? server.arg("token") : server.header("X-Auth-Token");
    if (!verifyDeviceToken(token)) {
      server.send(401, "application/json", "{\"error\":\"Pairing authorization required to manage profiles\"}");
      return;
    }
    String body = server.arg("plain");
    if (body.length() == 0 && server.hasArg("json")) body = server.arg("json");

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, body);
    if (err) {
      server.send(400, "application/json", "{\"error\":\"Invalid JSON format\"}");
      return;
    }
    String id = doc["id"].as<String>();
    if (id.length() == 0) {
      id = "profile-" + String(millis());
    }
    if (saveProfile(id, body)) {
      if (server.hasArg("set_active") && server.arg("set_active") == "1") {
        setActiveProfile(id);
      }
      server.send(200, "application/json", "{\"status\":\"ok\",\"id\":\"" + id + "\"}");
    } else {
      server.send(500, "application/json", "{\"error\":\"Failed to save profile\"}");
    }
  });

  server.on("/api/profiles/delete", HTTP_POST, []() {
    String token = server.hasArg("token") ? server.arg("token") : server.header("X-Auth-Token");
    if (!verifyDeviceToken(token)) {
      server.send(401, "application/json", "{\"error\":\"Pairing authorization required to manage profiles\"}");
      return;
    }
    String id = server.arg("id");
    if (deleteProfile(id)) {
      server.send(200, "application/json", "{\"status\":\"ok\"}");
    } else {
      server.send(400, "application/json", "{\"error\":\"Cannot delete profile\"}");
    }
  });

  // Macro Execution Endpoint
  server.on("/api/macro/run", HTTP_POST, []() {
    String token = server.hasArg("token") ? server.arg("token") : server.header("X-Auth-Token");
    if (authRequired && !verifyDeviceToken(token)) {
      server.send(401, "application/json", "{\"error\":\"Unauthorized: Valid pairing token required.\"}");
      return;
    }
    String script = server.hasArg("script") ? server.arg("script") : server.arg("plain");
    if (script.startsWith("E:")) script = decryptPayload(script);
    if (script.startsWith("M:")) script = script.substring(2);
    executeDuckyScript(script);
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  // HTTP Fallback Commands (Authenticated & Encrypted)
  server.on("/sendtext", HTTP_POST, []() {
    if (authRequired && !verifyDeviceToken(server.arg("token"))) {
      server.send(401, "text/plain", "Unauthorized: Valid pairing token required.");
      return;
    }
    if (server.hasArg("text")) {
      String text = server.arg("text");
      if (text.startsWith("E:")) text = decryptPayload(text);
      handleTextCommand(text);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing text");
    }
  });

  server.on("/sendkey", HTTP_POST, []() {
    if (authRequired && !verifyDeviceToken(server.arg("token"))) {
      server.send(401, "text/plain", "Unauthorized: Valid pairing token required.");
      return;
    }
    if (server.hasArg("key")) {
      String key = server.arg("key");
      if (key.startsWith("E:")) key = decryptPayload(key);
      if (key.startsWith("K:")) key = key.substring(2);
      handleKeyCommand(key);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing key");
    }
  });

  // ==========================================
  // Over-The-Air (OTA) Firmware Update Endpoints
  // ==========================================
  server.on("/api/ota/config", HTTP_GET, []() {
    JsonDocument doc;
    doc["version"] = FIRMWARE_VERSION;
    doc["test_device"] = testDeviceMode;
    doc["repo"] = GITHUB_REPO;
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
  });

  server.on("/api/ota/set_test_mode", HTTP_POST, []() {
    if (server.hasArg("enabled")) {
      testDeviceMode = (server.arg("enabled") == "1" || server.arg("enabled") == "true");
      prefs.begin("tvremote", false);
      prefs.putBool("test_dev", testDeviceMode);
      prefs.end();
      Serial.printf("[OTA] Test Device Mode: %s\n", testDeviceMode ? "ENABLED" : "DISABLED");
      server.send(200, "application/json", "{\"status\":\"ok\",\"test_device\":" + String(testDeviceMode ? "true" : "false") + "}");
    } else {
      server.send(400, "application/json", "{\"error\":\"Missing enabled parameter\"}");
    }
  });

  server.on("/api/ota/cloud_update", HTTP_POST, []() {
    String token = server.hasArg("token") ? server.arg("token") : server.header("X-Auth-Token");
    if (authRequired && !verifyDeviceToken(token)) {
      server.send(401, "application/json", "{\"error\":\"Pairing authorization required to update firmware\"}");
      return;
    }
    if (!server.hasArg("url")) {
      server.send(400, "application/json", "{\"error\":\"Missing download URL\"}");
      return;
    }
    String url = server.arg("url");
    String ver = server.hasArg("version") ? server.arg("version") : "Update";

    server.send(200, "application/json", "{\"status\":\"started\",\"version\":\"" + ver + "\"}");

    // Perform the cloud update directly from GitHub
    performCloudUpdate(url, ver);
  });

  // Manual Offline Firmware Upload (.bin) Handler
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    if (Update.hasError()) {
      server.send(500, "text/plain", "Update Failed: " + String(Update.errorString()));
    } else {
      server.send(200, "text/html", "<html><head><meta http-equiv='refresh' content='5;url=/'></head><body style='background:#0f172a;color:#fff;font-family:sans-serif;text-align:center;padding:40px;'><h2>Update Successful!</h2><p>Firmware flashed successfully. Rebooting dongle now...</p></body></html>");
      delay(1500);
      ESP.restart();
    }
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("[OTA-Manual] Upload started: %s\n", upload.filename.c_str());
      showOtaProgressScreen("Manual Upload", 0);
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      } else {
        if (upload.totalSize > 0) {
          int pct = (upload.currentSize * 100) / upload.totalSize;
          showOtaProgressScreen("Manual Upload", pct);
        }
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) {
        Serial.printf("[OTA-Manual] Success: %u bytes\n", upload.totalSize);
        showOtaSuccessScreen("Manual Upload");
      } else {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_ABORTED) {
      Update.end();
      Serial.println("[OTA-Manual] Aborted");
      updateScreenContent();
    }
  });
}

