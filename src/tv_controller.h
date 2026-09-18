#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "esp_efuse.h"
#include "soc/esp32s3/esp_hmac.h"
#include "esp_random.h"
#include "mbedtls/aes.h"
#include <Preferences.h>
#include "../include/config.h"

struct DiscoveredTv {
  String id;
  String name;
  String brand;
  String protocol;
  String ip;
  uint16_t port;
  String mac;
};

class TvController {
public:
  TvController();
  void begin(hmac_key_id_t hmacSlot, bool hasHmac);

  // Network Discovery (SSDP M-SEARCH on UDP 1900)
  String discoverTvsJson();

  // TV Pairing & Credentials (HMAC-derived AES-256 CTR in NVS)
  bool saveTvToken(const String& tvId, const String& token);
  String getTvToken(const String& tvId);
  bool hasTvToken(const String& tvId);

  // Command Execution
  bool sendTvCommand(const String& protocol, const String& ip, uint16_t port, const String& token, const String& command, const String& params = "");
  bool sendWakeOnLan(const String& macAddress);

  // Protocol-Specific Dispatchers
  bool sendRokuCommand(const String& ip, uint16_t port, const String& cmd, const String& params);
  bool sendLgCommand(const String& ip, uint16_t port, const String& token, const String& cmd, const String& params);
  bool sendSamsungCommand(const String& ip, uint16_t port, const String& token, const String& cmd, const String& params);
  bool sendSonyCommand(const String& ip, uint16_t port, const String& token, const String& cmd, const String& params);

  // Crypto helpers for tokens (Fail-Closed)
  String encryptToken(const String& plain);
  String decryptToken(const String& cipherHex);

private:
  hmac_key_id_t _hmacSlot;
  bool _hmacAvailable;
  uint8_t _tvKey[32];
  bool _keyDerived;

  bool deriveTvKey();
};

extern TvController tvController;
