#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "esp_efuse.h"
#include "soc/esp32s3/esp_hmac.h"
#include "esp_random.h"
#include "mbedtls/aes.h"
#include <Preferences.h>
#include "../include/config.h"

struct DiscoveredDevice {
  String id;
  String name;
  String ip;
  uint16_t port;
  String location;
  String server;
};

class TvController {
public:
  TvController();
  void begin(hmac_key_id_t hmacSlot, bool hasHmac);

  // Generic SSDP/UPnP Network Discovery (UDP 1900)
  String discoverTvsJson();

  // Generic Device Prober (queries UPnP/DIAL info endpoint directly)
  String probeDevice(const String& ip, uint16_t port, const String& path);

  // Generic Credential Persistence (Hardware HMAC AES-256 CTR in NVS)
  bool saveTvToken(const String& deviceId, const String& token);
  String getTvToken(const String& deviceId);
  bool hasTvToken(const String& deviceId);

  // Pure Generic Transport Dispatchers (Zero Vendor Code!)
  bool executeNetworkAction(const String& transport, const String& url, const String& method, const String& headersJson, const String& payload);
  bool sendGenericHttp(const String& method, const String& url, const String& headersJson, const String& body);
  bool sendGenericWs(const String& url, const String& payload);
  String listenForWsToken(const String& url, const String& handshakePayload, uint32_t timeoutMs);
  bool sendWakeOnLan(const String& macAddress);

  // Crypto helpers (Fail-Closed)
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
