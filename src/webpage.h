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
      --text: #f3f4f6;
      --text-muted: #9ca3af;
      --btn-bg: #222a3a;
      --btn-active: #323e57;
      --pulse-glow: rgba(16, 185, 129, 0.4);
    }
    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
      -webkit-tap-highlight-color: transparent;
      user-select: none;
    }
    body {
      background: var(--bg);
      color: var(--text);
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 12px 14px;
      min-height: 100vh;
    }
    .container {
      width: 100%;
      max-width: 420px;
      display: flex;
      flex-direction: column;
      gap: 12px;
    }
    header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 4px 2px;
    }
    .brand {
      font-size: 1.05rem;
      font-weight: 700;
      display: flex;
      align-items: center;
      gap: 8px;
    }
    .brand-icon {
      width: 24px;
      height: 24px;
    }
    .status-dot {
      width: 10px;
      height: 10px;
      border-radius: 50%;
      background-color: #eab308;
      transition: background-color 0.3s, box-shadow 0.3s;
    }
    .status-dot.connected {
      background-color: var(--accent);
      box-shadow: 0 0 10px var(--accent);
    }
    .header-links {
      display: flex;
      align-items: center;
      gap: 8px;
    }
    .badge {
      font-size: 0.75rem;
      background: #25334d;
      color: #93c5fd;
      padding: 4px 10px;
      border-radius: 9999px;
      border: 1px solid #3b82f640;
    }
    .config-link {
      color: var(--text-muted);
      text-decoration: none;
      font-size: 1.1rem;
      padding: 4px;
    }
    .card {
      background: var(--card-bg);
      border: 1px solid var(--card-border);
      border-radius: 16px;
      padding: 14px;
      box-shadow: 0 4px 20px rgba(0,0,0,0.25);
    }
    /* Live Typing Console */
    .typing-card {
      border: 1.5px solid #3b82f660;
      background: #141822;
      position: relative;
    }
    .typing-header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      font-size: 0.8rem;
      color: var(--text-muted);
      margin-bottom: 8px;
    }
    .live-indicator {
      display: flex;
      align-items: center;
      gap: 6px;
      color: var(--accent);
      font-weight: 600;
      font-size: 0.75rem;
      text-transform: uppercase;
      letter-spacing: 0.5px;
    }
    .live-pulse {
      width: 7px;
      height: 7px;
      border-radius: 50%;
      background: var(--accent);
      box-shadow: 0 0 8px var(--pulse-glow);
    }
    .input-wrapper {
      position: relative;
      display: flex;
      align-items: center;
    }
    input[type="text"] {
      width: 100%;
      padding: 14px 40px 14px 14px;
      font-size: 1.1rem;
      color: #fff;
      background: #0d1017;
      border: 1px solid var(--card-border);
      border-radius: 12px;
      outline: none;
      user-select: text;
      transition: border-color 0.2s, box-shadow 0.2s;
    }
    input[type="text"]:focus {
      border-color: var(--primary);
      box-shadow: 0 0 0 2px rgba(59, 130, 246, 0.35);
    }
    .clear-btn {
      position: absolute;
      right: 10px;
      background: none;
      border: none;
      color: var(--text-muted);
      font-size: 1.3rem;
      cursor: pointer;
      display: none;
      padding: 4px;
    }
    /* D-Pad Section */
    .dpad-container {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      grid-template-rows: repeat(3, 62px);
      gap: 8px;
      max-width: 250px;
      margin: 4px auto;
    }
    button {
      background: var(--btn-bg);
      border: 1px solid var(--card-border);
      color: var(--text);
      font-size: 0.95rem;
      font-weight: 600;
      border-radius: 12px;
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      transition: background 0.08s, transform 0.05s;
      touch-action: manipulation;
    }
    button:active {
      background: var(--btn-active);
      transform: scale(0.95);
    }
    .dpad-btn {
      font-size: 1.4rem;
      font-weight: bold;
      border-radius: 14px;
    }
    .dpad-center {
      background: var(--primary);
      border-color: var(--primary);
      color: #fff;
      font-size: 1.1rem;
    }
    .dpad-center:active {
      background: var(--primary-hover);
    }
    /* Navigation & Media Grids */
    .keys-grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 8px;
    }
    .keys-grid button {
      padding: 12px 6px;
      font-size: 0.9rem;
    }
    .media-grid {
      display: grid;
      grid-template-columns: repeat(4, 1fr);
      gap: 8px;
    }
    .media-grid button {
      padding: 10px 4px;
      font-size: 0.85rem;
    }
    .feedback-toast {
      font-size: 0.8rem;
      color: var(--text-muted);
      text-align: center;
      min-height: 18px;
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
        <span id="charCount" style="font-size: 0.75rem;">Keystrokes sent live</span>
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

  <script>
    const input = document.getElementById('liveInput');
    const clearBtn = document.getElementById('clearBtn');
    const toast = document.getElementById('toast');
    const connStatus = document.getElementById('connStatus');

    let previousVal = '';
    let ws = null;

    function connectWS() {
      const loc = window.location;
      const wsUri = (loc.protocol === 'https:' ? 'wss:' : 'ws:') + '//' + loc.hostname + ':81/';
      ws = new WebSocket(wsUri);

      ws.onopen = () => {
        connStatus.innerText = 'Connected (Live)';
        setToast('Connected! Ready to send commands');
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

    function sendPayload(msg) {
      if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(msg);
      } else {
        if (msg.startsWith('K:')) {
          fetch('/sendkey', { method: 'POST', body: new URLSearchParams({ key: msg.slice(2) }) });
        } else if (msg.startsWith('T:')) {
          fetch('/sendtext', { method: 'POST', body: new URLSearchParams({ text: msg.slice(2) }) });
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
  <title>Wi-Fi Setup - %ROOM_NAME% TV</title>
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
      margin-bottom: 12px;
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
      margin-top: 10px;
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
      <span class="badge">&#x1F512; Hardware eFuse HMAC Secured</span>
      
      <form action="/savewifi" method="POST">
        <div class="form-group">
          <label for="ssid">Select Wi-Fi Network</label>
          <select id="ssid" name="ssid" required>
            %WIFI_OPTIONS%
          </select>
        </div>

        <div class="form-group">
          <label for="manual_ssid">Or enter SSID manually</label>
          <input type="text" id="manual_ssid" name="manual_ssid" placeholder="Network Name (if hidden)">
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

        <button type="submit" class="btn-submit">Save &amp; Connect</button>
      </form>

      <p class="note">
        Your Wi-Fi password will be encrypted using the ESP32-S3 Hardware HMAC key before being committed to non-volatile flash memory.
      </p>

      <a href="/" class="back-link">&#x2190; Back to Remote Control</a>
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
      The dongle is restarting now to connect to your Wi-Fi.<br><br>
      Once connected, open <strong>http://%MDNS_HOSTNAME%.local</strong> on any device on your Wi-Fi network!
    </p>
  </div>
</body>
</html>
)rawliteral";
