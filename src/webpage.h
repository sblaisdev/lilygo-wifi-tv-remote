#pragma once
#include <Arduino.h>

const char PAGE_ICON_SVG[] PROGMEM = R"rawliteral(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" width="512" height="512">
  <defs>
    <linearGradient id="bgGrad" x1="0%" y1="0%" x2="100%" y2="100%">
      <stop offset="0%" stop-color="#1e2536"/>
      <stop offset="100%" stop-color="#0e1118"/>
    </linearGradient>
    <linearGradient id="blueGrad" x1="0%" y1="0%" x2="100%" y2="100%">
      <stop offset="0%" stop-color="#60a5fa"/>
      <stop offset="100%" stop-color="#2563eb"/>
    </linearGradient>
    <filter id="glow" x="-20%" y="-20%" width="140%" height="140%">
      <feGaussianBlur stdDeviation="10" result="blur"/>
      <feComposite in="SourceGraphic" in2="blur" operator="over"/>
    </filter>
  </defs>
  <rect width="512" height="512" rx="115" fill="url(#bgGrad)"/>
  <rect width="504" height="504" x="4" y="4" rx="111" fill="none" stroke="#3b82f6" stroke-width="6" stroke-opacity="0.3"/>
  <rect x="76" y="86" width="360" height="230" rx="24" fill="#141923" stroke="url(#blueGrad)" stroke-width="12" filter="url(#glow)"/>
  <path d="M 216 316 L 196 356 L 316 356 L 296 316 Z" fill="#2563eb" opacity="0.8"/>
  <rect x="176" y="356" width="160" height="12" rx="6" fill="#3b82f6"/>
  <g transform="translate(256, 201)">
    <circle cx="0" cy="0" r="22" fill="#10b981"/>
    <path d="M -16 -32 L 0 -54 L 16 -32 Z" fill="url(#blueGrad)"/>
    <path d="M -16 32 L 0 54 L 16 32 Z" fill="url(#blueGrad)"/>
    <path d="M -32 -16 L -54 0 L -32 16 Z" fill="url(#blueGrad)"/>
    <path d="M 32 -16 L 54 0 L 32 16 Z" fill="url(#blueGrad)"/>
  </g>
  <g fill="#93c5fd" opacity="0.6">
    <rect x="120" y="270" width="30" height="8" rx="3"/>
    <rect x="160" y="270" width="30" height="8" rx="3"/>
    <rect x="200" y="270" width="112" height="8" rx="3"/>
    <rect x="322" y="270" width="30" height="8" rx="3"/>
    <rect x="362" y="270" width="30" height="8" rx="3"/>
  </g>
</svg>
)rawliteral";

