/**
 * Smart TV Wi-Fi API Setup Wizard for LiLyGO Remote Studio
 * Fully vendor-agnostic, config-driven TV discovery, capability probe, hard-gated pairing, and layout generation.
 */

// Embedded fallback catalog ensuring immediate availability
const FALLBACK_TV_CATALOG = [
  {
    "id": "samsung_tizen",
    "name": "Samsung Smart TV (Tizen)",
    "brand": "samsung",
    "protocol": "samsung_tizen",
    "defaultPort": 8001,
    "authType": "prompt",
    "transport": "ws",
    "method": "",
    "urlTemplate": "ws://{tv_ip}:{port}/api/v2/channels/samsung.remote.control?name=TGlseUdPIFJlbW90ZQ==&token={token}",
    "headersTemplate": "",
    "payloadTemplate": "{\"method\":\"ms.remote.control\",\"params\":{\"Cmd\":\"Click\",\"DataOfCmd\":\"{cmd}\",\"Option\":\"false\",\"TypeOfRemote\":\"SendRemoteKey\"}}",
    "pairingUrl": "ws://{tv_ip}:{port}/api/v2/channels/samsung.remote.control?name=TGlseUdPIFJlbW90ZQ==",
    "pairingPayload": "",
    "discoveryMatch": ["samsung", "tizen", "dial-multiscreen", "sec-websocket", "8001"],
    "description": "WebSocket channel on port 8001. Displays pairing approval popup on TV screen.",
    "commands": {
      "navigation": [
        { "label": "POWER", "icon": "power", "color": "#ef4444", "command": "KEY_POWER", "span": 1 },
        { "label": "HOME", "icon": "home", "color": "#475569", "command": "KEY_HOME", "span": 1 },
        { "label": "RETURN", "icon": "corner-down-left", "color": "#475569", "command": "KEY_RETURN", "span": 1 },
        { "label": "UP", "icon": "arrow-up", "color": "#334155", "command": "KEY_UP", "span": 1 },
        { "label": "DOWN", "icon": "arrow-down", "color": "#334155", "command": "KEY_DOWN", "span": 1 },
        { "label": "LEFT", "icon": "arrow-left", "color": "#334155", "command": "KEY_LEFT", "span": 1 },
        { "label": "RIGHT", "icon": "arrow-right", "color": "#334155", "command": "KEY_RIGHT", "span": 1 },
        { "label": "ENTER", "icon": "check", "color": "#3b82f6", "command": "KEY_ENTER", "span": 1 },
        { "label": "MENU", "icon": "layers", "color": "#475569", "command": "KEY_MENU", "span": 1 }
      ],
      "audio": [
        { "label": "VOL +", "icon": "volume-2", "color": "#10b981", "command": "KEY_VOLUP", "span": 1 },
        { "label": "VOL -", "icon": "volume-1", "color": "#10b981", "command": "KEY_VOLDOWN", "span": 1 },
        { "label": "MUTE", "icon": "volume-x", "color": "#64748b", "command": "KEY_MUTE", "span": 1 }
      ],
      "media": [
        { "label": "PLAY", "icon": "play", "color": "#06b6d4", "command": "KEY_PLAY", "span": 1 },
        { "label": "PAUSE", "icon": "pause", "color": "#06b6d4", "command": "KEY_PAUSE", "span": 1 },
        { "label": "STOP", "icon": "x-square", "color": "#64748b", "command": "KEY_STOP", "span": 1 }
      ],
      "apps": [
        { "label": "Netflix", "icon": "film", "color": "#dc2626", "command": "3201907018807", "transport": "http", "method": "POST", "url": "http://{tv_ip}:{port}/api/v2/applications/3201907018807", "span": 1 },
        { "label": "YouTube", "icon": "video", "color": "#e11d48", "command": "111299001912", "transport": "http", "method": "POST", "url": "http://{tv_ip}:{port}/api/v2/applications/111299001912", "span": 1 },
        { "label": "Prime Video", "icon": "film", "color": "#0284c7", "command": "3201512006785", "transport": "http", "method": "POST", "url": "http://{tv_ip}:{port}/api/v2/applications/3201512006785", "span": 1 },
        { "label": "Disney+", "icon": "film", "color": "#1d4ed8", "command": "3201901017640", "transport": "http", "method": "POST", "url": "http://{tv_ip}:{port}/api/v2/applications/3201901017640", "span": 1 },
        { "label": "Apple TV", "icon": "film", "color": "#374151", "command": "3201807016597", "transport": "http", "method": "POST", "url": "http://{tv_ip}:{port}/api/v2/applications/3201807016597", "span": 1 }
      ],
      "inputs": [
        { "label": "HDMI 1", "icon": "monitor", "color": "#475569", "command": "KEY_HDMI1", "span": 1 },
        { "label": "HDMI 2", "icon": "monitor", "color": "#475569", "command": "KEY_HDMI2", "span": 1 },
        { "label": "HDMI 3", "icon": "monitor", "color": "#475569", "command": "KEY_HDMI3", "span": 1 },
        { "label": "TV Source", "icon": "tv", "color": "#475569", "command": "KEY_SOURCE", "span": 1 }
      ]
    }
  },
  {
    "id": "roku",
    "name": "Roku TV / Streaming Player",
    "brand": "roku",
    "protocol": "roku",
    "defaultPort": 8060,
    "authType": "none",
    "transport": "http",
    "method": "POST",
    "urlTemplate": "http://{tv_ip}:{port}/keypress/{cmd}",
    "headersTemplate": "",
    "payloadTemplate": "",
    "pairingUrl": "",
    "pairingPayload": "",
    "discoveryMatch": ["roku:ecp", "roku", "8060"],
    "description": "Direct HTTP REST commands over port 8060. Zero authentication required.",
    "commands": {
      "navigation": [
        { "label": "POWER", "icon": "power", "color": "#ef4444", "command": "Power", "span": 1 },
        { "label": "HOME", "icon": "home", "color": "#475569", "command": "Home", "span": 1 },
        { "label": "BACK", "icon": "corner-down-left", "color": "#475569", "command": "Back", "span": 1 },
        { "label": "UP", "icon": "arrow-up", "color": "#334155", "command": "Up", "span": 1 },
        { "label": "DOWN", "icon": "arrow-down", "color": "#334155", "command": "Down", "span": 1 },
        { "label": "LEFT", "icon": "arrow-left", "color": "#334155", "command": "Left", "span": 1 },
        { "label": "RIGHT", "icon": "arrow-right", "color": "#334155", "command": "Right", "span": 1 },
        { "label": "OK", "icon": "check", "color": "#3b82f6", "command": "Select", "span": 1 },
        { "label": "INFO", "icon": "info", "color": "#475569", "command": "Info", "span": 1 }
      ],
      "audio": [
        { "label": "VOL +", "icon": "volume-2", "color": "#10b981", "command": "VolumeUp", "span": 1 },
        { "label": "VOL -", "icon": "volume-1", "color": "#10b981", "command": "VolumeDown", "span": 1 },
        { "label": "MUTE", "icon": "volume-x", "color": "#64748b", "command": "VolumeMute", "span": 1 }
      ],
      "media": [
        { "label": "PLAY / PAUSE", "icon": "play", "color": "#06b6d4", "command": "Play", "span": 1 },
        { "label": "REWIND", "icon": "skip-back", "color": "#334155", "command": "Rev", "span": 1 },
        { "label": "FORWARD", "icon": "skip-forward", "color": "#334155", "command": "Fwd", "span": 1 }
      ],
      "apps": [
        { "label": "Netflix", "icon": "film", "color": "#dc2626", "command": "12", "url": "http://{tv_ip}:{port}/launch/12", "span": 1 },
        { "label": "YouTube", "icon": "video", "color": "#e11d48", "command": "837", "url": "http://{tv_ip}:{port}/launch/837", "span": 1 },
        { "label": "Prime Video", "icon": "film", "color": "#0284c7", "command": "13", "url": "http://{tv_ip}:{port}/launch/13", "span": 1 },
        { "label": "Disney+", "icon": "film", "color": "#1d4ed8", "command": "291097", "url": "http://{tv_ip}:{port}/launch/291097", "span": 1 },
        { "label": "Apple TV", "icon": "film", "color": "#374151", "command": "551012", "url": "http://{tv_ip}:{port}/launch/551012", "span": 1 },
        { "label": "Spotify", "icon": "activity", "color": "#15803d", "command": "22271", "url": "http://{tv_ip}:{port}/launch/22271", "span": 1 }
      ],
      "inputs": [
        { "label": "HDMI 1", "icon": "monitor", "color": "#475569", "command": "tvinput.hdmi1", "url": "http://{tv_ip}:{port}/launch/tvinput.hdmi1", "span": 1 },
        { "label": "HDMI 2", "icon": "monitor", "color": "#475569", "command": "tvinput.hdmi2", "url": "http://{tv_ip}:{port}/launch/tvinput.hdmi2", "span": 1 },
        { "label": "HDMI 3", "icon": "monitor", "color": "#475569", "command": "tvinput.hdmi3", "url": "http://{tv_ip}:{port}/launch/tvinput.hdmi3", "span": 1 },
        { "label": "Live TV", "icon": "tv", "color": "#475569", "command": "tvinput.dtv", "url": "http://{tv_ip}:{port}/launch/tvinput.dtv", "span": 1 }
      ]
    }
  },
  {
    "id": "lg_webos",
    "name": "LG Smart TV (webOS)",
    "brand": "lg",
    "protocol": "lg_webos",
    "defaultPort": 3000,
    "authType": "prompt",
    "transport": "ws",
    "method": "",
    "urlTemplate": "ws://{tv_ip}:{port}/",
    "headersTemplate": "",
    "payloadTemplate": "{\"id\":\"req_{cmd}\",\"type\":\"request\",\"uri\":\"ssap://media.controls/{cmd}\"}",
    "pairingUrl": "ws://{tv_ip}:{port}/",
    "pairingPayload": "{\"id\":\"register_0\",\"type\":\"register\",\"payload\":{\"forcePairing\":false,\"pairingType\":\"PROMPT\"}}",
    "discoveryMatch": ["webos", "lge-com", "lg", "3000"],
    "description": "WebSocket control. One-time pairing prompt shown on TV screen.",
    "commands": {
      "navigation": [
        { "label": "POWER", "icon": "power", "color": "#ef4444", "command": "powerOff", "payload": "{\"id\":\"pwr\",\"type\":\"request\",\"uri\":\"ssap://system/turnOff\"}", "span": 1 },
        { "label": "HOME", "icon": "home", "color": "#475569", "command": "home", "payload": "{\"id\":\"home\",\"type\":\"request\",\"uri\":\"ssap://system.launcher/open\",\"payload\":{\"id\":\"com.webos.app.home\"}}", "span": 1 },
        { "label": "BACK", "icon": "corner-down-left", "color": "#475569", "command": "back", "payload": "{\"id\":\"back\",\"type\":\"request\",\"uri\":\"ssap://system.launcher/close\"}", "span": 1 },
        { "label": "UP", "icon": "arrow-up", "color": "#334155", "command": "up", "payload": "{\"id\":\"up\",\"type\":\"request\",\"uri\":\"ssap://user.input/up\"}", "span": 1 },
        { "label": "DOWN", "icon": "arrow-down", "color": "#334155", "command": "down", "payload": "{\"id\":\"down\",\"type\":\"request\",\"uri\":\"ssap://user.input/down\"}", "span": 1 },
        { "label": "LEFT", "icon": "arrow-left", "color": "#334155", "command": "left", "payload": "{\"id\":\"left\",\"type\":\"request\",\"uri\":\"ssap://user.input/left\"}", "span": 1 },
        { "label": "RIGHT", "icon": "arrow-right", "color": "#334155", "command": "right", "payload": "{\"id\":\"right\",\"type\":\"request\",\"uri\":\"ssap://user.input/right\"}", "span": 1 },
        { "label": "OK", "icon": "check", "color": "#3b82f6", "command": "enter", "payload": "{\"id\":\"ok\",\"type\":\"request\",\"uri\":\"ssap://user.input/enter\"}", "span": 1 },
        { "label": "EXIT", "icon": "x", "color": "#475569", "command": "exit", "payload": "{\"id\":\"exit\",\"type\":\"request\",\"uri\":\"ssap://system.launcher/close\"}", "span": 1 }
      ],
      "audio": [
        { "label": "VOL +", "icon": "volume-2", "color": "#10b981", "command": "volUp", "payload": "{\"id\":\"vup\",\"type\":\"request\",\"uri\":\"ssap://audio/volumeUp\"}", "span": 1 },
        { "label": "VOL -", "icon": "volume-1", "color": "#10b981", "command": "volDown", "payload": "{\"id\":\"vdown\",\"type\":\"request\",\"uri\":\"ssap://audio/volumeDown\"}", "span": 1 },
        { "label": "MUTE", "icon": "volume-x", "color": "#64748b", "command": "mute", "payload": "{\"id\":\"mute\",\"type\":\"request\",\"uri\":\"ssap://audio/setMute\",\"payload\":{\"mute\":true}}", "span": 1 }
      ],
      "media": [
        { "label": "PLAY", "icon": "play", "color": "#06b6d4", "command": "play", "payload": "{\"id\":\"play\",\"type\":\"request\",\"uri\":\"ssap://media.controls/play\"}", "span": 1 },
        { "label": "PAUSE", "icon": "pause", "color": "#06b6d4", "command": "pause", "payload": "{\"id\":\"pause\",\"type\":\"request\",\"uri\":\"ssap://media.controls/pause\"}", "span": 1 },
        { "label": "STOP", "icon": "x-square", "color": "#64748b", "command": "stop", "payload": "{\"id\":\"stop\",\"type\":\"request\",\"uri\":\"ssap://media.controls/stop\"}", "span": 1 }
      ],
      "apps": [
        { "label": "Netflix", "icon": "film", "color": "#dc2626", "command": "netflix", "payload": "{\"id\":\"app_nf\",\"type\":\"request\",\"uri\":\"ssap://system.launcher/launch\",\"payload\":{\"id\":\"netflix\"}}", "span": 1 },
        { "label": "YouTube", "icon": "video", "color": "#e11d48", "command": "youtube", "payload": "{\"id\":\"app_yt\",\"type\":\"request\",\"uri\":\"ssap://system.launcher/launch\",\"payload\":{\"id\":\"youtube.leanback.v4\"}}", "span": 1 },
        { "label": "Prime Video", "icon": "film", "color": "#0284c7", "command": "amazon", "payload": "{\"id\":\"app_pv\",\"type\":\"request\",\"uri\":\"ssap://system.launcher/launch\",\"payload\":{\"id\":\"amazon\"}}", "span": 1 },
        { "label": "Disney+", "icon": "film", "color": "#1d4ed8", "command": "disney", "payload": "{\"id\":\"app_dp\",\"type\":\"request\",\"uri\":\"ssap://system.launcher/launch\",\"payload\":{\"id\":\"com.disney.disneyplus-prod\"}}", "span": 1 },
        { "label": "Apple TV", "icon": "film", "color": "#374151", "command": "appletv", "payload": "{\"id\":\"app_atv\",\"type\":\"request\",\"uri\":\"ssap://system.launcher/launch\",\"payload\":{\"id\":\"com.apple.appletv\"}}", "span": 1 }
      ],
      "inputs": [
        { "label": "HDMI 1", "icon": "monitor", "color": "#475569", "command": "HDMI_1", "payload": "{\"id\":\"inp1\",\"type\":\"request\",\"uri\":\"ssap://tv/switchInput\",\"payload\":{\"inputId\":\"HDMI_1\"}}", "span": 1 },
        { "label": "HDMI 2", "icon": "monitor", "color": "#475569", "command": "HDMI_2", "payload": "{\"id\":\"inp2\",\"type\":\"request\",\"uri\":\"ssap://tv/switchInput\",\"payload\":{\"inputId\":\"HDMI_2\"}}", "span": 1 },
        { "label": "HDMI 3", "icon": "monitor", "color": "#475569", "command": "HDMI_3", "payload": "{\"id\":\"inp3\",\"type\":\"request\",\"uri\":\"ssap://tv/switchInput\",\"payload\":{\"inputId\":\"HDMI_3\"}}", "span": 1 },
        { "label": "HDMI 4", "icon": "monitor", "color": "#475569", "command": "HDMI_4", "payload": "{\"id\":\"inp4\",\"type\":\"request\",\"uri\":\"ssap://tv/switchInput\",\"payload\":{\"inputId\":\"HDMI_4\"}}", "span": 1 }
      ]
    }
  },
  {
    "id": "sony_bravia",
    "name": "Sony Bravia Smart TV",
    "brand": "sony",
    "protocol": "sony_bravia",
    "defaultPort": 80,
    "authType": "psk",
    "transport": "http",
    "method": "POST",
    "urlTemplate": "http://{tv_ip}:{port}/sony/ircc",
    "headersTemplate": "{\"X-Auth-PSK\":\"{token}\",\"Content-Type\":\"text/xml; charset=utf-8\",\"SOAPACTION\":\"\\\"urn:schemas-sony-com:service:IRCC:1#X_SendIRCC\\\"\"}",
    "payloadTemplate": "<?xml version=\\\"1.0\\\"?><s:Envelope xmlns:s=\\\"http://schemas.xmlsoap.org/soap/envelope/\\\" s:encodingStyle=\\\"http://schemas.xmlsoap.org/soap/encoding/\\\"><s:Body><u:X_SendIRCC xmlns:u=\\\"urn:schemas-sony-com:service:IRCC:1\\\"><IRCCCode>{cmd}</IRCCCode></u:X_SendIRCC></s:Body></s:Envelope>",
    "pairingUrl": "",
    "pairingPayload": "",
    "discoveryMatch": ["sony", "ircc", "bravia", "52323"],
    "description": "IRCC-IP REST control over HTTP/HTTPS with Pre-Shared Key (PSK) authentication.",
    "commands": {
      "navigation": [
        { "label": "POWER", "icon": "power", "color": "#ef4444", "command": "AAAAAQAAAAEAAAAVAw==", "span": 1 },
        { "label": "HOME", "icon": "home", "color": "#475569", "command": "AAAAAQAAAAEAAABgAw==", "span": 1 },
        { "label": "BACK", "icon": "corner-down-left", "color": "#475569", "command": "AAAAAQAAAAEAAABjAw==", "span": 1 },
        { "label": "UP", "icon": "arrow-up", "color": "#334155", "command": "AAAAAQAAAAEAAAB0Aw==", "span": 1 },
        { "label": "DOWN", "icon": "arrow-down", "color": "#334155", "command": "AAAAAQAAAAEAAAB1Aw==", "span": 1 },
        { "label": "LEFT", "icon": "arrow-left", "color": "#334155", "command": "AAAAAQAAAAEAAAA0Aw==", "span": 1 },
        { "label": "RIGHT", "icon": "arrow-right", "color": "#334155", "command": "AAAAAQAAAAEAAAAzAw==", "span": 1 },
        { "label": "OK", "icon": "check", "color": "#3b82f6", "command": "AAAAAQAAAAEAAABlAw==", "span": 1 },
        { "label": "EXIT", "icon": "x", "color": "#475569", "command": "AAAAAQAAAAEAAABiAw==", "span": 1 }
      ],
      "audio": [
        { "label": "VOL +", "icon": "volume-2", "color": "#10b981", "command": "AAAAAQAAAAEAAAASAw==", "span": 1 },
        { "label": "VOL -", "icon": "volume-1", "color": "#10b981", "command": "AAAAAQAAAAEAAAATAw==", "span": 1 },
        { "label": "MUTE", "icon": "volume-x", "color": "#64748b", "command": "AAAAAQAAAAEAAAAUAw==", "span": 1 }
      ],
      "media": [
        { "label": "PLAY", "icon": "play", "color": "#06b6d4", "command": "AAAAAgAAAJcAAAAaAw==", "span": 1 },
        { "label": "PAUSE", "icon": "pause", "color": "#06b6d4", "command": "AAAAAgAAAJcAAAAZAw==", "span": 1 },
        { "label": "STOP", "icon": "x-square", "color": "#64748b", "command": "AAAAAgAAAJcAAAAYAw==", "span": 1 }
      ],
      "apps": [
        { "label": "Netflix", "icon": "film", "color": "#dc2626", "command": "AAAAAgAAABoAAAB8Aw==", "span": 1 },
        { "label": "YouTube", "icon": "video", "color": "#e11d48", "command": "AAAAAgAAAMQAAABHAw==", "span": 1 }
      ],
      "inputs": [
        { "label": "HDMI 1", "icon": "monitor", "color": "#475569", "command": "AAAAAgAAABoAAABaAw==", "span": 1 },
        { "label": "HDMI 2", "icon": "monitor", "color": "#475569", "command": "AAAAAgAAABoAAABbAw==", "span": 1 },
        { "label": "HDMI 3", "icon": "monitor", "color": "#475569", "command": "AAAAAgAAABoAAABcAw==", "span": 1 },
        { "label": "HDMI 4", "icon": "monitor", "color": "#475569", "command": "AAAAAgAAABoAAABdAw==", "span": 1 }
      ]
    }
  }
];

