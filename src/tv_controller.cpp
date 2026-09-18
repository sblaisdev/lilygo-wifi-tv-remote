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
      Serial.println("[TV] Hardware HMAC key derived for TV credentials (domain: " HMAC_CTX_TV_TOKEN ")");
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

bool TvController::saveTvToken(const String& tvId, const String& token) {
  if (tvId.length() == 0 || token.length() == 0) return false;
  String cipher = encryptToken(token);
  if (cipher.length() == 0) return false;

  Preferences p;
  if (!p.begin(NVS_TV_NAMESPACE, false)) return false;
  size_t written = p.putString(tvId.c_str(), cipher);
  p.end();
  return (written > 0);
}

String TvController::getTvToken(const String& tvId) {
  if (tvId.length() == 0) return "";
  Preferences p;
  if (!p.begin(NVS_TV_NAMESPACE, true)) return "";
  String cipher = p.getString(tvId.c_str(), "");
  p.end();
  if (cipher.length() == 0) return "";
  return decryptToken(cipher);
}

bool TvController::hasTvToken(const String& tvId) {
  Preferences p;
  if (!p.begin(NVS_TV_NAMESPACE, true)) return false;
  bool exists = p.isKey(tvId.c_str());
  p.end();
  return exists;
}

// SSDP Discovery implementation
String TvController::discoverTvsJson() {
  std::vector<DiscoveredTv> found;
  WiFiUDP udp;
  
  if (!udp.begin(0)) {
    Serial.println("[TV] Failed to bind local UDP port for SSDP discovery");
    return "[]";
  }

  IPAddress multicastIp;
  multicastIp.fromString(TV_SSDP_MULTICAST_IP);

  // Send SSDP M-SEARCH query
  const char* msearch = 
    "M-SEARCH * HTTP/1.1\r\n"
    "HOST: 239.255.255.250:1900\r\n"
    "MAN: \"ssdp:discover\"\r\n"
    "MX: 2\r\n"
    "ST: ssdp:all\r\n\r\n";

  udp.beginPacket(multicastIp, TV_SSDP_PORT);
  udp.write((const uint8_t*)msearch, strlen(msearch));
  udp.endPacket();

  // Also send targeted search for Roku ECP
  const char* msearchRoku = 
    "M-SEARCH * HTTP/1.1\r\n"
    "HOST: 239.255.255.250:1900\r\n"
    "MAN: \"ssdp:discover\"\r\n"
    "MX: 2\r\n"
    "ST: roku:ecp\r\n\r\n";

  udp.beginPacket(multicastIp, TV_SSDP_PORT);
  udp.write((const uint8_t*)msearchRoku, strlen(msearchRoku));
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
        resp.toLowerCase();

        IPAddress remoteIp = udp.remoteIP();
        String ipStr = remoteIp.toString();

        // Check if device is already registered
        bool alreadyAdded = false;
        for (const auto& dev : found) {
          if (dev.ip == ipStr) {
            alreadyAdded = true;
            break;
          }
        }

        if (!alreadyAdded) {
          DiscoveredTv dev;
          dev.ip = ipStr;
          dev.port = 80;

          if (resp.indexOf("roku:ecp") >= 0 || resp.indexOf("roku") >= 0) {
            dev.brand = "roku";
            dev.protocol = "roku";
            dev.port = 8060;
            dev.name = "Roku Smart TV / Player (" + ipStr + ")";
            dev.id = "roku-" + ipStr;
            found.push_back(dev);
          } else if (resp.indexOf("webos") >= 0 || resp.indexOf("lge-com") >= 0 || resp.indexOf("lg ") >= 0) {
            dev.brand = "lg";
            dev.protocol = "lg_webos";
            dev.port = 3000;
            dev.name = "LG webOS Smart TV (" + ipStr + ")";
            dev.id = "lg-" + ipStr;
            found.push_back(dev);
          } else if (resp.indexOf("samsung") >= 0 || resp.indexOf("tizen") >= 0 || resp.indexOf("dial-multiscreen") >= 0) {
            dev.brand = "samsung";
            dev.protocol = "samsung_tizen";
            dev.port = 8001;
            dev.name = "Samsung Tizen TV (" + ipStr + ")";
            dev.id = "samsung-" + ipStr;
            found.push_back(dev);
          } else if (resp.indexOf("sony") >= 0 || resp.indexOf("ircc") >= 0 || resp.indexOf("bravia") >= 0) {
            dev.brand = "sony";
            dev.protocol = "sony_bravia";
            dev.port = 80;
            dev.name = "Sony Bravia TV (" + ipStr + ")";
            dev.id = "sony-" + ipStr;
            found.push_back(dev);
          }
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
    obj["brand"] = dev.brand;
    obj["protocol"] = dev.protocol;
    obj["ip"] = dev.ip;
    obj["port"] = dev.port;
  }

  String result;
  serializeJson(doc, result);
  return result;
}