const char PAGE_INDEX_TEMPLATE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <meta name="theme-color" content="#10131a">
  <meta name="mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
  <link rel="manifest" href="/manifest.json">
  <link rel="icon" type="image/svg+xml" href="/icon.svg">
  <link rel="apple-touch-icon" href="/icon.svg">
  <title>%ROOM_NAME% TV Remote</title>
  <style>
    :root {
      --bg: #10131a;
      --card-bg: #181d28;
      --card-border: #263043;
      --primary: #3b82f6;
      --primary-hover: #2563eb;
      --accent: #10b981;
      --danger: #ef4444;
      --text: #f3f4f6;
      --text-muted: #9ca3af;
      --dpad-bg: #1f2737;
      --dpad-hover: #2d374d;
      --btn-active: #3b82f640;
    }
    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
      user-select: none;
      -webkit-user-select: none;
      -webkit-tap-highlight-color: transparent;
      touch-action: manipulation;
    }
    body {
      background: var(--bg);
      color: var(--text);
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 12px;
      min-height: 100vh;
      overflow-x: hidden;
    }
    .container {
      width: 100%;
      max-width: 380px;
      display: flex;
      flex-direction: column;
      gap: 12px;
    }
    header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 4px 6px;
    }
    .brand {
      display: flex;
      align-items: center;
      gap: 8px;
      font-weight: 700;
      font-size: 1.1rem;
      letter-spacing: -0.5px;
    }
    .brand-icon {
      width: 30px;
      height: 30px;
      border-radius: 8px;
    }
    .header-links {
      display: flex;
      align-items: center;
      gap: 10px;
    }
    .config-link {
      color: var(--text-muted);
      text-decoration: none;
      font-size: 1.2rem;
      display: flex;
      align-items: center;
      justify-content: center;
      width: 32px;
      height: 32px;
      border-radius: 8px;
      background: var(--card-bg);
      border: 1px solid var(--card-border);
    }
    .badge {
      display: inline-block;
      font-size: 0.75rem;
      background: #0f766e40;
      color: #34d399;
      padding: 3px 8px;
      border-radius: 9999px;
      border: 1px solid #10b98140;
      font-weight: 500;
    }
    .card {
      background: var(--card-bg);
      border: 1px solid var(--card-border);
      border-radius: 18px;
      padding: 14px;
      box-shadow: 0 4px 20px rgba(0,0,0,0.3);
    }
    .typing-card {
      display: flex;
      flex-direction: column;
      gap: 8px;
    }
    .typing-header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      color: var(--text-muted);
      font-size: 0.8rem;
    }
    .live-indicator {
      display: flex;
      align-items: center;
      gap: 6px;
      color: var(--accent);
      font-weight: 600;
    }
    .live-pulse {
      width: 8px;
      height: 8px;
      border-radius: 50%;
      background: var(--accent);
      animation: pulse 1.5s infinite;
    }
    @keyframes pulse {
      0% { opacity: 0.4; transform: scale(0.9); }
      50% { opacity: 1; transform: scale(1.1); }
      100% { opacity: 0.4; transform: scale(0.9); }
    }
    .input-wrapper {
      position: relative;
      display: flex;
      align-items: center;
    }
    input[type="text"] {
      width: 100%;
      padding: 12px 38px 12px 14px;
      background: #0d1017;
      border: 1px solid var(--card-border);
      border-radius: 12px;
      color: #fff;
      font-size: 1rem;
      outline: none;
      user-select: auto;
      -webkit-user-select: auto;
    }
    input[type="text"]:focus {
      border-color: var(--primary);
    }
    .clear-btn {
      position: absolute;
      right: 10px;
      background: none;
      border: none;
      color: var(--text-muted);
      font-size: 1.2rem;
      cursor: pointer;
      display: none;
    }
    .dpad-container {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      grid-template-rows: repeat(3, 62px);
      gap: 8px;
      max-width: 250px;
      margin: 0 auto;
    }
    .dpad-btn {
      background: var(--dpad-bg);
      border: 1px solid var(--card-border);
      border-radius: 14px;
      color: #fff;
      font-size: 1.3rem;
      display: flex;
      align-items: center;
      justify-content: center;
      cursor: pointer;
      box-shadow: 0 3px 0 rgba(0,0,0,0.4);
    }
    .dpad-btn:active {
      transform: translateY(2px);
      box-shadow: none;
      background: var(--btn-active);
    }
    .dpad-center {
      background: var(--primary);
      font-weight: 700;
      font-size: 1.1rem;
    }
    .keys-grid, .media-grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 8px;
    }
    .media-grid {
      grid-template-columns: repeat(4, 1fr);
    }
    .keys-grid button, .media-grid button {
      background: var(--dpad-bg);
      border: 1px solid var(--card-border);
      border-radius: 12px;
      color: #fff;
      font-size: 0.85rem;
      font-weight: 600;
      padding: 12px 6px;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 4px;
      cursor: pointer;
    }
    .keys-grid button:active, .media-grid button:active {
      background: var(--btn-active);
    }
    .feedback-toast {
      font-size: 0.8rem;
      color: var(--text-muted);
      text-align: center;
      min-height: 18px;
    }
    /* Pairing Modal */
    .modal-overlay {
      position: fixed; top: 0; left: 0; right: 0; bottom: 0;
      background: rgba(10, 13, 20, 0.9); backdrop-filter: blur(8px);
      display: none; align-items: center; justify-content: center;
      padding: 20px; z-index: 1000;
    }
    .modal-card {
      background: var(--card-bg); border: 1px solid var(--card-border);
      border-radius: 18px; padding: 24px; max-width: 360px; width: 100%;
      text-align: center; box-shadow: 0 10px 30px rgba(0,0,0,0.6);
    }
  </style>