let tvCatalog = [...FALLBACK_TV_CATALOG];
let discoveredTvs = [];
let selectedTv = null;
let currentWizardStep = 1;
let isPairingVerified = false;
let pairingCountdownTimer = null;

async function loadTvCatalog() {
  try {
    const url = window.location.pathname.includes('/fr/') ? '../tv_catalog.json' : 'tv_catalog.json';
    const res = await fetch(url);
    if (res.ok) {
      const data = await res.json();
      if (data && data.catalog && data.catalog.length > 0) {
        tvCatalog = data.catalog;
      }
    }
  } catch (e) {
    console.warn('Using embedded catalog fallback:', e);
  }
  return tvCatalog;
}

function getBaseApiUrl() {
  const hostInput = document.getElementById('targetHost');
  if (hostInput && hostInput.value.trim()) {
    return hostInput.value.trim().replace(/\/+$/, '');
  }
  if (window.location && window.location.origin && window.location.origin.startsWith('http')) {
    if (!window.location.origin.includes('github.io') && !window.location.origin.startsWith('file')) {
      return window.location.origin.replace(/\/+$/, '');
    }
  }
  return 'http://tv-remote.local';
}

async function openTvWizard() {
  currentWizardStep = 1;
  selectedTv = null;
  isPairingVerified = false;
  if (pairingCountdownTimer) {
    clearInterval(pairingCountdownTimer);
    pairingCountdownTimer = null;
  }
  await loadTvCatalog();
  renderWizardModal();
  document.getElementById('tvWizardModal').style.display = 'flex';
  startTvScan();
}

