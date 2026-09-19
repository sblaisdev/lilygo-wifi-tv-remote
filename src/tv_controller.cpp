#include "tv_controller.h"
#include <vector>

TvController tvController;

TvController::TvController() 
  : _hmacSlot(HMAC_KEY_MAX), _hmacAvailable(false), _keyDerived(false) {
  memset(_tvKey, 0, sizeof(_tvKey));
}

void TvController::begin(hmac_key_id_t hmacSlot, bool hasHmac) {
  _hmacSlot = hmacSlot;
  _hmacAvailable = hasHmac;
  if (_hmacAvailable && _hmacSlot != HMAC_KEY_MAX) {
    _keyDerived = deriveTvKey();
    if (_keyDerived) {
      Serial.println("[TV] Hardware HMAC key active (domain: " HMAC_CTX_TV_TOKEN ")");
    } else {
      Serial.println("[TV] WARNING: Failed to derive HMAC key for TV credentials!");
    }
  } else {
    Serial.println("[TV] Hardware HMAC offline. Fail-closed credential policy active.");
  }
}

bool TvController::deriveTvKey() {
  if (!_hmacAvailable || _hmacSlot == HMAC_KEY_MAX) return false;
  esp_err_t err = esp_hmac_calculate(_hmacSlot, HMAC_CTX_TV_TOKEN, strlen(HMAC_CTX_TV_TOKEN), _tvKey);
  return (err == ESP_OK);
}

// Encrypt TV token using AES-256 CTR (Fail-closed)
String TvController::encryptToken(const String& plain) {
  if (!_keyDerived || plain.length() == 0) return "";

  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, _tvKey, 256);

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

// Decrypt TV token using AES-256 CTR (Fail-closed)
String TvController::decryptToken(const String& cipherHex) {
  if (!_keyDerived || cipherHex.length() < 34) return "";

  size_t totalBytes = cipherHex.length() / 2;
  if (totalBytes <= 16) return "";

  uint8_t* raw = (uint8_t*)malloc(totalBytes);
  if (!raw) return "";

  for (size_t i = 0; i < totalBytes; i++) {
    char byteString[3] = { cipherHex[i * 2], cipherHex[i * 2 + 1], '\0' };
    raw[i] = (uint8_t)strtol(byteString, NULL, 16);
  }

  uint8_t nonce[16];
  memcpy(nonce, raw, 16);

  size_t cipherLen = totalBytes - 16;
  uint8_t* ciphertext = raw + 16;

  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, _tvKey, 256);

  uint8_t nonce_counter[16];
  memcpy(nonce_counter, nonce, 16);

  size_t nc_off = 0;
  uint8_t stream_block[16];
  memset(stream_block, 0, sizeof(stream_block));

  uint8_t* plainBytes = (uint8_t*)malloc(cipherLen);
  if (!plainBytes) {
    mbedtls_aes_free(&aes);
    free(raw);
    return "";
  }

  mbedtls_aes_crypt_ctr(&aes, cipherLen, &nc_off, nonce_counter, stream_block, ciphertext, plainBytes);
  mbedtls_aes_free(&aes);

  String plain = "";
  plain.reserve(cipherLen);
  for (size_t i = 0; i < cipherLen; i++) {
    plain += (char)plainBytes[i];
  }

  free(plainBytes);
  free(raw);
  return plain;
}

static String sanitizeNvsKey(const String& key) {
  if (key.length() <= 15) return key;
  uint32_t h = 5381;
  for (size_t i = 0; i < key.length(); i++) {
    h = ((h << 5) + h) + (uint8_t)key[i];
  }
  char buf[16];
  snprintf(buf, sizeof(buf), "tv_%08x", (unsigned int)h);
  return String(buf);
}

bool TvController::saveTvToken(const String& deviceId, const String& token) {
  if (deviceId.length() == 0 || token.length() == 0) return false;
  String cipher = encryptToken(token);
  if (cipher.length() == 0) return false;

  String safeKey = sanitizeNvsKey(deviceId);
  Preferences p;
  if (!p.begin(NVS_TV_NAMESPACE, false)) return false;
  size_t written = p.putString(safeKey.c_str(), cipher);
  p.end();
  return (written > 0);
}

String TvController::getTvToken(const String& deviceId) {
  if (deviceId.length() == 0) return "";
  String safeKey = sanitizeNvsKey(deviceId);
  Preferences p;
  if (!p.begin(NVS_TV_NAMESPACE, true)) return "";
  String cipher = p.getString(safeKey.c_str(), "");
  p.end();
  if (cipher.length() == 0) return "";
  return decryptToken(cipher);
}

bool TvController::hasTvToken(const String& deviceId) {
  String safeKey = sanitizeNvsKey(deviceId);
  Preferences p;
  if (!p.begin(NVS_TV_NAMESPACE, true)) return false;
  bool exists = p.isKey(safeKey.c_str());
  p.end();
  return exists;
}