// Wake-On-LAN packet transmission
bool TvController::sendWakeOnLan(const String& macAddress) {
  if (macAddress.length() < 12) return false;

  uint8_t mac[6];
  int parsed = sscanf(macAddress.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                      &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
  if (parsed != 6) {
    // Try without colons
    parsed = sscanf(macAddress.c_str(), "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
                    &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    if (parsed != 6) return false;
  }

  uint8_t packet[102];
  // 6 bytes of 0xFF
  memset(packet, 0xFF, 6);
  // 16 repetitions of the target MAC address
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

// Roku External Control Protocol (ECP)
bool TvController::sendRokuCommand(const String& ip, uint16_t port, const String& cmd, const String& params) {
  if (ip.length() == 0) return false;
  HTTPClient http;
  http.setTimeout(2500);

  String url = "http://" + ip + ":" + String(port > 0 ? port : 8060);

  if (cmd.startsWith("LAUNCH_") || cmd.equalsIgnoreCase("launch")) {
    String appId = params.length() > 0 ? params : cmd.substring(7);
    url += "/launch/" + appId;
  } else {
    // Standard keypress mapping
    String key = cmd;
    if (cmd.equalsIgnoreCase("POWER") || cmd.equalsIgnoreCase("POWER_TOGGLE")) key = "Power";
    else if (cmd.equalsIgnoreCase("VOL_UP") || cmd.equalsIgnoreCase("VOLUME_UP")) key = "VolumeUp";
    else if (cmd.equalsIgnoreCase("VOL_DOWN") || cmd.equalsIgnoreCase("VOLUME_DOWN")) key = "VolumeDown";
    else if (cmd.equalsIgnoreCase("MUTE")) key = "VolumeMute";
    else if (cmd.equalsIgnoreCase("UP")) key = "Up";
    else if (cmd.equalsIgnoreCase("DOWN")) key = "Down";
    else if (cmd.equalsIgnoreCase("LEFT")) key = "Left";
    else if (cmd.equalsIgnoreCase("RIGHT")) key = "Right";
    else if (cmd.equalsIgnoreCase("OK") || cmd.equalsIgnoreCase("SELECT")) key = "Select";
    else if (cmd.equalsIgnoreCase("BACK")) key = "Back";
    else if (cmd.equalsIgnoreCase("HOME")) key = "Home";
    else if (cmd.equalsIgnoreCase("PLAY") || cmd.equalsIgnoreCase("PLAY_PAUSE")) key = "Play";
    else if (cmd.equalsIgnoreCase("FAST_FORWARD")) key = "Fwd";
    else if (cmd.equalsIgnoreCase("REWIND")) key = "Rev";
    else if (cmd.equalsIgnoreCase("INFO")) key = "Info";

    url += "/keypress/" + key;
  }

  http.begin(url);
  int httpCode = http.POST("");
  http.end();

  return (httpCode >= 200 && httpCode < 300);
}

// Sony Bravia IRCC-IP Protocol
bool TvController::sendSonyCommand(const String& ip, uint16_t port, const String& token, const String& cmd, const String& params) {
  if (ip.length() == 0) return false;
  HTTPClient http;
  http.setTimeout(2500);

  String url = "http://" + ip + ":" + String(port > 0 ? port : 80) + "/sony/IRCC";
  http.begin(url);
  http.addHeader("Content-Type", "text/xml; charset=UTF-8");
  http.addHeader("SOAPACTION", "\"urn:schemas-sony-com:service:IRCC:1#X_SendIRCC\"");

  if (token.length() > 0) {
    http.addHeader("X-Auth-PSK", token);
  }

  // IRCC code resolution
  String irccCode = params;
  if (irccCode.length() == 0) {
    if (cmd.equalsIgnoreCase("POWER") || cmd.equalsIgnoreCase("POWER_TOGGLE")) irccCode = "AAAAAQAAAAEAAAAVAw==";
    else if (cmd.equalsIgnoreCase("VOL_UP") || cmd.equalsIgnoreCase("VOLUME_UP")) irccCode = "AAAAAQAAAAEAAAASAw==";
    else if (cmd.equalsIgnoreCase("VOL_DOWN") || cmd.equalsIgnoreCase("VOLUME_DOWN")) irccCode = "AAAAAQAAAAEAAAATAw==";
    else if (cmd.equalsIgnoreCase("MUTE")) irccCode = "AAAAAQAAAAEAAAAUAw==";
    else if (cmd.equalsIgnoreCase("UP")) irccCode = "AAAAAQAAAAEAAAB0Aw==";
    else if (cmd.equalsIgnoreCase("DOWN")) irccCode = "AAAAAQAAAAEAAAB1Aw==";
    else if (cmd.equalsIgnoreCase("LEFT")) irccCode = "AAAAAQAAAAEAAAA0Aw==";
    else if (cmd.equalsIgnoreCase("RIGHT")) irccCode = "AAAAAQAAAAEAAAAzAw==";
    else if (cmd.equalsIgnoreCase("OK") || cmd.equalsIgnoreCase("SELECT")) irccCode = "AAAAAQAAAAEAAABlAw==";
    else if (cmd.equalsIgnoreCase("HOME")) irccCode = "AAAAAQAAAAEAAABgAw==";
    else if (cmd.equalsIgnoreCase("BACK")) irccCode = "AAAAAQAAAAEAAABjAw==";
    else if (cmd.equalsIgnoreCase("PLAY")) irccCode = "AAAAAgAAAJcAAAAaAw==";
    else if (cmd.equalsIgnoreCase("PAUSE")) irccCode = "AAAAAgAAAJcAAAAZAw==";
    else if (cmd.equalsIgnoreCase("HDMI_1")) irccCode = "AAAAAgAAABoAAABaAw==";
    else if (cmd.equalsIgnoreCase("HDMI_2")) irccCode = "AAAAAgAAABoAAABbAw==";
    else if (cmd.equalsIgnoreCase("HDMI_3")) irccCode = "AAAAAgAAABoAAABcAw==";
    else if (cmd.equalsIgnoreCase("HDMI_4")) irccCode = "AAAAAgAAABoAAABdAw==";
  }

  if (irccCode.length() == 0) {
    http.end();
    return false;
  }

  String soapBody = 
    "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
    "<s:Body>"
    "<u:X_SendIRCC xmlns:u=\"urn:schemas-sony-com:service:IRCC:1\">"
    "<IRCCCode>" + irccCode + "</IRCCCode>"
    "</u:X_SendIRCC>"
    "</s:Body>"
    "</s:Envelope>";

  int httpCode = http.POST(soapBody);
  http.end();

  return (httpCode == 200);
}

// LG webOS second-screen REST command dispatcher
bool TvController::sendLgCommand(const String& ip, uint16_t port, const String& token, const String& cmd, const String& params) {
  if (ip.length() == 0) return false;
  HTTPClient http;
  http.setTimeout(2500);

  uint16_t targetPort = port > 0 ? port : 3000;
  String url = "http://" + ip + ":" + String(targetPort) + "/roap/api/command";
  http.begin(url);
  http.addHeader("Content-Type", "application/atom+xml");

  // Map command to LG RoAP keycode if applicable
  String code = "1";
  if (cmd.equalsIgnoreCase("POWER")) code = "1";
  else if (cmd.equalsIgnoreCase("UP")) code = "12";
  else if (cmd.equalsIgnoreCase("DOWN")) code = "13";
  else if (cmd.equalsIgnoreCase("LEFT")) code = "14";
  else if (cmd.equalsIgnoreCase("RIGHT")) code = "15";
  else if (cmd.equalsIgnoreCase("OK")) code = "20";
  else if (cmd.equalsIgnoreCase("HOME")) code = "21";
  else if (cmd.equalsIgnoreCase("BACK")) code = "23";
  else if (cmd.equalsIgnoreCase("VOL_UP")) code = "24";
  else if (cmd.equalsIgnoreCase("VOL_DOWN")) code = "25";
  else if (cmd.equalsIgnoreCase("MUTE")) code = "26";
  else if (cmd.equalsIgnoreCase("PLAY")) code = "33";
  else if (cmd.equalsIgnoreCase("PAUSE")) code = "34";

  String xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><command><name>HandleKeyInput</name><value>" + code + "</value></command>";
  int httpCode = http.POST(xml);
  http.end();
  return (httpCode >= 200 && httpCode < 300);
}

// Samsung Tizen remote HTTP/REST dispatcher
bool TvController::sendSamsungCommand(const String& ip, uint16_t port, const String& token, const String& cmd, const String& params) {
  if (ip.length() == 0) return false;
  HTTPClient http;
  http.setTimeout(2500);

  uint16_t targetPort = port > 0 ? port : 8001;
  String url = "http://" + ip + ":" + String(targetPort) + "/api/v2/";
  http.begin(url);
  int httpCode = http.GET();
  http.end();
  return (httpCode >= 200 && httpCode < 400);
}

// Unified TV Command Dispatcher
bool TvController::sendTvCommand(const String& protocol, const String& ip, uint16_t port, const String& token, const String& command, const String& params) {
  if (command.equalsIgnoreCase("WOL") || command.equalsIgnoreCase("WAKE_ON_LAN")) {
    return sendWakeOnLan(params.length() > 0 ? params : token);
  }

  if (protocol.equalsIgnoreCase("roku")) {
    return sendRokuCommand(ip, port, command, params);
  } else if (protocol.equalsIgnoreCase("sony") || protocol.equalsIgnoreCase("sony_bravia")) {
    return sendSonyCommand(ip, port, token, command, params);
  } else if (protocol.equalsIgnoreCase("lg") || protocol.equalsIgnoreCase("lg_webos")) {
    return sendLgCommand(ip, port, token, command, params);
  } else if (protocol.equalsIgnoreCase("samsung") || protocol.equalsIgnoreCase("samsung_tizen")) {
    return sendSamsungCommand(ip, port, token, command, params);
  }

  Serial.println("[TV] Unknown TV protocol: " + protocol);
  return false;
}