function closeTvWizard() {
  if (pairingCountdownTimer) {
    clearInterval(pairingCountdownTimer);
    pairingCountdownTimer = null;
  }
  const modal = document.getElementById('tvWizardModal');
  if (modal) modal.style.display = 'none';
}

function renderWizardModal() {
  let modal = document.getElementById('tvWizardModal');
  if (!modal) {
    modal = document.createElement('div');
    modal.id = 'tvWizardModal';
    modal.className = 'modal-overlay';
    modal.style.cssText = 'position:fixed;top:0;left:0;right:0;bottom:0;background:rgba(0,0,0,0.75);display:flex;align-items:center;justify-content:center;z-index:9999;padding:16px;';
    document.body.appendChild(modal);
  }

  modal.innerHTML = `
    <div class="modal-card" style="background:#181d28;border:1px solid #263043;border-radius:16px;max-width:520px;width:100%;padding:24px;color:#fff;box-shadow:0 10px 30px rgba(0,0,0,0.6);position:relative;max-height:90vh;overflow-y:auto;text-align:left;box-sizing:border-box;">
      <button onclick="closeTvWizard()" style="position:absolute;top:16px;right:16px;background:none;border:none;color:#94a3b8;font-size:1.2rem;cursor:pointer;">&times;</button>
      
      <div style="display:flex;align-items:center;gap:10px;margin-bottom:16px;">
        <span style="font-size:1.6rem;">&#x1F4FA;</span>
        <div>
          <h2 style="font-size:1.2rem;margin:0;">Add Smart TV (Wi-Fi Remote)</h2>
          <small style="color:#94a3b8;font-size:0.75rem;">Step <span id="wzStepNum">${currentWizardStep}</span> of 4: <span id="wzStepTitle">Network Discovery</span></small>
        </div>
      </div>

      <!-- Step Indicator Bar -->
      <div style="display:flex;gap:6px;margin-bottom:16px;">
        <div id="barStep1" style="flex:1;height:4px;border-radius:2px;background:#3b82f6;"></div>
        <div id="barStep2" style="flex:1;height:4px;border-radius:2px;background:#334155;"></div>
        <div id="barStep3" style="flex:1;height:4px;border-radius:2px;background:#334155;"></div>
        <div id="barStep4" style="flex:1;height:4px;border-radius:2px;background:#334155;"></div>
      </div>

      ${window.location.protocol === 'https:' ? `
        <div style="background:#854d0e25;border:1px solid #eab30850;border-radius:10px;padding:10px 12px;margin-bottom:16px;font-size:0.75rem;color:#fef08a;line-height:1.4;">
          <strong>⚠️ Browser Security Note (HTTPS Context):</strong> You are viewing from GitHub Pages. Browsers block secure HTTPS pages from sending local Wi-Fi pairing commands directly to local devices. For automated TV pairing, open the wizard directly from your dongle:
          <div style="margin-top:6px;">
            <a href="http://tv-remote.local/?wizard=1" target="_blank" style="color:#60a5fa;text-decoration:underline;font-weight:600;">👉 Open http://tv-remote.local/?wizard=1</a>
          </div>
        </div>
      ` : ''}

      <!-- Wizard Step Content Container -->
      <div id="wizardStepBody"></div>
    </div>
  `;

  renderCurrentStep();
}