</head>
<body>
  <div class="container">
    <header>
      <div class="brand">
        <img src="/icon.svg" class="brand-icon" alt="TV icon">
        <span>%ROOM_NAME% TV</span>
      </div>
      <div class="header-links">
        <span class="badge" id="connStatus">Connecting...</span>
        <a href="/setup" class="config-link" title="Wi-Fi Settings">&#x2699;</a>
      </div>
    </header>

    <!-- Live Typing Console -->
    <div class="card typing-card">
      <div class="typing-header">
        <span class="live-indicator">
          <span class="live-pulse"></span>
          Live Keystroke Stream
        </span>
        <span id="encStatus" style="font-size: 0.72rem; color: #38bdf8;">&#x1F512; Encrypted</span>
      </div>
      <div class="input-wrapper">
        <input 
          type="text" 
          id="liveInput" 
          placeholder="Tap here to type (streams live to TV)" 
          autocomplete="off" 
          autocorrect="off" 
          autocapitalize="off" 
          spellcheck="false"
        >
        <button class="clear-btn" id="clearBtn" onclick="clearInput()">&times;</button>
      </div>
    </div>

    <!-- D-Pad Navigation -->
    <div class="card">
      <div class="dpad-container">
        <div></div>
        <button class="dpad-btn" onclick="sendKey('UP')">&#x25B2;</button>
        <div></div>

        <button class="dpad-btn" onclick="sendKey('LEFT')">&#x25C0;</button>
        <button class="dpad-btn dpad-center" onclick="sendKey('ENTER')">OK</button>
        <button class="dpad-btn" onclick="sendKey('RIGHT')">&#x25B6;</button>

        <div></div>
        <button class="dpad-btn" onclick="sendKey('DOWN')">&#x25BC;</button>
        <div></div>
      </div>
    </div>

    <!-- System / TV Keys -->
    <div class="card">
      <div class="keys-grid">
        <button onclick="sendKey('BACK')">&#x232B; Back / Esc</button>
        <button onclick="sendKey('HOME')">&#x1F3E0; Home</button>
        <button onclick="sendKey('BACKSPACE')">&#x21FD; Backspace</button>
        <button onclick="sendKey('SPACE')">Space</button>
        <button onclick="sendKey('TAB')">&#x21E5; Tab</button>
        <button onclick="sendKey('ENTER')">&#x23CE; Enter</button>
      </div>
    </div>

    <!-- Media Controls -->
    <div class="card">
      <div class="media-grid">
        <button onclick="sendKey('VOL_DOWN')">&#x1F508; Vol -</button>
        <button onclick="sendKey('VOL_UP')">&#x1F50A; Vol +</button>
        <button onclick="sendKey('MUTE')">&#x1F507; Mute</button>
        <button onclick="sendKey('PLAY_PAUSE')">&#x25B6;&#x23F8; Play</button>
      </div>
    </div>

    <div class="feedback-toast" id="toast">Ready</div>
  </div>

  <!-- Physical Pairing Modal -->
  <div id="pairOverlay" class="modal-overlay">
    <div class="modal-card">
      <div style="font-size: 2.5rem; margin-bottom: 12px;">&#x1F512;</div>
      <h2 style="font-size: 1.2rem; margin-bottom: 8px;">Device Pairing Required</h2>
      <p style="color: var(--text-muted); font-size: 0.85rem; margin-bottom: 16px; line-height: 1.4;">
        This TV remote requires physical authorization. Tap below, then <strong>press the physical button on the TV stick</strong> to approve this phone.
      </p>
      <button id="btnPair" style="width: 100%; padding: 12px; font-weight: bold; background: var(--primary); border: none; border-radius: 10px; color: #fff; cursor: pointer;" onclick="startPairing()">
        Request Pairing
      </button>
      <div id="pairStatus" style="margin-top: 14px; font-size: 0.85rem; color: #38bdf8;"></div>
    </div>
  </div>

  <script>
    const AUTH_REQUIRED = %AUTH_REQUIRED%;
    let deviceToken = localStorage.getItem('tv_remote_token') || '';
    let cryptoKey = null;

    const input = document.getElementById('liveInput');
    const clearBtn = document.getElementById('clearBtn');
    const toast = document.getElementById('toast');
    const connStatus = document.getElementById('connStatus');
    const pairOverlay = document.getElementById('pairOverlay');

    let previousVal = '';
    let ws = null;

    function hexToBytes(hex) {
      const bytes = new Uint8Array(hex.length / 2);
      for (let i = 0; i < bytes.length; i++) bytes[i] = parseInt(hex.substr(i * 2, 2), 16);
      return bytes;
    }

    function bytesToHex(bytes) {
      return Array.from(bytes).map(b => b.toString(16).padStart(2, '0')).join('');
    }

    // Initialize AES-CTR in-browser keystroke encryption key
    async function initCryptoKey() {
      try {
        if (!window.crypto || !window.crypto.subtle) return;
        const seedStr = deviceToken || 'tv-remote-default-salt-v1';
        const rawSeed = new TextEncoder().encode(seedStr);
        const hash = await crypto.subtle.digest('SHA-256', rawSeed);
        cryptoKey = await crypto.subtle.importKey('raw', hash, { name: 'AES-CTR' }, false, ['encrypt']);
      } catch (e) {
        console.warn('WebCrypto not available:', e);
      }
    }
    initCryptoKey();

    function getOrCreateDeviceId() {
      let id = localStorage.getItem('tv_remote_devid');
      if (!id) {
        id = Array.from(crypto.getRandomValues(new Uint8Array(8))).map(b => b.toString(16).padStart(2, '0')).join('');
        localStorage.setItem('tv_remote_devid', id);
      }
      return id;
    }

    let pollTimer = null;
    async function startPairing() {
      const devId = getOrCreateDeviceId();
      const statusEl = document.getElementById('pairStatus');
      statusEl.innerText = 'Request sent! Press the button on the TV stick now...';
      try {
        await fetch('/api/pair_request?device=' + devId, { method: 'POST' });
        if (pollTimer) clearInterval(pollTimer);
        pollTimer = setInterval(async () => {
          try {
            const res = await fetch('/api/pair_status?device=' + devId);
            const data = await res.json();
            if (data.status === 'approved' && data.token) {
              clearInterval(pollTimer);
              localStorage.setItem('tv_remote_token', data.token);
              deviceToken = data.token;
              await initCryptoKey();
              pairOverlay.style.display = 'none';
              setToast('Device paired successfully! \u2713');
              connectWS();
            } else if (data.status === 'expired') {
              clearInterval(pollTimer);
              statusEl.innerText = 'Pairing timed out. Tap button to try again.';
            }
          } catch (e) {}
        }, 1000);
      } catch (e) {
        statusEl.innerText = 'Error connecting to device.';
      }
    }

    function checkAuthRequirement() {
      if (AUTH_REQUIRED && !deviceToken) {
        pairOverlay.style.display = 'flex';
        return false;
      }
      return true;
    }

    function connectWS() {
      if (!checkAuthRequirement()) return;

      const loc = window.location;
      const wsUri = (loc.protocol === 'https:' ? 'wss:' : 'ws:') + '//' + loc.hostname + ':81/';
      ws = new WebSocket(wsUri);

      ws.onopen = () => {
        connStatus.innerText = 'Connected (Live)';
        setToast('Connected! Ready to send commands');
        if (deviceToken) {
          ws.send('AUTH:' + deviceToken);
        }
      };

      ws.onmessage = (e) => {
        if (e.data === 'AUTH_REQUIRED') {
          pairOverlay.style.display = 'flex';
        } else if (e.data === 'AUTH_OK') {
          pairOverlay.style.display = 'none';
        }
      };

      ws.onclose = () => {
        connStatus.innerText = 'Reconnecting...';
        setToast('Connection lost. Reconnecting...');
        setTimeout(connectWS, 1500);
      };

      ws.onerror = () => {
        ws.close();
      };
    }

    connectWS();

    async function sendPayload(msg) {
      // In-browser WebCrypto encryption (Item 1 Option A)
      if (cryptoKey) {
        try {
          const counter = new Uint8Array(16);
          crypto.getRandomValues(counter);
          const encoded = new TextEncoder().encode(msg);
          const ciphertext = await crypto.subtle.encrypt(
            { name: 'AES-CTR', counter: counter, length: 64 },
            cryptoKey,
            encoded
          );
          const encPacket = 'E:' + bytesToHex(counter) + ':' + bytesToHex(new Uint8Array(ciphertext));
          transmit(encPacket);
          return;
        } catch (err) {
          console.warn('Crypto error, sending plain:', err);
        }
      }
      transmit(msg);
    }

    function transmit(packet) {
      if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(packet);
      } else {
        if (packet.startsWith('K:') || packet.startsWith('E:')) {
          fetch('/sendkey', { 
            method: 'POST', 
            body: new URLSearchParams({ key: packet, token: deviceToken }) 
          });
        } else if (packet.startsWith('T:')) {
          fetch('/sendtext', { 
            method: 'POST', 
            body: new URLSearchParams({ text: packet.slice(2), token: deviceToken }) 
          });
        }
      }
    }

    function vibrate() {
      if (navigator.vibrate) navigator.vibrate(25);
    }

    function setToast(msg) {
      toast.innerText = msg;
    }

    input.addEventListener('input', (e) => {
      const current = input.value;
      clearBtn.style.display = current ? 'block' : 'none';

      if (current.length > previousVal.length && current.startsWith(previousVal)) {
        const added = current.slice(previousVal.length);
        sendPayload('T:' + added);
        setToast(`Sent: "${added}"`);
      } else if (current.length < previousVal.length && previousVal.startsWith(current)) {
        const deletedCount = previousVal.length - current.length;
        for (let i = 0; i < deletedCount; i++) {
          sendPayload('K:BACKSPACE');
        }
        setToast(`Sent: Backspace (${deletedCount})`);
      } else if (current !== previousVal) {
        for (let i = 0; i < previousVal.length; i++) {
          sendPayload('K:BACKSPACE');
        }
        if (current.length > 0) {
          sendPayload('T:' + current);
        }
        setToast(`Updated: "${current}"`);
      }

      previousVal = current;
    });

    input.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') {
        e.preventDefault();
        vibrate();
        sendPayload('K:ENTER');
        setToast('Sent: Enter ⏎');
        input.value = '';
        previousVal = '';
        clearBtn.style.display = 'none';
      } else if (e.key === 'ArrowUp') {
        e.preventDefault();
        sendPayload('K:UP');
        setToast('Key: UP');
      } else if (e.key === 'ArrowDown') {
        e.preventDefault();
        sendPayload('K:DOWN');
        setToast('Key: DOWN');
      } else if (e.key === 'ArrowLeft') {
        e.preventDefault();
        sendPayload('K:LEFT');
        setToast('Key: LEFT');
      } else if (e.key === 'ArrowRight') {
        e.preventDefault();
        sendPayload('K:RIGHT');
        setToast('Key: RIGHT');
      } else if (e.key === 'Escape') {
        e.preventDefault();
        sendPayload('K:BACK');
        setToast('Key: BACK / ESC');
      }
    });

    function sendKey(keyName) {
      vibrate();
      sendPayload('K:' + keyName);
      setToast('Key: ' + keyName);
    }

    function clearInput() {
      input.value = '';
      previousVal = '';
      clearBtn.style.display = 'none';
    }
  </script>