bool TvController::deleteTvToken(const String& deviceId) {
  String safeKey = sanitizeNvsKey(deviceId);
  Preferences p;
  if (!p.begin(NVS_TV_NAMESPACE, false)) return false;
  bool ok = p.remove(safeKey.c_str());
  p.end();
  return ok;
}

// Generic SSDP Discovery (Standard UPnP M-SEARCH on UDP 1900)
String TvController::discoverTvsJson() {
  std::vector<DiscoveredDevice> found;
  WiFiUDP udp;
  
  if (!udp.begin(0)) {
    Serial.println("[TV] Failed to bind local UDP port for SSDP discovery");
    return "[]";
  }

  IPAddress multicastIp;
  multicastIp.fromString(TV_SSDP_MULTICAST_IP);

  // Send standard UPnP discovery broadcast
  const char* msearch = 
    "M-SEARCH * HTTP/1.1\r\n"
    "HOST: 239.255.255.250:1900\r\n"
    "MAN: \"ssdp:discover\"\r\n"
    "MX: 2\r\n"
    "ST: ssdp:all\r\n\r\n";

  udp.beginPacket(multicastIp, TV_SSDP_PORT);
  udp.write((const uint8_t*)msearch, strlen(msearch));
  udp.endPacket();

  unsigned long startTime = millis();
  char packetBuffer[1024];

  while (millis() - startTime < TV_DISCOVERY_TIMEOUT_MS) {
    int packetSize = udp.parsePacket();
    if (packetSize > 0) {
      int len = udp.read(packetBuffer, sizeof(packetBuffer) - 1);
      if (len > 0) {
        packetBuffer[len] = 0;
        String resp = String(packetBuffer);

        IPAddress remoteIp = udp.remoteIP();
        String ipStr = remoteIp.toString();

        bool alreadyAdded = false;
        for (const auto& dev : found) {
          if (dev.ip == ipStr) {
            alreadyAdded = true;
            break;
          }
        }

        if (!alreadyAdded) {
          DiscoveredDevice dev;
          dev.ip = ipStr;
          dev.port = 0;
          dev.id = "dev-" + ipStr;

          // Extract LOCATION header
          int locIdx = resp.indexOf("LOCATION: ");
          if (locIdx < 0) locIdx = resp.indexOf("location: ");
          if (locIdx >= 0) {
            int endLoc = resp.indexOf("\r\n", locIdx);
            if (endLoc > locIdx) {
              dev.location = resp.substring(locIdx + 10, endLoc);
              dev.location.trim();
            }
          }

          // Extract SERVER header
          int srvIdx = resp.indexOf("SERVER: ");
          if (srvIdx < 0) srvIdx = resp.indexOf("server: ");
          if (srvIdx >= 0) {
            int endSrv = resp.indexOf("\r\n", srvIdx);
            if (endSrv > srvIdx) {
              dev.server = resp.substring(srvIdx + 8, endSrv);
              dev.server.trim();
            }
          }

          dev.name = dev.server.length() > 0 ? dev.server : ("Device (" + ipStr + ")");
          found.push_back(dev);
        }
      }
    }
    delay(10);
  }
  udp.stop();

  // Serialize to JSON
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();

  for (const auto& dev : found) {
    JsonObject obj = arr.add<JsonObject>();
    obj["id"] = dev.id;
    obj["name"] = dev.name;
    obj["ip"] = dev.ip;
    obj["port"] = dev.port;
    obj["location"] = dev.location;
    obj["server"] = dev.server;
  }

  String result;
  serializeJson(doc, result);
  return result;
}

// Generic Device Info Prober (DIAL / UPnP HTTP query)
String TvController::probeDevice(const String& ip, uint16_t port, const String& path) {
  if (ip.length() == 0) return "{}";
  HTTPClient http;
  http.setTimeout(2500);

  String url = "http://" + ip + ":" + String(port > 0 ? port : 80) + path;
  http.begin(url);
  int code = http.GET();

  String payload = "{}";
  if (code >= 200 && code < 400) {
    payload = http.getString();
  }
  http.end();
  return payload;
}

// =========================================================================
// Pure Generic Network Transport Dispatchers (Zero Vendor Code!)
// =========================================================================