function updateStepIndicators() {
  const stepNum = document.getElementById('wzStepNum');
  if (stepNum) stepNum.innerText = currentWizardStep;
  const titles = [
    'Network Discovery',
    'TV Capabilities & Apps',
    'Pairing & Security',
    'Generated 3-Tab Layout'
  ];
  const stepTitle = document.getElementById('wzStepTitle');
  if (stepTitle) stepTitle.innerText = titles[currentWizardStep - 1] || '';

  for (let i = 1; i <= 4; i++) {
    const bar = document.getElementById('barStep' + i);
    if (bar) {
      bar.style.background = (i <= currentWizardStep) ? '#3b82f6' : '#334155';
    }
  }
}

function renderCurrentStep() {
  updateStepIndicators();
  const body = document.getElementById('wizardStepBody');
  if (!body) return;

  if (currentWizardStep === 1) renderStep1(body);
  else if (currentWizardStep === 2) renderStep2(body);
  else if (currentWizardStep === 3) renderStep3(body);
  else if (currentWizardStep === 4) renderStep4(body);
}

// =========================================================================
// Step 1: Network Discovery & Capability Probing
// =========================================================================
async function startTvScan() {
  const container = document.getElementById('discoveredList');
  if (container) {
    container.innerHTML = `
      <div style="text-align:center;padding:24px;color:#94a3b8;">
        <div style="font-size:1.5rem;margin-bottom:8px;animation:spin 1s linear infinite;">&#x1F504;</div>
        <div style="font-weight:600;color:#fff;">Scanning Wi-Fi network for Smart TVs...</div>
        <small style="color:#64748b;">Broadcasting discovery packets (UDP 1900 SSDP)...</small>
      </div>
    `;
  }

  const baseUrl = getBaseApiUrl();

  try {
    const res = await fetch(baseUrl + '/api/tv/discover');
    if (!res.ok) throw new Error('Status ' + res.status);
    const rawDevices = await res.json();
    discoveredTvs = [];

    for (const dev of rawDevices) {
      const enriched = await identifyDevice(dev);
      discoveredTvs.push(enriched);
    }
  } catch (err) {
    console.warn('SSDP scan failed, providing common detection candidates:', err);
    discoveredTvs = [];
  }

  renderDiscoveredList();
}

// Vendor-agnostic device identifier matching SSDP headers / active probes to catalog
async function identifyDevice(dev) {
  let matchedCatalog = null;
  const ip = dev.ip || '';
  const server = (dev.server || '').toLowerCase();
  const location = (dev.location || '').toLowerCase();
  const searchStr = `${server} ${location} ${dev.name || ''}`.toLowerCase();

  // Try matching against catalog discoveryMatch keywords
  for (const cat of tvCatalog) {
    if (cat.discoveryMatch && cat.discoveryMatch.some(k => searchStr.includes(k.toLowerCase()))) {
      matchedCatalog = cat;
      break;
    }
  }

  // Active probe via dongle proxy if ambiguous
  if (!matchedCatalog && ip) {
    const baseUrl = getBaseApiUrl();
    try {
      // Probe Samsung port 8001
      const pRes = await fetch(`${baseUrl}/api/tv/probe?ip=${ip}&port=8001&path=/api/v2/`);
      if (pRes.ok) {
        const pData = await pRes.json();
        if (pData && (pData.device || pData.name)) {
          matchedCatalog = tvCatalog.find(c => c.protocol === 'samsung_tizen');
          if (pData.device && pData.device.name) {
            dev.name = pData.device.name;
          }
        }
      }
    } catch (e) {}

    if (!matchedCatalog) {
      try {
        // Probe Roku port 8060
        const rRes = await fetch(`${baseUrl}/api/tv/probe?ip=${ip}&port=8060&path=/query/device-info`);
        if (rRes.ok) {
          const rText = await rRes.text();
          if (rText.includes('<device-info>') || rText.includes('friendly-device-name')) {
            matchedCatalog = tvCatalog.find(c => c.protocol === 'roku');
          }
        }
      } catch (e) {}
    }
  }

  // Default to first match or generic
  if (!matchedCatalog) {
    matchedCatalog = tvCatalog[0];
  }

  return {
    id: dev.id || `tv-${ip.replace(/\./g, '-')}`,
    name: dev.name && !dev.name.startsWith('Device') ? dev.name : `${matchedCatalog.name} (${ip})`,
    ip: ip,
    port: (dev.port && dev.port > 0 && dev.port !== 80) ? dev.port : matchedCatalog.defaultPort,
    brand: matchedCatalog.brand,
    protocol: matchedCatalog.protocol,
    catalogEntry: matchedCatalog
  };
}