</body>
</html>
)rawliteral";

const char PAGE_SETUP_TEMPLATE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <meta name="theme-color" content="#10131a">
  <link rel="icon" type="image/svg+xml" href="/icon.svg">
  <title>Wi-Fi &amp; Device Setup - %ROOM_NAME% TV</title>
  <style>
    :root {
      --bg: #10131a;
      --card-bg: #181d28;
      --card-border: #263043;
      --primary: #3b82f6;
      --primary-hover: #2563eb;
      --accent: #10b981;
      --text: #f3f4f6;
      --text-muted: #9ca3af;
    }
    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
      -webkit-tap-highlight-color: transparent;
    }
    body {
      background: var(--bg);
      color: var(--text);
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 16px;
      min-height: 100vh;
    }
    .container {
      width: 100%;
      max-width: 420px;
      display: flex;
      flex-direction: column;
      gap: 16px;
    }
    header {
      display: flex;
      align-items: center;
      gap: 10px;
      padding: 8px 0;
    }
    .brand-icon {
      width: 28px;
      height: 28px;
    }
    h1 {
      font-size: 1.2rem;
      font-weight: 700;
    }
    .card {
      background: var(--card-bg);
      border: 1px solid var(--card-border);
      border-radius: 16px;
      padding: 20px;
      box-shadow: 0 4px 20px rgba(0,0,0,0.3);
    }
    .badge {
      display: inline-block;
      font-size: 0.75rem;
      background: #0f766e40;
      color: #34d399;
      padding: 4px 10px;
      border-radius: 9999px;
      border: 1px solid #10b98140;
      margin-bottom: 14px;
    }
    .form-group {
      display: flex;
      flex-direction: column;
      gap: 6px;
      margin-bottom: 16px;
    }
    label {
      font-size: 0.85rem;
      font-weight: 600;
      color: var(--text-muted);
    }
    select, input[type="text"], input[type="password"] {
      width: 100%;
      padding: 12px 14px;
      font-size: 1rem;
      color: #fff;
      background: #0d1017;
      border: 1px solid var(--card-border);
      border-radius: 10px;
      outline: none;
    }
    select:focus, input:focus {
      border-color: var(--primary);
    }
    .password-wrapper {
      position: relative;
      display: flex;
      align-items: center;
    }
    .toggle-pwd {
      position: absolute;
      right: 12px;
      background: none;
      border: none;
      color: var(--text-muted);
      cursor: pointer;
      font-size: 0.85rem;
    }
    .btn-submit {
      width: 100%;
      background: var(--primary);
      border: none;
      color: #fff;
      font-size: 1rem;
      font-weight: 600;
      padding: 14px;
      border-radius: 12px;
      cursor: pointer;
      transition: background 0.15s;
      margin-top: 10px;
    }
    .btn-submit:active {
      background: var(--primary-hover);
    }
    .note {
      font-size: 0.75rem;
      color: var(--text-muted);
      line-height: 1.4;
      margin-top: 14px;
      text-align: center;
    }
    .back-link {
      display: block;
      text-align: center;
      color: var(--primary);
      text-decoration: none;
      font-size: 0.9rem;
      margin-top: 14px;
    }
  </style>
