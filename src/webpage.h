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
    }
    .tabs-container {

      display: flex;
      gap: 8px;
      overflow-x: auto;
      padding: 4px 2px;
      scrollbar-width: none;
    }
    .tabs-container::-webkit-scrollbar { display: none; }
    .tab-btn {
      background: var(--card-bg);
      border: 1px solid var(--card-border);
      border-radius: 20px;
      color: var(--text-muted);
      font-size: 0.82rem;
      font-weight: 600;
      padding: 6px 14px;
      white-space: nowrap;
      cursor: pointer;
      transition: all 0.15s ease;
    }
    .tab-btn.active {
      background: var(--primary);
      color: #fff;
      border-color: var(--primary);
      box-shadow: 0 0 10px rgba(59, 130, 246, 0.4);
    }
    .dynamic-grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 8px;
    }
    .dyn-btn {
      background: var(--dpad-bg);
      border: 1px solid var(--card-border);
      border-radius: 12px;
      color: #fff;
      font-size: 0.85rem;
      font-weight: 600;
      padding: 10px 4px;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      gap: 4px;
      cursor: pointer;
      min-height: 54px;
      box-shadow: 0 2px 0 rgba(0,0,0,0.3);
      transition: transform 0.08s ease, background 0.1s ease;
    }
    .dyn-btn:active {
      transform: translateY(2px);
      box-shadow: none;
      filter: brightness(1.2);
    }
    .btn-icon {
      font-size: 1.25rem;
      line-height: 1;
    }
    .btn-label {
      font-size: 0.72rem;
      font-weight: 600;
      letter-spacing: 0.2px;
      text-align: center;
      word-break: break-word;
    }
    .spacer {
      visibility: hidden;
      min-height: 54px;
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

    <!-- Multi-Page Tabs -->
    <div class="tabs-container" id="pageTabs"></div>

    <!-- Dynamic Button Grid -->
    <div class="card" style="padding: 12px;">
      <div class="dynamic-grid" id="buttonGrid"></div>
    </div>

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

    const activeProfile = %ACTIVE_PROFILE_JSON%;
    let currentPageIndex = 0;

    function renderProfile() {
      if (!activeProfile || !activeProfile.pages || activeProfile.pages.length === 0) return;

      const brandSpan = document.querySelector('.brand span');
      if (brandSpan && activeProfile.name) brandSpan.innerText = activeProfile.name;

      const tabs = document.getElementById('pageTabs');
      tabs.innerHTML = '';
      if (activeProfile.pages.length > 1) {
        tabs.style.display = 'flex';
        activeProfile.pages.forEach((p, idx) => {
          const btn = document.createElement('button');
          btn.className = 'tab-btn' + (idx === currentPageIndex ? ' active' : '');
          btn.innerText = p.name || `Page ${idx + 1}`;
          btn.onclick = () => {
            currentPageIndex = idx;
            renderProfile();
          };
          tabs.appendChild(btn);
        });
      } else {
        tabs.style.display = 'none';
      }

      const page = activeProfile.pages[currentPageIndex] || activeProfile.pages[0];
      const grid = document.getElementById('buttonGrid');
      const cols = page.columns || activeProfile.columns || 3;
      grid.style.gridTemplateColumns = `repeat(${cols}, 1fr)`;
      grid.innerHTML = '';

      (page.buttons || []).forEach(b => {
        if (b.action === 'none') {
          const sp = document.createElement('div');
          sp.className = 'spacer';
          if (b.span) sp.style.gridColumn = `span ${b.span}`;
          grid.appendChild(sp);
          return;
        }

        const el = document.createElement('button');
        el.className = 'dyn-btn';
        if (b.span) el.style.gridColumn = `span ${b.span}`;
        if (b.color) el.style.background = b.color;

        let content = '';
        if (b.icon) content += `<span class="btn-icon">${renderIcon(b.icon)}</span>`;
        if (b.label) content += `<span class="btn-label">${b.label}</span>`;
        el.innerHTML = content || '&bull;';

        el.onclick = () => {
          vibrate();
          if (b.action === 'hid') {
            sendKey(b.code);
          } else if (b.action === 'macro') {
            sendMacro(b.macro);
          }
        };
        grid.appendChild(el);
      });
    }

    function renderIcon(icon) {
      const map = {
        'power': '&#x23FB;',
        'volume-x': '&#x1F507;',
        'volume-1': '&#x1F508;',
        'volume-2': '&#x1F50A;',
        'play': '&#x25B6;',
        'pause': '&#x23F8;',
        'skip-back': '&#x23EE;',
        'skip-forward': '&#x23ED;',
        'arrow-up': '&#x25B2;',
        'arrow-down': '&#x25BC;',
        'arrow-left': '&#x25C0;',
        'arrow-right': '&#x25B6;',
        'check': '&#x2714;',
        'corner-down-left': '&#x23CE;',
        'home': '&#x1F3E0;',
        'info': '&#x2139;',
        'lock': '&#x1F512;',
        'monitor': '&#x1F5A5;',
        'terminal': '&#x25B6;_',
        'activity': '&#x1F4CA;',
        'film': '&#x1F3AC;',
        'video': '&#x1F3A6;',
        'search': '&#x1F50D;',
        'maximize': '&#x26F6;',
        'x': '&#x2715;',
        'layers': '&#x2398;',
        'x-square': '&#x2327;'
      };
      return map[icon] || icon;
    }

    function sendMacro(script) {
      vibrate();
      sendPayload('M:' + script);
      setToast('Running Macro...');
    }

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

    // Initialize layout
    renderProfile();
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

      <!-- Device Profiles & Macro Management -->
      <div style="border-top: 1px solid var(--card-border); padding-top: 16px; margin-top: 16px;">
        <h3 style="font-size: 1.05rem; color: var(--primary); margin-bottom: 12px; display: flex; align-items: center; gap: 8px;">
          <span>&#x1F4F1;</span> Device Profile &amp; Layout
        </h3>
        <div class="form-group">
          <label for="active_profile">Active Remote Profile</label>
          <select id="active_profile" name="active_profile">
            %PROFILE_OPTIONS%
          </select>
        </div>
        <div style="display: flex; gap: 8px; margin-bottom: 14px;">
          <button type="button" class="btn-submit" onclick="switchActiveProfile()" style="background: var(--primary); padding: 10px; margin-top: 0;">Switch Active</button>
          <button type="button" class="btn-submit" onclick="deleteActiveProfile()" style="background: #ef4444; padding: 10px; width: auto; margin-top: 0;">Delete</button>
        </div>

        <div class="form-group" style="margin-top: 12px;">
          <label for="profileUpload">Upload Custom Profile (.json)</label>
          <input type="file" id="profileUpload" accept=".json" style="font-size: 0.85rem; margin-bottom: 8px;">
          <button type="button" class="btn-submit" onclick="uploadProfileFile()" style="background: var(--accent); padding: 10px; margin-top: 0;">Upload Profile</button>
        </div>

        <div style="text-align: center; margin-top: 14px;">
          <a href="https://sblaisdev.github.io/t-dongle-s3-tv-remote/" target="_blank" rel="noopener noreferrer" style="color: #38bdf8; font-size: 0.9rem; font-weight: 600; text-decoration: none;">&#x2728; Open Layout &amp; Macro Designer (GitHub Pages)</a>
        </div>
      </div>

      <!-- Firmware & Over-The-Air Updates -->
      <div style="border-top: 1px solid var(--card-border); padding-top: 16px; margin-top: 16px;">
        <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 12px;">
          <h3 style="font-size: 1.05rem; color: var(--primary); margin: 0; display: flex; align-items: center; gap: 8px;">
            <span>&#x1F680;</span> Firmware &amp; Updates
          </h3>
          <span class="badge" style="background: rgba(59, 130, 246, 0.2); color: #60a5fa; border: 1px solid rgba(59, 130, 246, 0.4); margin: 0;">%FIRMWARE_VERSION%</span>
        </div>

        <!-- Test Device Toggle -->
        <div class="form-group" style="display: flex; align-items: flex-start; gap: 10px; margin: 10px 0 12px 0;">
          <input type="checkbox" id="test_device_mode" onchange="toggleTestDeviceMode()" %TEST_DEVICE_CHECKED% style="width: 18px; height: 18px; margin-top: 2px;">
          <div>
            <label for="test_device_mode" style="cursor: pointer; display: block; font-weight: 600; color: #fff; font-size: 0.85rem;">Test Device Mode (Beta &amp; Pre-releases)</label>
            <small style="color: var(--text-muted); font-size: 0.72rem; display: block; line-height: 1.3;">Shows pre-releases, test builds, and enables custom firmware URL flashing.</small>
          </div>
        </div>

        <div id="testDeviceBadge" style="margin-bottom: 10px; display: %TEST_DEVICE_BADGE_DISPLAY%;">
          <span class="badge" style="background:#8b5cf630;color:#c084fc;border-color:#8b5cf660;margin:0;">&#x1F9EA; Test Device Channel Active</span>
        </div>

        <!-- Check for Updates Button -->
        <button type="button" id="btnCheckUpdates" class="btn-submit" onclick="checkGitHubUpdates()" style="background: #334155; padding: 10px; margin-top: 0; margin-bottom: 10px;">
          &#x1F50D; Check for Updates on GitHub
        </button>

        <!-- Version Selector & Cloud Update (Hidden until checked) -->
        <div id="otaUpdateSection" style="display: none; background: #0f172a; border: 1px solid var(--card-border); border-radius: 12px; padding: 14px; margin-bottom: 12px;">
          <label for="otaVersionSelect" style="display: block; font-weight: 600; font-size: 0.82rem; margin-bottom: 6px; color: var(--text-muted);">Select Firmware Version:</label>
          <select id="otaVersionSelect" onchange="onOtaVersionChange()" style="margin-bottom: 10px;">
          </select>

          <div id="otaNotes" style="font-size: 0.75rem; color: #94a3b8; background: #1e293b; border-radius: 8px; padding: 8px 10px; margin-bottom: 12px; max-height: 80px; overflow-y: auto; white-space: pre-wrap;"></div>

          <button type="button" id="btnInstallOta" class="btn-submit" onclick="installSelectedOta()" style="background: #10b981; padding: 12px; margin: 0; font-weight: bold;">
            &#x26A1; Install Selected Firmware
          </button>
        </div>

        <!-- Custom URL Input (Only visible in test device mode) -->
        <div id="customUrlSection" style="display: %CUSTOM_URL_DISPLAY%; background: #1a1528; border: 1px solid #8b5cf640; border-radius: 12px; padding: 12px; margin-bottom: 12px;">
          <label for="customOtaUrl" style="display: block; font-weight: 600; font-size: 0.8rem; color: #c084fc; margin-bottom: 4px;">&#x1F9EA; Custom Firmware URL (.bin):</label>
          <input type="text" id="customOtaUrl" placeholder="https://example.com/firmware.bin" style="font-size: 0.8rem; margin-bottom: 8px;">
          <button type="button" class="btn-submit" onclick="installCustomOta()" style="background: #8b5cf6; padding: 8px; margin: 0; font-size: 0.8rem;">Install from Custom URL</button>
        </div>

        <!-- OTA Progress Display -->
        <div id="otaProgressCard" style="display: none; background: #0f172a; border: 1px solid #3b82f6; border-radius: 12px; padding: 14px; margin-bottom: 12px; text-align: center;">
          <div id="otaStatusText" style="font-weight: 600; font-size: 0.88rem; color: #60a5fa; margin-bottom: 8px;">Downloading &amp; installing firmware...</div>
          <div style="background: #1e293b; border-radius: 8px; height: 12px; overflow: hidden; margin-bottom: 8px;">
            <div id="otaProgressBar" style="width: 0%; height: 100%; background: #10b981; transition: width 0.3s ease;"></div>
          </div>
          <small style="color: #ef4444; font-size: 0.72rem;">Do not disconnect or unplug the dongle!</small>
        </div>

        <!-- Collapsible Manual Upload Fallback -->
        <details style="margin-top: 10px; font-size: 0.8rem; color: var(--text-muted);">
          <summary style="cursor: pointer; padding: 4px 0; color: #94a3b8;">Manual Offline Firmware Upload (.bin)</summary>
          <form action="/update" method="POST" enctype="multipart/form-data" style="margin-top: 8px;">
            <input type="file" name="update" accept=".bin" required style="font-size: 0.8rem; margin-bottom: 6px;">
            <button type="submit" class="btn-submit" style="background: #475569; padding: 8px; margin: 0; font-size: 0.8rem;">Upload &amp; Flash .bin</button>
          </form>
        </details>
      </div>

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

    async function switchActiveProfile() {
      const sel = document.getElementById('active_profile');
      const id = sel.value;
      if (!id) return;
      const token = localStorage.getItem('tv_remote_token') || '';
      try {
        const params = new URLSearchParams({ id: id, token: token });
        const res = await fetch('/api/profiles/set_active', { method: 'POST', body: params });
        const data = await res.json();
        if (data.status === 'ok') {
          alert('Active profile switched to: ' + id);
          window.location.href = '/';
        } else {
          alert('Failed to switch profile: ' + (data.error || 'Pairing token required. Open remote and pair first.'));
        }
      } catch (e) {
        alert('Network error switching profile: ' + e);
      }
    }

    async function deleteActiveProfile() {
      const sel = document.getElementById('active_profile');
      const id = sel.value;
      if (!id || id === 'default-tv') {
        alert('Cannot delete the built-in default TV profile.');
        return;
      }
      if (!confirm('Are you sure you want to delete profile "' + id + '"?')) return;
      const token = localStorage.getItem('tv_remote_token') || '';
      try {
        const params = new URLSearchParams({ id: id, token: token });
        const res = await fetch('/api/profiles/delete', { method: 'POST', body: params });
        const data = await res.json();
        if (data.status === 'ok') {
          alert('Profile deleted.');
          window.location.reload();
        } else {
          alert('Error: ' + (data.error || 'Unauthorized'));
        }
      } catch (e) {
        alert('Network error: ' + e);
      }
    }

    async function uploadProfileFile() {
      const fileInput = document.getElementById('profileUpload');
      if (!fileInput.files || fileInput.files.length === 0) {
        alert('Please select a .json profile file first.');
        return;
      }
      const file = fileInput.files[0];
      const reader = new FileReader();
      reader.onload = async (e) => {
        try {
          const jsonText = e.target.result;
          JSON.parse(jsonText); // Validate JSON
          const token = localStorage.getItem('tv_remote_token') || '';
          const res = await fetch('/api/profiles/upload?set_active=1&token=' + encodeURIComponent(token), {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: jsonText
          });
          const data = await res.json();
          if (data.status === 'ok') {
            alert('Profile uploaded successfully and set as active!');
            window.location.href = '/';
          } else {
            alert('Upload error: ' + (data.error || 'Pairing token required. Open remote and pair first.'));
          }
        } catch (err) {
          alert('Invalid JSON file: ' + err.message);
        }
      };
      reader.readAsText(file);
    }

    let availableReleases = [];
    const currentVersion = "%FIRMWARE_VERSION%";
    const githubRepo = "%GITHUB_REPO%";

    async function toggleTestDeviceMode() {
      const chk = document.getElementById('test_device_mode');
      const enabled = chk.checked;
      try {
        const res = await fetch('/api/ota/set_test_mode?enabled=' + (enabled ? '1' : '0'), { method: 'POST' });
        const data = await res.json();
        if (data.status === 'ok') {
          document.getElementById('testDeviceBadge').style.display = enabled ? 'block' : 'none';
          document.getElementById('customUrlSection').style.display = enabled ? 'block' : 'none';
          if (availableReleases.length > 0) {
            populateReleaseDropdown();
          }
        }
      } catch (e) {
        alert('Failed to update test mode: ' + e);
      }
    }

    async function checkGitHubUpdates() {
      const btn = document.getElementById('btnCheckUpdates');
      btn.innerText = 'Checking GitHub...';
      btn.disabled = true;

      try {
        const res = await fetch('https://api.github.com/repos/' + githubRepo + '/releases');
        if (!res.ok) throw new Error('GitHub API response ' + res.status);
        const releases = await res.json();

        availableReleases = releases.filter(r => !r.draft);
        if (availableReleases.length === 0) {
          alert('No published releases found on GitHub repository: ' + githubRepo);
          btn.innerText = '\uD83D\uDD0D Check for Updates on GitHub';
          btn.disabled = false;
          return;
        }

        populateReleaseDropdown();
        document.getElementById('otaUpdateSection').style.display = 'block';
        btn.innerText = '\u2714 Updates Loaded';
      } catch (err) {
        alert('Could not check GitHub releases: ' + err.message + '\nEnsure your device has Internet access.');
        btn.innerText = '\uD83D\uDD0D Check for Updates on GitHub';
        btn.disabled = false;
      }
    }

    function populateReleaseDropdown() {
      const isTestDevice = document.getElementById('test_device_mode').checked;
      const filtered = availableReleases.filter(r => isTestDevice || !r.prerelease);
      const sel = document.getElementById('otaVersionSelect');
      sel.innerHTML = '';

      if (filtered.length === 0) {
        sel.innerHTML = '<option value="">No matching releases found</option>';
        return;
      }

      filtered.forEach((r, idx) => {
        let label = r.tag_name;
        if (idx === 0 && !r.prerelease) label += ' (Latest) — Recommended';
        else if (idx === 0 && r.prerelease) label += ' \uD83E\uDDEA (Latest Pre-Release)';
        else if (r.prerelease) label += ' \uD83E\uDDEA (Pre-Release)';

        if (r.tag_name === currentVersion) label += ' [Currently Installed]';

        const opt = document.createElement('option');
        opt.value = idx;
        opt.innerText = label;
        sel.appendChild(opt);
      });

      sel.selectedIndex = 0;
      onOtaVersionChange();
    }

    function onOtaVersionChange() {
      const isTestDevice = document.getElementById('test_device_mode').checked;
      const filtered = availableReleases.filter(r => isTestDevice || !r.prerelease);
      const sel = document.getElementById('otaVersionSelect');
      const idx = parseInt(sel.value, 10);
      const r = filtered[idx];
      if (!r) return;

      const dateStr = new Date(r.published_at || r.created_at).toLocaleDateString();
      const notesEl = document.getElementById('otaNotes');
      notesEl.innerText = 'Released: ' + dateStr + '\n' + (r.body || 'No release description provided.');

      const btn = document.getElementById('btnInstallOta');
      btn.innerText = '\u26A1 Install ' + r.tag_name;
    }

    async function installSelectedOta() {
      const isTestDevice = document.getElementById('test_device_mode').checked;
      const filtered = availableReleases.filter(r => isTestDevice || !r.prerelease);
      const sel = document.getElementById('otaVersionSelect');
      const idx = parseInt(sel.value, 10);
      const r = filtered[idx];
      if (!r) return;

      let binUrl = '';
      if (r.assets && r.assets.length > 0) {
        const binAsset = r.assets.find(a => a.name.endsWith('.bin'));
        if (binAsset) binUrl = binAsset.browser_download_url;
      }
      if (!binUrl) {
        binUrl = 'https://github.com/' + githubRepo + '/releases/download/' + r.tag_name + '/firmware.bin';
      }

      if (!confirm('Are you sure you want to install firmware ' + r.tag_name + '? The dongle will download and restart automatically.')) {
        return;
      }

      startOtaExecution(binUrl, r.tag_name);
    }

    async function installCustomOta() {
      const url = document.getElementById('customOtaUrl').value.trim();
      if (!url) {
        alert('Please enter a valid firmware .bin URL.');
        return;
      }
      if (!confirm('Install firmware from custom URL?\n' + url)) return;
      startOtaExecution(url, 'Custom URL');
    }

    async function startOtaExecution(url, versionTag) {
      document.getElementById('otaUpdateSection').style.display = 'none';
      const progCard = document.getElementById('otaProgressCard');
      progCard.style.display = 'block';
      const statusText = document.getElementById('otaStatusText');
      const bar = document.getElementById('otaProgressBar');

      statusText.innerText = 'Starting installation of ' + versionTag + '...';
      bar.style.width = '15%';

      const token = localStorage.getItem('tv_remote_token') || '';
      try {
        const params = new URLSearchParams({ url: url, version: versionTag, token: token });
        fetch('/api/ota/cloud_update', { method: 'POST', body: params });

        let progress = 20;
        const simTimer = setInterval(() => {
          progress = Math.min(progress + 5, 90);
          bar.style.width = progress + '%';
          statusText.innerText = 'Dongle downloading & flashing ' + versionTag + '... (' + progress + '%)';
        }, 1200);

        setTimeout(() => {
          clearInterval(simTimer);
          bar.style.width = '100%';
          statusText.innerText = 'Rebooting into new firmware! Reconnecting...';
          setTimeout(() => { window.location.href = '/'; }, 6000);
        }, 18000);

      } catch (err) {
        statusText.innerText = 'Error initiating update: ' + err.message;
      }
    }
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