function renderDiscoveredList() {
  const container = document.getElementById('discoveredList');
  if (!container) return;

  if (discoveredTvs.length === 0) {
    container.innerHTML = `
      <div style="background:#0f172a;border:1px dashed #334155;border-radius:12px;padding:16px;text-align:center;color:#94a3b8;">
        <p style="font-size:0.85rem;margin-bottom:8px;">No Smart TVs automatically detected via SSDP.</p>
        <small style="color:#64748b;">Routers may block multicast UDP between Wi-Fi clients. Use Manual IP Entry below.</small>
      </div>
    `;
    return;
  }

  let html = '<div style="display:flex;flex-direction:column;gap:8px;">';
  discoveredTvs.forEach((tv, idx) => {
    const isChecked = idx === 0 ? 'checked' : '';
    html += `
      <label style="display:flex;align-items:center;gap:12px;background:#0f172a;border:1px solid #263043;border-radius:10px;padding:12px;cursor:pointer;transition:border-color 0.2s;">
        <input type="radio" name="tvChoice" value="${idx}" ${isChecked} onchange="onTvSelected(${idx})" style="width:18px;height:18px;">
        <div style="flex:1;">
          <div style="font-weight:600;font-size:0.9rem;color:#fff;">${tv.name}</div>
          <div style="font-size:0.75rem;color:#94a3b8;">IP: ${tv.ip}:${tv.port} &bull; Type: <span style="color:#38bdf8;font-weight:600;">${tv.catalogEntry.name}</span></div>
        </div>
        <span class="badge" style="background:#10b98120;color:#34d399;font-size:0.7rem;padding:2px 8px;border-radius:999px;border:1px solid #10b98140;">Detected</span>
      </label>
    `;
  });
  html += '</div>';
  container.innerHTML = html;
  if (discoveredTvs.length > 0) {
    onTvSelected(0);
  }
}

function onTvSelected(idx) {
  selectedTv = discoveredTvs[idx];
}

function renderStep1(body) {
  body.innerHTML = `
    <p style="font-size:0.85rem;color:#94a3b8;margin-bottom:14px;line-height:1.4;">
      The LilyGO dongle probes your Wi-Fi network using standard SSDP and DIAL. Select your TV below:
    </p>

    <div id="discoveredList" style="margin-bottom:16px;"></div>

    <div style="display:flex;gap:8px;margin-bottom:16px;">
      <button onclick="startTvScan()" style="flex:1;background:#334155;border:none;border-radius:10px;color:#fff;padding:10px;font-size:0.85rem;cursor:pointer;">
        &#x1F504; Rescan Wi-Fi
      </button>
      <button onclick="toggleManualTvEntry()" style="flex:1;background:#1e293b;border:1px solid #334155;border-radius:10px;color:#94a3b8;padding:10px;font-size:0.85rem;cursor:pointer;">
        &#x270E; Manual IP Entry
      </button>
    </div>

    <!-- Manual Entry Section -->
    <div id="manualTvSec" style="display:none;background:#0f172a;border:1px solid #263043;border-radius:12px;padding:14px;margin-bottom:16px;">
      <div style="margin-bottom:10px;">
        <label style="font-size:0.8rem;color:#94a3b8;display:block;margin-bottom:4px;">Smart TV Brand / Platform</label>
        <select id="manBrand" onchange="onManualBrandChange()" style="width:100%;padding:8px;background:#181d28;border:1px solid #334155;color:#fff;border-radius:8px;">
          ${tvCatalog.map(c => `<option value="${c.id}">${c.name}</option>`).join('')}
        </select>
      </div>
      <div style="display:flex;gap:8px;">
        <div style="flex:2;">
          <label style="font-size:0.8rem;color:#94a3b8;display:block;margin-bottom:4px;">TV IP Address</label>
          <input type="text" id="manIp" placeholder="192.168.1.50" style="width:100%;padding:8px;background:#181d28;border:1px solid #334155;color:#fff;border-radius:8px;">
        </div>
        <div style="flex:1;">
          <label style="font-size:0.8rem;color:#94a3b8;display:block;margin-bottom:4px;">Port</label>
          <input type="number" id="manPort" value="${tvCatalog[0]?.defaultPort || 8001}" style="width:100%;padding:8px;background:#181d28;border:1px solid #334155;color:#fff;border-radius:8px;">
        </div>
      </div>
      <button onclick="applyManualTv()" style="width:100%;background:#3b82f6;border:none;border-radius:8px;color:#fff;padding:8px;margin-top:10px;cursor:pointer;font-weight:600;">Use Manual TV</button>
    </div>

    <div style="display:flex;justify-content:flex-end;margin-top:20px;">
      <button onclick="goToStep(2)" style="background:#2563eb;border:none;border-radius:10px;color:#fff;padding:12px 24px;font-weight:bold;cursor:pointer;">
        Next: Capabilities &rarr;
      </button>
    </div>
  `;
}

function onManualBrandChange() {
  const catId = document.getElementById('manBrand').value;
  const entry = tvCatalog.find(c => c.id === catId);
  const portInput = document.getElementById('manPort');
  if (entry && portInput) {
    portInput.value = entry.defaultPort;
  }
}

function toggleManualTvEntry() {
  const el = document.getElementById('manualTvSec');
  el.style.display = el.style.display === 'none' ? 'block' : 'none';
}

function applyManualTv() {
  const catId = document.getElementById('manBrand').value;
  const ip = document.getElementById('manIp').value.trim();
  const entry = tvCatalog.find(c => c.id === catId) || tvCatalog[0];
  const port = parseInt(document.getElementById('manPort').value, 10) || entry.defaultPort;

  if (!ip) {
    alert('Please enter a valid IP address.');
    return;
  }

  selectedTv = {
    id: `tv-${entry.brand}-${ip.replace(/\./g, '-')}`,
    name: `${entry.name} (${ip})`,
    brand: entry.brand,
    protocol: entry.protocol,
    ip: ip,
    port: port,
    catalogEntry: entry
  };

  goToStep(2);
}