</head>
<body>
  <div class="container">
    <header>
      <img src="/icon.svg" class="brand-icon" alt="TV icon">
      <h1>Wi-Fi &amp; Device Setup</h1>
    </header>

    <div class="card">
      %CRYPTO_STATUS_BADGE%
      
      <form action="/savewifi" method="POST">
        <div class="form-group">
          <label for="op_mode">Network Operating Mode</label>
          <select id="op_mode" name="op_mode" onchange="toggleMode()">
            <option value="sta" %MODE_STA_SELECTED%>Connect to Home Wi-Fi (Station)</option>
            <option value="ap" %MODE_AP_SELECTED%>Standalone Private Wi-Fi (Access Point)</option>
          </select>
        </div>

        <!-- Station Mode Section -->
        <div id="staSection">
          <div class="form-group">
            <label for="ssid">Select Wi-Fi Network</label>
            <select id="ssid" name="ssid">
              %WIFI_OPTIONS%
            </select>
          </div>

          <div class="form-group">
            <label for="ssid_custom">Or enter SSID manually</label>
            <input type="text" id="ssid_custom" name="ssid_custom" placeholder="Network Name (if hidden)">
          </div>
        </div>

        <!-- Standalone AP Mode Section -->
        <div id="apSection" style="display: none;">
          <div class="form-group">
            <label for="ap_ssid">Access Point Name (SSID)</label>
            <input type="text" id="ap_ssid" name="ap_ssid" value="%AP_SSID%" placeholder="e.g. My-TV-Remote">
          </div>
        </div>

        <div class="form-group">
          <label for="password">Wi-Fi Password</label>
          <div class="password-wrapper">
            <input type="password" id="password" name="password" placeholder="Enter password" required>
            <button type="button" class="toggle-pwd" onclick="togglePassword()">Show</button>
          </div>
        </div>

        <div class="form-group">
          <label for="room">Room Name</label>
          <input type="text" id="room" name="room" value="%ROOM_NAME%" placeholder="e.g. Living Room, Bedroom" required>
        </div>

        <div class="form-group">
          <label for="hostname">mDNS Hostname</label>
          <input type="text" id="hostname" name="hostname" value="%MDNS_HOSTNAME%" placeholder="e.g. tv-remote" required>
          <small style="color: var(--text-muted); font-size: 0.75rem;">Access via http://&lt;hostname&gt;.local</small>
        </div>

        <!-- Device Restriction Option -->
        <div class="form-group" style="display: flex; align-items: flex-start; gap: 10px; margin: 14px 0 6px 0;">
          <input type="checkbox" id="auth_required" name="auth_required" value="1" %AUTH_CHECKED% style="width: 20px; height: 20px; margin-top: 2px;">
          <div>
            <label for="auth_required" style="cursor: pointer; display: block; font-weight: 600; color: #fff;">Restrict remote control to approved devices only</label>
            <small style="color: var(--text-muted); font-size: 0.75rem; display: block; line-height: 1.3; margin-top: 2px;">Requires clicking the physical button on the TV stick to approve new phones.</small>
          </div>
        </div>

        <button type="submit" class="btn-submit">Save &amp; Connect</button>
      </form>

      <p class="note">
        Your Wi-Fi password will be encrypted using the ESP32-S3 Hardware HMAC key before being committed to flash memory.
      </p>

      %BACK_LINK%
    </div>
  </div>

  <script>
    function togglePassword() {
      const pwd = document.getElementById('password');
      const btn = event.target;
      if (pwd.type === 'password') {
        pwd.type = 'text';
        btn.innerText = 'Hide';
      } else {
        pwd.type = 'password';
        btn.innerText = 'Show';
      }
    }

    function toggleMode() {
      const mode = document.getElementById('op_mode').value;
      const staSec = document.getElementById('staSection');
      const apSec = document.getElementById('apSection');
      if (mode === 'ap') {
        staSec.style.display = 'none';
        apSec.style.display = 'block';
      } else {
        staSec.style.display = 'block';
        apSec.style.display = 'none';
      }
    }
    toggleMode();
  </script>
</body>
</html>
)rawliteral";

const char PAGE_SAVED[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Credentials Saved</title>
  <style>
    body {
      background: #10131a;
      color: #f3f4f6;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
      margin: 0;
      padding: 20px;
      text-align: center;
    }
    .card {
      background: #181d28;
      border: 1px solid #263043;
      border-radius: 20px;
      padding: 30px;
      max-width: 380px;
      box-shadow: 0 10px 30px rgba(0,0,0,0.5);
    }
    .check {
      font-size: 3rem;
      color: #10b981;
      margin-bottom: 16px;
    }
    h1 {
      font-size: 1.3rem;
      margin-bottom: 12px;
    }
    p {
      font-size: 0.9rem;
      color: #9ca3af;
      line-height: 1.5;
      margin-bottom: 20px;
    }
  </style>
</head>
<body>
  <div class="card">
    <div class="check">&#x2714;</div>
    <h1>Credentials Encrypted &amp; Saved</h1>
    <p>
      The dongle is restarting now to apply your configuration.<br><br>
      Once connected, open <strong>http://%MDNS_HOSTNAME%.local</strong> on any device on your Wi-Fi network!
    </p>
  </div>
</body>
</html>
)rawliteral";
