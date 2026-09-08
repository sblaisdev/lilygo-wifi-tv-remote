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
#include "webpage.h"

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

    if (!authRequired) {
      // Open mode: immediately issue signed token
      String token = generateSignedToken(devId);
      server.send(200, "application/json", "{\"status\":\"approved\",\"token\":\"" + token + "\"}");
      return;
    }

    // Restricted mode: initiate physical pairing confirmation
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

    if (!authRequired) {
      String token = generateSignedToken(devId);
      server.send(200, "application/json", "{\"status\":\"approved\",\"token\":\"" + token + "\"}");
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
}