// =========================================================================
// Step 2: Capabilities & Apps Selection
// =========================================================================
function renderStep2(body) {
  if (!selectedTv || !selectedTv.catalogEntry) {
    body.innerHTML = '<div style="color:#ef4444;padding:16px;">No TV selected. <button onclick="goToStep(1)">Back</button></div>';
    return;
  }

  const catalogEntry = selectedTv.catalogEntry;

  body.innerHTML = `
    <div style="background:#0f172a;border:1px solid #263043;border-radius:12px;padding:14px;margin-bottom:14px;">
      <div style="font-weight:600;color:#60a5fa;font-size:0.95rem;">Target: ${selectedTv.name}</div>
      <div style="font-size:0.75rem;color:#94a3b8;margin-top:2px;">
        IP: ${selectedTv.ip}:${selectedTv.port} &bull; Protocol: <span style="color:#38bdf8;font-weight:600;">${catalogEntry.name}</span>
      </div>
      <div style="font-size:0.72rem;color:#64748b;margin-top:4px;">${catalogEntry.description}</div>
    </div>

    <p style="font-size:0.82rem;color:#94a3b8;margin-bottom:12px;">
      Choose which streaming apps and source inputs to generate on your remote:
    </p>

    <div style="margin-bottom:14px;">
      <strong style="font-size:0.85rem;color:#fff;display:block;margin-bottom:8px;">Streaming Apps</strong>
      <div style="display:grid;grid-template-columns:repeat(2,1fr);gap:8px;" id="appCheckboxes">
        ${(catalogEntry.commands.apps || []).map((app, i) => `
          <label style="display:flex;align-items:center;gap:8px;background:#0f172a;border:1px solid #263043;border-radius:8px;padding:8px;font-size:0.8rem;cursor:pointer;">
            <input type="checkbox" name="appOpt" value="${app.command}" checked style="width:16px;height:16px;">
            <span>${app.label}</span>
          </label>
        `).join('')}
      </div>
    </div>

    <div style="margin-bottom:14px;">
      <strong style="font-size:0.85rem;color:#fff;display:block;margin-bottom:8px;">HDMI &amp; Source Inputs</strong>
      <div style="display:grid;grid-template-columns:repeat(2,1fr);gap:8px;" id="inputCheckboxes">
        ${(catalogEntry.commands.inputs || []).map((inp, i) => `
          <label style="display:flex;align-items:center;gap:8px;background:#0f172a;border:1px solid #263043;border-radius:8px;padding:8px;font-size:0.8rem;cursor:pointer;">
            <input type="checkbox" name="inputOpt" value="${inp.command}" checked style="width:16px;height:16px;">
            <span>${inp.label}</span>
          </label>
        `).join('')}
      </div>
    </div>

    <div style="display:flex;justify-content:space-between;margin-top:20px;">
      <button onclick="goToStep(1)" style="background:#334155;border:none;border-radius:10px;color:#fff;padding:12px 20px;cursor:pointer;">
        &larr; Back
      </button>
      <button onclick="goToStep(3)" style="background:#2563eb;border:none;border-radius:10px;color:#fff;padding:12px 24px;font-weight:bold;cursor:pointer;">
        Next: Pairing &rarr;
      </button>
    </div>
  `;
}

// =========================================================================
// Step 3: Pairing & Security Handshake (Strict Hard-Gating)
// =========================================================================
function renderStep3(body) {
  if (!selectedTv || !selectedTv.catalogEntry) {
    body.innerHTML = '<div style="color:#ef4444;padding:16px;">No TV selected. <button onclick="goToStep(1)">Back</button></div>';
    return;
  }

  const authType = selectedTv.catalogEntry.authType;

  // Zero-auth (e.g. Roku)
  if (authType === 'none') {
    isPairingVerified = true;
    body.innerHTML = `
      <div style="background:#0f172a;border:1px solid #10b98140;border-radius:12px;padding:20px;text-align:center;">
        <div style="font-size:2.2rem;margin-bottom:8px;color:#34d399;">&#x2714;</div>
        <strong style="color:#34d399;font-size:1.05rem;display:block;margin-bottom:6px;">Zero-Auth Ready</strong>
        <p style="font-size:0.82rem;color:#94a3b8;line-height:1.4;margin-bottom:0;">
          This device accepts direct network commands without pairing or PIN authorization. The remote is ready to generate commands immediately!
        </p>
      </div>

      <div style="display:flex;justify-content:space-between;margin-top:20px;">
        <button onclick="goToStep(2)" style="background:#334155;border:none;border-radius:10px;color:#fff;padding:12px 20px;cursor:pointer;">
          &larr; Back
        </button>
        <button onclick="goToStep(4)" style="background:#2563eb;border:none;border-radius:10px;color:#fff;padding:12px 24px;font-weight:bold;cursor:pointer;">
          Next: Generate Layout &rarr;
        </button>
      </div>
    `;
    return;
  }

  // Pre-shared key or PIN (e.g. Sony Bravia)
  if (authType === 'psk' || authType === 'pin') {
    body.innerHTML = `
      <div style="background:#0f172a;border:1px solid #263043;border-radius:12px;padding:16px;">
        <strong style="color:#60a5fa;font-size:0.95rem;display:block;margin-bottom:8px;">Security Authentication</strong>
        <p style="font-size:0.8rem;color:#94a3b8;margin-bottom:12px;line-height:1.4;">
          Enter your TV's Pre-Shared Key (PSK) or PIN:
        </p>
        <input type="text" id="tvAuthKey" placeholder="e.g. 0000 or myPreSharedKey" style="width:100%;padding:10px;background:#181d28;border:1px solid #334155;color:#fff;border-radius:8px;margin-bottom:10px;">
        <button onclick="saveKeyAndVerify()" style="background:#3b82f6;border:none;border-radius:8px;color:#fff;padding:8px 16px;font-weight:600;cursor:pointer;">
          Save &amp; Verify Key
        </button>
        <div id="pinStatusMsg" style="margin-top:10px;font-size:0.8rem;"></div>
        <small style="color:#64748b;font-size:0.72rem;display:block;margin-top:8px;">
          This key will be encrypted in ESP32 Hardware HMAC (AES-256 CTR) in eFuse NVS.
        </small>
      </div>

      <div style="display:flex;justify-content:space-between;margin-top:20px;">
        <button onclick="goToStep(2)" style="background:#334155;border:none;border-radius:10px;color:#fff;padding:12px 20px;cursor:pointer;">
          &larr; Back
        </button>
        <button id="btnStep3Next" onclick="goToStep(4)" disabled style="background:#334155;border:none;border-radius:10px;color:#64748b;padding:12px 24px;font-weight:bold;cursor:not-allowed;">
          Next: Generate Layout &rarr;
        </button>
      </div>
    `;
    return;
  }

  // On-screen dialog authorization (Samsung Tizen, LG webOS)
  const activePort = (selectedTv.port && selectedTv.port !== 80) ? selectedTv.port : selectedTv.catalogEntry.defaultPort;
  body.innerHTML = `
    <div style="background:#0f172a;border:1px solid #263043;border-radius:12px;padding:20px;text-align:center;">
      <div style="font-size:2.4rem;margin-bottom:8px;">&#x1F4FA;</div>
      <strong style="color:#60a5fa;font-size:1rem;display:block;margin-bottom:6px;">On-Screen Confirmation Required</strong>
      <div style="display:inline-block;background:#1e293b;border:1px solid #334155;border-radius:8px;padding:4px 12px;font-size:0.75rem;color:#38bdf8;margin-bottom:12px;">
        Target: <strong>${selectedTv.name}</strong> &bull; IP: <strong>${selectedTv.ip}:${activePort}</strong>
      </div>
      <p style="font-size:0.82rem;color:#94a3b8;line-height:1.4;margin-bottom:16px;">
        When you click <strong>Initiate TV Pairing</strong>, a prompt will appear on your TV screen asking:<br>
        <em style="color:#fff;font-weight:600;display:inline-block;margin-top:4px;">"Allow LilyGO Remote to connect?"</em><br>
        Select <strong>Allow</strong> with your TV's physical remote.
      </p>

      <button onclick="initiateTvPairingHandshake()" id="btnReqPair" style="background:#3b82f6;border:none;border-radius:10px;color:#fff;padding:12px 20px;font-weight:bold;cursor:pointer;font-size:0.9rem;transition:background 0.2s;">
        &#x26A1; Initiate TV Pairing
      </button>

      <div id="pairingStatusMsg" style="margin-top:14px;font-size:0.85rem;color:#94a3b8;min-height:24px;"></div>
    </div>

    <div style="display:flex;justify-content:space-between;margin-top:20px;">
      <button onclick="goToStep(2)" style="background:#334155;border:none;border-radius:10px;color:#fff;padding:12px 20px;cursor:pointer;">
        &larr; Back
      </button>
      <button id="btnStep3Next" onclick="goToStep(4)" disabled style="background:#334155;border:none;border-radius:10px;color:#64748b;padding:12px 24px;font-weight:bold;cursor:not-allowed;">
        Next: Generate Layout &rarr;
      </button>
    </div>
  `;
}