// Generic HTTP Dispatcher (GET, POST, PUT, DELETE)
bool TvController::sendGenericHttp(const String& method, const String& url, const String& headersJson, const String& body) {
  if (url.length() == 0) return false;
  HTTPClient http;
  http.setTimeout(2500);
  http.begin(url);

  // Set custom headers if provided
  if (headersJson.length() > 2) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, headersJson);
    if (!err && doc.is<JsonObject>()) {
      JsonObject obj = doc.as<JsonObject>();
      for (JsonPair kv : obj) {
        http.addHeader(kv.key().c_str(), kv.value().as<String>());
      }
    }
  }

  int httpCode = -1;
  String m = method;
  m.toUpperCase();

  if (m == "POST") {
    httpCode = http.POST(body);
  } else if (m == "GET") {
    httpCode = http.GET();
  } else if (m == "PUT") {
    httpCode = http.PUT(body);
  } else if (m == "DELETE") {
    httpCode = http.sendRequest("DELETE", (uint8_t*)body.c_str(), body.length());
  } else {
    httpCode = http.POST(body);
  }

  http.end();
  return (httpCode >= 200 && httpCode < 400);
}

// Generic WebSocket Frame Dispatcher
bool TvController::sendGenericWs(const String& url, const String& payload) {
  if (url.length() == 0) return false;

  // Parse ws://host:port/path
  String cleanUrl = url;
  bool isWss = false;
  if (cleanUrl.startsWith("wss://")) {
    isWss = true;
    cleanUrl = cleanUrl.substring(6);
  } else if (cleanUrl.startsWith("ws://")) {
    cleanUrl = cleanUrl.substring(5);
  }

  int slashIdx = cleanUrl.indexOf('/');
  String hostPort = (slashIdx >= 0) ? cleanUrl.substring(0, slashIdx) : cleanUrl;
  String path = (slashIdx >= 0) ? cleanUrl.substring(slashIdx) : "/";

  String host = hostPort;
  uint16_t port = isWss ? 443 : 80;
  int colonIdx = hostPort.indexOf(':');
  if (colonIdx >= 0) {
    host = hostPort.substring(0, colonIdx);
    port = hostPort.substring(colonIdx + 1).toInt();
  }

  if (port == 8002) isWss = true;
  // Safety correction: if port was unspecified or default HTTP (80) on a Samsung WS endpoint
  if ((port == 80 || port == 0) && path.indexOf("samsung") >= 0) {
    port = 8002;
    isWss = true;
  }

  struct WsPortCandidate {
    bool ssl;
    uint16_t p;
  };
  std::vector<WsPortCandidate> candidates;
  candidates.push_back({ isWss, port });
  if (path.indexOf("samsung") >= 0) {
    if (port == 8002) candidates.push_back({ false, 8001 });
    else if (port == 8001) candidates.push_back({ true, 8002 });
  }

  for (const auto& cand : candidates) {
    WebSocketsClient client;
    bool messageSent = false;
    bool connected = false;

    client.onEvent([&](WStype_t type, uint8_t * pl, size_t length) {
      if (type == WStype_CONNECTED) {
        connected = true;
        if (payload.length() > 0) {
          client.sendTXT((uint8_t*)payload.c_str(), payload.length());
          messageSent = true;
        }
      }
    });

    if (cand.ssl) {
      client.beginSSL(host.c_str(), cand.p, path.c_str());
    } else {
      client.begin(host.c_str(), cand.p, path.c_str());
    }

    unsigned long start = millis();
    while (millis() - start < 1500) {
      client.loop();
      if (messageSent) {
        delay(40);
        break;
      }
      delay(10);
    }

    client.disconnect();
    if (messageSent || connected) return true;
  }

  return false;
}