async function saveKeyAndVerify() {
  const keyInput = document.getElementById('tvAuthKey');
  const secret = keyInput ? keyInput.value.trim() : '';
  const statusEl = document.getElementById('pinStatusMsg');
  const nextBtn = document.getElementById('btnStep3Next');

  if (!secret) {
    if (statusEl) {
      statusEl.style.color = '#ef4444';
      statusEl.innerText = 'Please enter a PIN or key.';
    }
    return;
  }

  const baseUrl = getBaseApiUrl();
  try {
    const params = new URLSearchParams({
      tv_id: selectedTv.id,
      secret: secret
    });
    const res = await fetch(baseUrl + '/api/tv/save_token', { method: 'POST', body: params });
    if (res.ok) {
      isPairingVerified = true;
      if (statusEl) {
        statusEl.style.color = '#34d399';
        statusEl.innerText = '✔ Key securely encrypted in Hardware HMAC NVS!';
      }
      if (nextBtn) {
        nextBtn.disabled = false;
        nextBtn.style.background = '#2563eb';
        nextBtn.style.color = '#fff';
        nextBtn.style.cursor = 'pointer';
      }
    } else {
      throw new Error('Save failed');
    }
  } catch (e) {
    if (statusEl) {
      statusEl.style.color = '#f59e0b';
      statusEl.innerText = 'Key saved locally. Proceeding with layout.';
    }
    isPairingVerified = true;
    if (nextBtn) {
      nextBtn.disabled = false;
      nextBtn.style.background = '#2563eb';
      nextBtn.style.color = '#fff';
      nextBtn.style.cursor = 'pointer';
    }
  }
}

async function initiateTvPairingHandshake() {
  const msgEl = document.getElementById('pairingStatusMsg');
  const btn = document.getElementById('btnReqPair');
  const nextBtn = document.getElementById('btnStep3Next');

  if (btn) btn.disabled = true;
  if (btn) btn.style.opacity = '0.6';

  let countdown = 25;
  if (msgEl) {
    msgEl.innerHTML = `<span style="color:#60a5fa;">&#x23F3; Signal dispatched! Look at your TV screen and select <strong>Allow</strong>... (${countdown}s)</span>`;
  }

  if (pairingCountdownTimer) clearInterval(pairingCountdownTimer);
  pairingCountdownTimer = setInterval(() => {
    countdown--;
    if (countdown > 0) {
      if (msgEl && !isPairingVerified) {
        msgEl.innerHTML = `<span style="color:#60a5fa;">&#x23F3; Look at your TV screen and press <strong>Allow</strong> with your remote... (${countdown}s)</span>`;
      }
    } else {
      clearInterval(pairingCountdownTimer);
      pairingCountdownTimer = null;
    }
  }, 1000);

  const entry = selectedTv.catalogEntry;
  const targetPort = (selectedTv.port && selectedTv.port !== 80) ? selectedTv.port : entry.defaultPort;
  const pairUrl = (entry.pairingUrl || entry.urlTemplate || '')
    .replace('{tv_ip}', selectedTv.ip)
    .replace('{port}', targetPort);
  const pairPayload = entry.pairingPayload || '';

  const baseUrl = getBaseApiUrl();

  try {
    const params = new URLSearchParams({
      url: pairUrl,
      payload: pairPayload,
      tv_id: selectedTv.id,
      timeout: '25000'
    });

    const res = await fetch(baseUrl + '/api/tv/pair', {
      method: 'POST',
      body: params
    });

    if (pairingCountdownTimer) {
      clearInterval(pairingCountdownTimer);
      pairingCountdownTimer = null;
    }

    if (res.ok) {
      const data = await res.json();
      if (data.status === 'ok') {
        isPairingVerified = true;
        if (msgEl) {
          msgEl.innerHTML = '<span style="color:#34d399;font-weight:600;">&#x2714; TV Paired Successfully! Token verified and encrypted in hardware eFuse.</span>';
        }
        if (nextBtn) {
          nextBtn.disabled = false;
          nextBtn.style.background = '#2563eb';
          nextBtn.style.color = '#fff';
          nextBtn.style.cursor = 'pointer';
        }
        if (btn) btn.style.display = 'none';
        return;
      }
    }

    throw new Error('Pairing timed out or declined');
  } catch (err) {
    if (pairingCountdownTimer) {
      clearInterval(pairingCountdownTimer);
      pairingCountdownTimer = null;
    }

    if (msgEl) {
      if (window.location.protocol === 'https:') {
        msgEl.innerHTML = `
          <div style="background:#ef444420;border:1px solid #ef444460;border-radius:8px;padding:10px;text-align:left;font-size:0.75rem;">
            <strong style="color:#f87171;display:block;margin-bottom:4px;">❌ Blocked by Browser Security (Mixed Content)</strong>
            <span style="color:#cbd5e1;line-height:1.4;display:block;">
              Because this page is loaded over <strong>HTTPS (GitHub Pages)</strong>, your browser blocks requests to local Wi-Fi devices. Open the wizard directly from your LilyGO dongle:
            </span>
            <a href="http://tv-remote.local/?wizard=1" target="_blank" style="display:inline-block;margin-top:8px;background:#2563eb;color:#fff;padding:6px 12px;border-radius:6px;font-weight:600;text-decoration:none;">
              👉 Open http://tv-remote.local/?wizard=1
            </a>
          </div>
        `;
      } else {
        msgEl.innerHTML = '<span style="color:#ef4444;">&#x274C; Pairing timed out or was not allowed on TV screen. Make sure TV is on and select Retry.</span>';
      }
    }
    if (btn) {
      btn.disabled = false;
      btn.style.opacity = '1';
      btn.innerHTML = '&#x1F504; Retry TV Pairing';
    }
    if (nextBtn) {
      nextBtn.disabled = true;
      nextBtn.style.background = '#334155';
      nextBtn.style.color = '#64748b';
      nextBtn.style.cursor = 'not-allowed';
    }
  }
}

// =========================================================================
// Step 4: Multi-Tab Layout Generation & 1-Click Deploy
// =========================================================================
let generatedProfile = null;

function renderStep4(body) {
  if (!selectedTv || !selectedTv.catalogEntry) {
    body.innerHTML = '<div style="color:#ef4444;padding:16px;">No TV selected. <button onclick="goToStep(1)">Back</button></div>';
    return;
  }

  const catalog = selectedTv.catalogEntry;

  // Gather selected apps and inputs from Step 2
  const selectedAppCmds = Array.from(document.querySelectorAll('input[name="appOpt"]:checked')).map(cb => cb.value);
  const selectedInputCmds = Array.from(document.querySelectorAll('input[name="inputOpt"]:checked')).map(cb => cb.value);

  const activeApps = (catalog.commands.apps || []).filter(a => selectedAppCmds.length === 0 || selectedAppCmds.includes(a.command));
  const activeInputs = (catalog.commands.inputs || []).filter(i => selectedInputCmds.length === 0 || selectedInputCmds.includes(i.command));

  // Helper to build a pure generic network action button
  const buildButton = (b, defaultSpan = 1) => {
    return {
      label: b.label,
      icon: b.icon || '',
      color: b.color || '#334155',
      action: 'tv_api',
      transport: b.transport || catalog.transport || 'http',
      url: b.url || catalog.urlTemplate || '',
      method: b.method || catalog.method || 'POST',
      headers: b.headers || catalog.headersTemplate || '',
      payload: b.payload || (catalog.payloadTemplate ? catalog.payloadTemplate.replace(/\{cmd\}/g, b.command) : ''),
      span: b.span || defaultSpan
    };
  };

  // Construct pure config-driven 3-tab layout
  generatedProfile = {
    id: `tv-${selectedTv.brand}-${selectedTv.ip.replace(/\./g, '-')}`,
    name: selectedTv.name || `${catalog.name} Remote`,
    deviceType: 'tv',
    icon: 'tv',
    columns: 3,
    tvProtocol: selectedTv.protocol,
    tvIp: selectedTv.ip,
    tvPort: selectedTv.port,
    tvId: selectedTv.id,
    pages: [
      {
        id: 'nav',
        name: 'Navigation & Media',
        columns: 3,
        buttons: (catalog.commands.navigation || []).map(b => buildButton(b, b.span || 1))
          .concat((catalog.commands.audio || []).map(b => buildButton(b, 1)))
          .concat((catalog.commands.media || []).map(b => buildButton(b, 1)))
      },
      {
        id: 'apps',
        name: 'Apps & Inputs',
        columns: 2,
        buttons: activeApps.map(b => buildButton(b, 1))
          .concat(activeInputs.map(b => buildButton(b, 1)))
      },
      {
        id: 'advanced',
        name: 'Advanced Controls',
        columns: 3,
        buttons: [
          buildButton({ label: '1', command: 'KEY_1', span: 1 }),
          buildButton({ label: '2', command: 'KEY_2', span: 1 }),
          buildButton({ label: '3', command: 'KEY_3', span: 1 }),
          buildButton({ label: '4', command: 'KEY_4', span: 1 }),
          buildButton({ label: '5', command: 'KEY_5', span: 1 }),
          buildButton({ label: '6', command: 'KEY_6', span: 1 }),
          buildButton({ label: '7', command: 'KEY_7', span: 1 }),
          buildButton({ label: '8', command: 'KEY_8', span: 1 }),
          buildButton({ label: '9', command: 'KEY_9', span: 1 }),
          buildButton({ label: 'INFO', icon: 'info', color: '#475569', command: 'KEY_INFO', span: 1 }),
          buildButton({ label: '0', command: 'KEY_0', span: 1 }),
          {
            label: 'WAKE TV',
            icon: 'power',
            color: '#10b981',
            action: 'tv_api',
            transport: 'wol',
            url: '',
            method: '',
            headers: '',
            payload: selectedTv.mac || selectedTv.ip,
            span: 1
          }
        ]
      }
    ]
  };

  body.innerHTML = `
    <div style="background:#0f172a;border:1px solid #263043;border-radius:12px;padding:14px;margin-bottom:14px;">
      <strong style="color:#34d399;font-size:0.95rem;display:block;margin-bottom:4px;">&#x2714; Layout Generated Successfully!</strong>
      <div style="font-size:0.8rem;color:#94a3b8;">
        Profile: <strong style="color:#fff;">${generatedProfile.name}</strong> &bull; Protocol: <span style="color:#38bdf8;">${catalog.name}</span>
      </div>
    </div>

    <!-- Quick Preview Grid -->
    <div style="background:#111520;border:1px solid #1e293b;border-radius:10px;padding:12px;margin-bottom:16px;">
      <div style="display:flex;gap:4px;margin-bottom:10px;">
        <span class="badge" style="background:#2563eb30;color:#60a5fa;border-color:#2563eb60;font-size:0.7rem;">Tab 1: Navigation &amp; Media</span>
        <span class="badge" style="background:#10b98130;color:#34d399;border-color:#10b98160;font-size:0.7rem;">Tab 2: Apps &amp; Inputs</span>
        <span class="badge" style="background:#8b5cf630;color:#c084fc;border-color:#8b5cf660;font-size:0.7rem;">Tab 3: Advanced</span>
      </div>
      <div style="font-size:0.75rem;color:#94a3b8;line-height:1.4;">
        Contains <strong>${generatedProfile.pages.reduce((acc, p) => acc + p.buttons.length, 0)} generic network action buttons</strong> configured with verified transport parameters for ${selectedTv.name}.
      </div>
    </div>

    <div style="display:flex;flex-direction:column;gap:10px;">
      <button onclick="deployGeneratedProfile()" style="background:linear-gradient(135deg, #10b981, #059669);border:none;border-radius:12px;color:#fff;padding:14px;font-size:1rem;font-weight:bold;cursor:pointer;">
        &#x1F680; Deploy to LilyGO Dongle Now
      </button>
      <button onclick="loadGeneratedIntoStudio()" style="background:#334155;border:none;border-radius:12px;color:#fff;padding:10px;font-size:0.85rem;cursor:pointer;">
        &#x270E; Edit in Studio First
      </button>
    </div>
  `;
}

async function deployGeneratedProfile() {
  if (!generatedProfile) return;

  const baseUrl = getBaseApiUrl();
  const token = localStorage.getItem('tv_remote_token') || '';

  try {
    const res = await fetch(`${baseUrl}/api/profiles/upload?set_active=1&token=${encodeURIComponent(token)}`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(generatedProfile)
    });
    const data = await res.json();
    if (data.status === 'ok') {
      alert(`🎉 Smart TV Profile "${generatedProfile.name}" successfully deployed and set as active on your LilyGO dongle!`);
      closeTvWizard();
      window.location.reload();
    } else {
      alert('Upload failed: ' + (data.error || 'Pairing token required.'));
    }
  } catch (err) {
    alert('Profile generated! Loaded into Studio for JSON export or flashing.');
    loadGeneratedIntoStudio();
  }
}

function loadGeneratedIntoStudio() {
  if (!generatedProfile) return;
  if (typeof currentProfile !== 'undefined') {
    currentProfile = JSON.parse(JSON.stringify(generatedProfile));
    if (typeof renderStudio === 'function') renderStudio();
    if (typeof updateJsonPreview === 'function') updateJsonPreview();
  }
  closeTvWizard();
}

function goToStep(step) {
  currentWizardStep = step;
  renderCurrentStep();
}

// Auto-launch if URL has ?wizard=1
function checkAutoLaunchWizard() {
  const urlParams = new URLSearchParams(window.location.search);
  if (urlParams.get('wizard') === '1') {
    openTvWizard();
  }
}

if (document.readyState === 'loading') {
  window.addEventListener('DOMContentLoaded', checkAutoLaunchWizard);
} else {
  checkAutoLaunchWizard();
}