// Generic WebSocket Pair / Token Handshake Listener
String TvController::listenForWsToken(const String& url, const String& handshakePayload, uint32_t timeoutMs) {
  if (url.length() == 0) return "";

  String cleanUrl = url;
  bool isWss = false;
  if (cleanUrl.startsWith("wss://")) {
    isWss = true;
    cleanUrl = cleanUrl.substring(6);
  } else if (cleanUrl.startsWith("ws://")) {
    cleanUrl = cleanUrl.substring(5);
  }

  int slashIdx = cleanUrl.indexOf('/');
  String hostPort = (slashIdx >= 0) ? cleanUrl.substring(0, slashIdx) : cleanUrl;
  String path = (slashIdx >= 0) ? cleanUrl.substring(slashIdx) : "/";

  String host = hostPort;
  uint16_t port = isWss ? 443 : 80;
  int colonIdx = hostPort.indexOf(':');
  if (colonIdx >= 0) {
    host = hostPort.substring(0, colonIdx);
    port = hostPort.substring(colonIdx + 1).toInt();
  }

  if (port == 8002) isWss = true;
  // Safety correction: if port was unspecified or default HTTP (80) on a Samsung WS endpoint
  if ((port == 80 || port == 0) && path.indexOf("samsung") >= 0) {
    port = 8002;
    isWss = true;
  }

  struct WsCandidate {
    bool ssl;
    uint16_t port;
  };
  std::vector<WsCandidate> candidates;

  // For Samsung TVs, modern Tizen (2016-2025) requires WSS on 8002 for pairing
  if (path.indexOf("samsung") >= 0) {
    candidates.push_back({ true, 8002 });
    candidates.push_back({ false, 8001 });
  } else {
    candidates.push_back({ isWss || port == 8002, port });
  }

  String extractedToken = "";
  unsigned long overallStart = millis();

  for (size_t cIdx = 0; cIdx < candidates.size(); cIdx++) {
    bool curSsl = candidates[cIdx].ssl;
    uint16_t curPort = candidates[cIdx].port;
    Serial.printf("[TV] Initiating WS handshake to %s:%u (SSL: %s)\n", host.c_str(), curPort, curSsl ? "YES" : "NO");

    WebSocketsClient client;
    bool connected = false;
    bool finished = false;

    client.onEvent([&](WStype_t type, uint8_t * pl, size_t length) {
      if (type == WStype_CONNECTED) {
        connected = true;
        Serial.printf("[TV] WS Connected to %s:%u\n", host.c_str(), curPort);
        if (handshakePayload.length() > 0) {
          client.sendTXT((uint8_t*)handshakePayload.c_str(), handshakePayload.length());
        }
      } else if (type == WStype_TEXT && length > 0) {
        String msg = String((char*)pl).substring(0, length);
        Serial.println("[TV] WS Frame: " + msg);

        if (msg.indexOf("ms.channel.unauthorized") >= 0) {
          Serial.println("[TV] WS Unauthorized on this port, failing over to next candidate immediately.");
          finished = true;
          return;
        }

        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, msg);
        if (!err) {
          if (doc["data"]["token"].is<String>()) {
            extractedToken = doc["data"]["token"].as<String>();
            finished = true;
          } else if (doc["payload"]["client-key"].is<String>()) {
            extractedToken = doc["payload"]["client-key"].as<String>();
            finished = true;
          } else if (doc["token"].is<String>()) {
            extractedToken = doc["token"].as<String>();
            finished = true;
          } else if (doc["key"].is<String>()) {
            extractedToken = doc["key"].as<String>();
            finished = true;
          }
        } else {
          int tIdx = msg.indexOf("\"token\":\"");
          if (tIdx >= 0) {
            int endQ = msg.indexOf("\"", tIdx + 9);
            if (endQ > tIdx) {
              extractedToken = msg.substring(tIdx + 9, endQ);
              finished = true;
            }
          }
        }
      }
    });

    if (curSsl) {
      client.beginSSL(host.c_str(), curPort, path.c_str());
    } else {
      client.begin(host.c_str(), curPort, path.c_str());
    }

    unsigned long candStart = millis();
    uint32_t waitLimit = (candidates.size() > 1 && cIdx == 0 && curPort == 8001) ? 3000 : (timeoutMs - (millis() - overallStart));

    while (millis() - candStart < waitLimit && millis() - overallStart < timeoutMs && !finished) {
      client.loop();
      if (connected && curPort != 8001) {
        // Connected on pairing channel! Give user full time to click Allow on screen
        waitLimit = timeoutMs - (millis() - overallStart);
      }
      delay(20);
    }

    client.disconnect();

    if (finished && extractedToken.length() > 0) {
      Serial.println("[TV] Token received successfully: " + extractedToken);
      break;
    }
  }

  return extractedToken;
}

// Generic Wake-On-LAN
bool TvController::sendWakeOnLan(const String& macAddress) {
  if (macAddress.length() < 12) return false;

  uint8_t mac[6];
  int parsed = sscanf(macAddress.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                      &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
  if (parsed != 6) {
    parsed = sscanf(macAddress.c_str(), "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
                    &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    if (parsed != 6) return false;
  }

  uint8_t packet[102];
  memset(packet, 0xFF, 6);
  for (int i = 1; i <= 16; i++) {
    memcpy(packet + (i * 6), mac, 6);
  }

  WiFiUDP udp;
  if (!udp.begin(0)) return false;
  udp.beginPacket(IPAddress(255, 255, 255, 255), TV_WOL_PORT);
  udp.write(packet, sizeof(packet));
  bool ok = udp.endPacket();
  udp.stop();
  return ok;
}

// Universal Network Action Dispatcher (Executes purely based on config parameters)
bool TvController::executeNetworkAction(const String& transport, const String& url, const String& method, const String& headersJson, const String& payload) {
  String t = transport;
  t.toLowerCase();

  if (t == "http" || t == "rest") {
    return sendGenericHttp(method, url, headersJson, payload);
  } else if (t == "ws" || t == "websocket") {
    return sendGenericWs(url, payload);
  } else if (t == "wol" || t == "wakeonlan") {
    return sendWakeOnLan(payload.length() > 0 ? payload : url);
  }

  Serial.println("[TV] Unknown generic transport: " + transport);
  return false;
}
