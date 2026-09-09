#pragma once
#include <Arduino.h>

const char PAGE_DESIGNER[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>LilyGO TV Remote &amp; Macro Studio</title>
  <style>:root {
  --bg-dark: #0b0f17;
  --panel-bg: #121824;
  --panel-border: #1e293b;
  --accent-primary: #3b82f6;
  --accent-primary-hover: #2563eb;
  --accent-green: #10b981;
  --accent-red: #ef4444;
  --accent-amber: #f59e0b;
  --accent-purple: #8b5cf6;
  --text-main: #f8fafc;
  --text-muted: #94a3b8;
  --btn-bg: #1e293b;
  --btn-hover: #334155;
  --phone-bg: #0f172a;
}

* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
  font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
}

body {
  background: var(--bg-dark);
  color: var(--text-main);
  height: 100vh;
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

/* App Header */
header {
  background: var(--panel-bg);
  border-bottom: 1px solid var(--panel-border);
  padding: 10px 20px;
  display: flex;
  justify-content: space-between;
  align-items: center;
  z-index: 10;
}

.brand {
  display: flex;
  align-items: center;
  gap: 12px;
}

.brand-icon {
  width: 32px;
  height: 32px;
  border-radius: 8px;
  background: linear-gradient(135deg, #3b82f6, #1d4ed8);
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 1.2rem;
}

.brand h1 {
  font-size: 1.1rem;
  font-weight: 700;
  letter-spacing: -0.5px;
}

.header-controls {
  display: flex;
  align-items: center;
  gap: 12px;
}

.connection-bar {
  display: flex;
  align-items: center;
  gap: 8px;
  background: #0f172a;
  border: 1px solid var(--panel-border);
  padding: 4px 10px;
  border-radius: 20px;
}

.connection-bar input {
  background: none;
  border: none;
  color: var(--text-main);
  font-size: 0.85rem;
  width: 170px;
  outline: none;
}

.status-dot {
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background: var(--accent-red);
}

.status-dot.connected {
  background: var(--accent-green);
  box-shadow: 0 0 8px var(--accent-green);
}

button {
  background: var(--btn-bg);
  border: 1px solid var(--panel-border);
  color: var(--text-main);
  padding: 8px 14px;
  border-radius: 8px;
  font-size: 0.85rem;
  font-weight: 600;
  cursor: pointer;
  display: inline-flex;
  align-items: center;
  gap: 6px;
  transition: all 0.15s ease;
}

button:hover {
  background: var(--btn-hover);
}

button.primary {
  background: var(--accent-primary);
  border-color: var(--accent-primary);
}

button.primary:hover {
  background: var(--accent-primary-hover);
}

button.success {
  background: var(--accent-green);
  border-color: var(--accent-green);
}

button.danger {
  background: var(--accent-red);
  border-color: var(--accent-red);
}

/* App Main Layout (3-Column Studio) */
.studio-container {
  display: flex;
  flex: 1;
  overflow: hidden;
}

/* Left Sidebar: Pages & Templates */
.sidebar-left {
  width: 260px;
  background: var(--panel-bg);
  border-right: 1px solid var(--panel-border);
  display: flex;
  flex-direction: column;
  padding: 16px;
  gap: 16px;
  overflow-y: auto;
}

.section-title {
  font-size: 0.75rem;
  text-transform: uppercase;
  letter-spacing: 0.8px;
  color: var(--text-muted);
  font-weight: 700;
  margin-bottom: 8px;
}

.template-picker select, .form-input, .form-select {
  width: 100%;
  background: #0f172a;
  border: 1px solid var(--panel-border);
  color: var(--text-main);
  padding: 8px 12px;
  border-radius: 8px;
  font-size: 0.88rem;
  outline: none;
}

.page-list {
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.page-item {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 8px 12px;
  background: #0f172a;
  border: 1px solid var(--panel-border);
  border-radius: 8px;
  cursor: pointer;
  font-size: 0.85rem;
}

.page-item.active {
  border-color: var(--accent-primary);
  background: rgba(59, 130, 246, 0.1);
  color: #fff;
  font-weight: 600;
}

/* Center Stage: Phone Simulator */
.stage-center {
  flex: 1;
  background: var(--bg-dark);
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  padding: 20px;
  position: relative;
  overflow-y: auto;
}

.stage-toolbar {
  position: absolute;
  top: 16px;
  display: flex;
  gap: 10px;
  background: rgba(18, 24, 36, 0.8);
  backdrop-filter: blur(8px);
  padding: 6px 12px;
  border-radius: 20px;
  border: 1px solid var(--panel-border);
  z-index: 5;
}

.phone-frame {
  width: 380px;
  height: 680px;
  background: var(--phone-bg);
  border: 10px solid #1e293b;
  border-radius: 40px;
  box-shadow: 0 25px 60px rgba(0,0,0,0.6);
  display: flex;
  flex-direction: column;
  overflow: hidden;
  position: relative;
}

.phone-notch {
  width: 120px;
  height: 18px;
  background: #1e293b;
  border-radius: 0 0 12px 12px;
  margin: 0 auto;
}

.phone-screen {
  flex: 1;
  padding: 14px;
  display: flex;
  flex-direction: column;
  gap: 12px;
  overflow-y: auto;
}

.phone-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 4px 0;
  color: var(--text-muted);
  font-size: 0.8rem;
}

.tab-bar-preview {
  display: flex;
  gap: 6px;
  overflow-x: auto;
  padding-bottom: 2px;
}

.tab-pill {
  padding: 4px 12px;
  border-radius: 16px;
  font-size: 0.75rem;
  font-weight: 600;
  background: #1e293b;
  color: var(--text-muted);
  white-space: nowrap;
}

.tab-pill.active {
  background: var(--accent-primary);
  color: #fff;
}

/* Grid on Phone */
.sim-grid {
  display: grid;
  gap: 8px;
}

.sim-btn {
  background: #1e293b;
  border: 1px solid var(--panel-border);
  border-radius: 12px;
  color: #fff;
  min-height: 52px;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 3px;
  padding: 8px 4px;
  cursor: pointer;
  position: relative;
  transition: transform 0.08s ease;
}

.sim-btn:hover {
  filter: brightness(1.15);
}

.sim-btn:active {
  transform: scale(0.96);
}

.sim-btn.selected {
  outline: 2px solid var(--accent-primary);
  box-shadow: 0 0 12px rgba(59, 130, 246, 0.5);
}

.sim-btn-icon {
  font-size: 1.25rem;
  line-height: 1;
}

.sim-btn-label {
  font-size: 0.72rem;
  font-weight: 600;
  text-align: center;
  word-break: break-word;
}

.sim-spacer {
  border: 1px dashed #334155;
  border-radius: 12px;
  min-height: 52px;
  display: flex;
  align-items: center;
  justify-content: center;
  color: #475569;
  font-size: 0.7rem;
  cursor: pointer;
}

.sim-spacer.selected {
  border-color: var(--accent-primary);
}

/* Right Sidebar: Inspector & Macro Builder */
.sidebar-right {
  width: 360px;
  background: var(--panel-bg);
  border-left: 1px solid var(--panel-border);
  display: flex;
  flex-direction: column;
  padding: 16px;
  gap: 14px;
  overflow-y: auto;
}

.form-group {
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.form-group label {
  font-size: 0.8rem;
  font-weight: 600;
  color: var(--text-muted);
}

.palette-row {
  display: flex;
  gap: 6px;
  flex-wrap: wrap;
}

.color-swatch {
  width: 26px;
  height: 26px;
  border-radius: 6px;
  cursor: pointer;
  border: 2px solid transparent;
}

.color-swatch.active {
  border-color: #fff;
}

/* Macro Tabs & Steps */
.macro-tabs {
  display: flex;
  border-bottom: 1px solid var(--panel-border);
  margin-bottom: 10px;
}

.macro-tab-btn {
  background: none;
  border: none;
  border-bottom: 2px solid transparent;
  color: var(--text-muted);
  border-radius: 0;
  padding: 8px 12px;
}

.macro-tab-btn.active {
  color: var(--accent-primary);
  border-bottom-color: var(--accent-primary);
}

.step-item {
  background: #0f172a;
  border: 1px solid var(--panel-border);
  border-radius: 8px;
  padding: 8px 10px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
  margin-bottom: 6px;
}

.raw-editor {
  width: 100%;
  height: 180px;
  background: #0f172a;
  border: 1px solid var(--panel-border);
  border-radius: 8px;
  color: #38bdf8;
  font-family: monospace;
  font-size: 0.85rem;
  padding: 10px;
  outline: none;
  resize: vertical;
}

/* Modal Overlay */
.modal {
  position: fixed;
  top: 0; left: 0; right: 0; bottom: 0;
  background: rgba(0,0,0,0.7);
  backdrop-filter: blur(4px);
  display: none;
  align-items: center;
  justify-content: center;
  z-index: 100;
}

.modal-content {
  background: var(--panel-bg);
  border: 1px solid var(--panel-border);
  border-radius: 16px;
  padding: 24px;
  max-width: 440px;
  width: 100%;
  box-shadow: 0 20px 40px rgba(0,0,0,0.6);
  text-align: center;
}
</style>
</head>
<body>

  <!-- Header -->
  <header>
    <div class="brand">
      <div class="brand-icon">&#x1F4F1;</div>
      <div>
        <h1>LilyGO Remote Studio</h1>
        <small style="color: var(--text-muted); font-size: 0.72rem;">Universal Layout &amp; DuckyScript Macro Designer</small>
      </div>
    </div>

    <div class="header-controls">
      <div class="connection-bar">
        <span class="status-dot" id="connDot"></span>
        <input type="text" id="targetHost" value="http://tv-remote.local" placeholder="Target IP or mDNS">
        <button id="btnConnect" onclick="checkConnection()" style="padding: 4px 8px; font-size: 0.75rem;">Connect</button>
      </div>

      <button id="btnPair" onclick="openPairModal()" style="font-size: 0.8rem;">&#x1F512; Pair Dongle</button>
      <button onclick="importProfile()" style="font-size: 0.8rem;">&#x1F4C2; Import JSON</button>
      <button onclick="exportProfile()" style="font-size: 0.8rem;">&#x1F4BE; Export JSON</button>
      <button class="primary" onclick="deployToDongle()" style="font-size: 0.8rem;">&#x1F680; Deploy to LilyGO</button>
    </div>
  </header>

  <!-- Studio Main -->
  <div class="studio-container">

    <!-- Left Sidebar -->
    <aside class="sidebar-left">
      <div class="template-picker">
        <div class="section-title">Load Template</div>
        <select id="templateSelect" onchange="loadSelectedTemplate()">
          <option value="">-- Choose a Preset Template --</option>
          <option value="tv">Smart TV Remote</option>
          <option value="pc">Windows PC &amp; Media Center</option>
          <option value="mac">macOS Media &amp; Shortcuts</option>
          <option value="android">Android / Google TV</option>
          <option value="kodi">Kodi / Home Theater PC</option>
          <option value="sysadmin">SysAdmin / Ducky Toolkit</option>
          <option value="blank">Blank Layout (Custom)</option>
        </select>
      </div>

      <hr style="border: none; border-top: 1px solid var(--panel-border);">

      <div class="form-group">
        <div class="section-title">Profile Settings</div>
        <label for="profileId">Profile ID (alphanumeric)</label>
        <input type="text" id="profileId" class="form-input" value="custom-remote">
      </div>

      <div class="form-group">
        <label for="profileName">Profile Name</label>
        <input type="text" id="profileName" class="form-input" value="Custom Remote" oninput="updateProfileMeta()">
      </div>

      <div class="form-group">
        <label for="profileDevice">Target Device</label>
        <select id="profileDevice" class="form-select" onchange="updateProfileMeta()">
          <option value="tv">Smart TV</option>
          <option value="pc">Windows PC</option>
          <option value="mac">Apple Mac</option>
          <option value="android">Android TV</option>
          <option value="other">Universal Device</option>
        </select>
      </div>

      <hr style="border: none; border-top: 1px solid var(--panel-border);">

      <div>
        <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px;">
          <span class="section-title" style="margin: 0;">Pages</span>
          <button onclick="addNewPage()" style="padding: 2px 8px; font-size: 0.75rem;">+ Add Page</button>
        </div>
        <div class="page-list" id="pagesList"></div>
      </div>

      <div class="form-group" style="margin-top: 6px;">
        <label for="pageCols">Grid Columns for This Page</label>
        <select id="pageCols" class="form-select" onchange="updatePageCols()">
          <option value="2">2 Columns</option>
          <option value="3" selected>3 Columns (Default)</option>
          <option value="4">4 Columns</option>
          <option value="5">5 Columns</option>
          <option value="6">6 Columns</option>
        </select>
      </div>
    </aside>

    <!-- Center Stage: Phone Simulator -->
    <main class="stage-center">
      <div class="stage-toolbar">
        <button onclick="addButtonToPage()">+ Add Button</button>
        <button onclick="addSpacerToPage()">+ Add Spacer</button>
        <label style="display: flex; align-items: center; gap: 6px; font-size: 0.8rem; font-weight: 600; cursor: pointer; margin-left: 10px;">
          <input type="checkbox" id="liveTestToggle">
          <span>&#x26A1; Live Test on Dongle</span>
        </label>
      </div>

      <!-- Phone Simulator Frame -->
      <div class="phone-frame">
        <div class="phone-notch"></div>
        <div class="phone-screen">
          <div class="phone-header">
            <span id="simRoom">Living Room TV</span>
            <span style="color: var(--accent-green);">&#x1F512; Encrypted</span>
          </div>

          <!-- Page Tabs Bar -->
          <div class="tab-bar-preview" id="simTabs"></div>

          <!-- Grid Preview -->
          <div class="sim-grid" id="simGrid"></div>
        </div>
      </div>
    </main>

    <!-- Right Sidebar: Inspector & Macro Builder -->
    <aside class="sidebar-right">
      <div class="section-title">Button Inspector</div>

      <div id="noSelectionMsg" style="color: var(--text-muted); font-size: 0.85rem; padding: 20px 0; text-align: center;">
        Select any button or spacer on the phone screen to edit its properties and macros.
      </div>

      <div id="inspectorForm" style="display: none; display: flex; flex-direction: column; gap: 14px;">
        
        <div class="form-group">
          <label>Action Type</label>
          <select id="btnAction" class="form-select" onchange="changeBtnAction()">
            <option value="hid">Single HID Key (TV / PC Key)</option>
            <option value="macro">DuckyScript Macro (Script)</option>
            <option value="none">Empty Spacer</option>
          </select>
        </div>

        <div id="btnVisualProps">
          <div class="form-group">
            <label for="btnLabel">Button Label</label>
            <input type="text" id="btnLabel" class="form-input" oninput="updateSelectedBtn()">
          </div>

          <div class="form-group">
            <label for="btnIcon">Icon / Symbol</label>
            <select id="btnIcon" class="form-select" onchange="updateSelectedBtn()">
              <option value="">None (Label only)</option>
              <option value="arrow-up">▲ Up</option>
              <option value="arrow-down">▼ Down</option>
              <option value="arrow-left">◀ Left</option>
              <option value="arrow-right">▶ Right</option>
              <option value="check">✔ Check / OK</option>
              <option value="power">⏻ Power</option>
              <option value="volume-2">🔊 Vol +</option>
              <option value="volume-1">🔉 Vol -</option>
              <option value="volume-x">🔇 Mute</option>
              <option value="play">▶ Play</option>
              <option value="pause">⏸ Pause</option>
              <option value="skip-back">⏮ Prev Track</option>
              <option value="skip-forward">⏭ Next Track</option>
              <option value="home">🏠 Home</option>
              <option value="corner-down-left">⏎ Back / Return</option>
              <option value="lock">🔒 Lock</option>
              <option value="terminal">▶_ Terminal</option>
              <option value="monitor">🖥 Desktop / Monitor</option>
              <option value="activity">📊 Task Manager</option>
              <option value="video">🎥 YouTube / Video</option>
              <option value="film">🎬 Netflix / Film</option>
              <option value="search">🔍 Search</option>
            </select>
          </div>

          <div class="form-group">
            <label for="btnSpan">Grid Span (Width)</label>
            <select id="btnSpan" class="form-select" onchange="updateSelectedBtn()">
              <option value="1">1 Column (Standard)</option>
              <option value="2">2 Columns (Wide)</option>
              <option value="3">3 Columns (Full Width on 3-col)</option>
              <option value="4">4 Columns (Full Width on 4-col)</option>
            </select>
          </div>

          <div class="form-group">
            <label>Color Palette</label>
            <div class="palette-row">
              <div class="color-swatch" style="background: #1e293b;" onclick="setBtnColor('#1e293b')"></div>
              <div class="color-swatch" style="background: #3b82f6;" onclick="setBtnColor('#3b82f6')"></div>
              <div class="color-swatch" style="background: #10b981;" onclick="setBtnColor('#10b981')"></div>
              <div class="color-swatch" style="background: #ef4444;" onclick="setBtnColor('#ef4444')"></div>
              <div class="color-swatch" style="background: #f59e0b;" onclick="setBtnColor('#f59e0b')"></div>
              <div class="color-swatch" style="background: #8b5cf6;" onclick="setBtnColor('#8b5cf6')"></div>
              <div class="color-swatch" style="background: #06b6d4;" onclick="setBtnColor('#06b6d4')"></div>
              <div class="color-swatch" style="background: #dc2626;" onclick="setBtnColor('#dc2626')"></div>
            </div>
          </div>
        </div>

        <!-- HID Single Key Settings -->
        <div id="hidConfig" class="form-group">
          <label for="hidKey">Target HID Key</label>
          <select id="hidKey" class="form-select" onchange="updateSelectedBtn()">
            <optgroup label="D-Pad &amp; Navigation">
              <option value="UP">UP Arrow</option>
              <option value="DOWN">DOWN Arrow</option>
              <option value="LEFT">LEFT Arrow</option>
              <option value="RIGHT">RIGHT Arrow</option>
              <option value="OK">ENTER / OK</option>
              <option value="BACK">BACK / ESC</option>
              <option value="HOME">HOME (GUI / Windows)</option>
              <option value="TAB">TAB</option>
              <option value="SPACE">SPACE</option>
              <option value="BACKSPACE">BACKSPACE</option>
            </optgroup>
            <optgroup label="Consumer Controls">
              <option value="VOL_UP">Volume Up</option>
              <option value="VOL_DOWN">Volume Down</option>
              <option value="MUTE">Mute</option>
              <option value="PLAY_PAUSE">Play / Pause</option>
              <option value="NEXT">Next Track</option>
              <option value="PREV">Previous Track</option>
              <option value="STOP">Stop</option>
              <option value="POWER">Power Toggle</option>
            </optgroup>
            <optgroup label="Function Keys">
              <option value="F1">F1</option>
              <option value="F2">F2</option>
              <option value="F5">F5 (Refresh)</option>
              <option value="F11">F11 (Fullscreen)</option>
              <option value="F12">F12</option>
            </optgroup>
            <optgroup label="Numpad">
              <option value="KEY_1">1</option>
              <option value="KEY_2">2</option>
              <option value="KEY_3">3</option>
              <option value="KEY_4">4</option>
              <option value="KEY_5">5</option>
              <option value="KEY_6">6</option>
              <option value="KEY_7">7</option>
              <option value="KEY_8">8</option>
              <option value="KEY_9">9</option>
              <option value="KEY_0">0</option>
            </optgroup>
          </select>
        </div>

        <!-- DuckyScript Macro Settings -->
        <div id="macroConfig" style="display: none;">
          <div class="macro-tabs">
            <button class="macro-tab-btn active" id="tabBtnVisual" onclick="switchMacroTab('visual')">Visual Builder</button>
            <button class="macro-tab-btn" id="tabBtnRaw" onclick="switchMacroTab('raw')">Raw DuckyScript</button>
          </div>

          <!-- Visual Macro Builder -->
          <div id="macroVisualPanel">
            <div id="macroStepsList"></div>
            <div style="display: flex; gap: 6px; flex-wrap: wrap; margin-top: 8px;">
              <button onclick="addMacroStep('string')" style="font-size: 0.75rem;">+ Type Text</button>
              <button onclick="addMacroStep('combo')" style="font-size: 0.75rem;">+ Key Combo</button>
              <button onclick="addMacroStep('delay')" style="font-size: 0.75rem;">+ Delay (ms)</button>
              <button onclick="addMacroStep('enter')" style="font-size: 0.75rem;">+ Enter</button>
            </div>
          </div>

          <!-- Raw DuckyScript Text Editor -->
          <div id="macroRawPanel" style="display: none;">
            <textarea id="rawDuckyEditor" class="raw-editor" placeholder="DELAY 200&#10;GUI r&#10;DELAY 150&#10;STRING cmd&#10;ENTER" oninput="onRawDuckyInput()"></textarea>
            <small style="color: var(--text-muted); font-size: 0.72rem; display: block; margin-top: 4px;">
              Supports standard DuckyScript syntax: STRING, DELAY, GUI, CTRL, ALT, SHIFT, ENTER, REPEAT.
            </small>
          </div>

          <button onclick="testCurrentMacro()" style="margin-top: 10px; width: 100%; justify-content: center; background: #8b5cf6;">
            &#x26A1; Test Macro Now
          </button>
        </div>

        <!-- Button Actions -->
        <div style="display: flex; gap: 8px; margin-top: 10px; border-top: 1px solid var(--panel-border); padding-top: 12px;">
          <button onclick="moveBtn(-1)" style="flex: 1; justify-content: center;">◀ Move Up</button>
          <button onclick="moveBtn(1)" style="flex: 1; justify-content: center;">Move Down ▶</button>
          <button class="danger" onclick="deleteSelectedBtn()" style="justify-content: center;">Delete</button>
        </div>

      </div>
    </aside>

  </div>

  <!-- Pairing Modal -->
  <div class="modal" id="pairModal">
    <div class="modal-content">
      <div style="font-size: 2.2rem; margin-bottom: 12px;">&#x1F512;</div>
      <h3 style="margin-bottom: 8px;">Pair with LilyGO T-Dongle-S3</h3>
      <p style="color: var(--text-muted); font-size: 0.85rem; margin-bottom: 16px; line-height: 1.4;">
        To manage layouts and inject macros, click <strong>"Request Pairing"</strong> below, then <strong>press the physical button on the TV stick</strong> to approve.
      </p>
      <div id="pairModalStatus" style="color: #38bdf8; font-size: 0.85rem; margin-bottom: 14px;"></div>
      <div style="display: flex; gap: 10px; justify-content: center;">
        <button class="primary" onclick="initiatePairing()">Request Pairing</button>
        <button onclick="closePairModal()">Cancel</button>
      </div>
    </div>
  </div>

  <!-- Hidden File Input for Import -->
  <input type="file" id="fileInput" accept=".json" style="display: none;" onchange="onFileSelected(event)">

  <script>// =========================================================================
// LilyGO TV Remote & Macro Studio - Core Engine
// =========================================================================

// Preset Templates
const TEMPLATES = {
  tv: {
    id: "default-tv",
    name: "Smart TV Remote",
    deviceType: "tv",
    columns: 3,
    pages: [
      {
        id: "nav",
        name: "Navigation",
        columns: 3,
        buttons: [
          { label: "POWER", icon: "power", color: "#ef4444", action: "hid", code: "POWER", span: 1 },
          { label: "", action: "none", span: 1 },
          { label: "MUTE", icon: "volume-x", color: "#64748b", action: "hid", code: "MUTE", span: 1 },
          { label: "", action: "none", span: 1 },
          { label: "▲", icon: "arrow-up", color: "#334155", action: "hid", code: "UP", span: 1 },
          { label: "", action: "none", span: 1 },
          { label: "◄", icon: "arrow-left", color: "#334155", action: "hid", code: "LEFT", span: 1 },
          { label: "OK", icon: "check", color: "#3b82f6", action: "hid", code: "OK", span: 1 },
          { label: "►", icon: "arrow-right", color: "#334155", action: "hid", code: "RIGHT", span: 1 },
          { label: "", action: "none", span: 1 },
          { label: "▼", icon: "arrow-down", color: "#334155", action: "hid", code: "DOWN", span: 1 },
          { label: "", action: "none", span: 1 },
          { label: "BACK", icon: "corner-down-left", color: "#475569", action: "hid", code: "BACK", span: 1 },
          { label: "HOME", icon: "home", color: "#475569", action: "hid", code: "HOME", span: 1 },
          { label: "INFO", icon: "info", color: "#475569", action: "hid", code: "TAB", span: 1 },
          { label: "VOL +", icon: "volume-2", color: "#10b981", action: "hid", code: "VOL_UP", span: 1 },
          { label: "VOL -", icon: "volume-1", color: "#10b981", action: "hid", code: "VOL_DOWN", span: 1 },
          { label: "PLAY", icon: "play", color: "#06b6d4", action: "hid", code: "PLAY_PAUSE", span: 1 },
          { label: "YouTube", icon: "video", color: "#e11d48", action: "macro", macro: "DELAY 100\nSTRING youtube\nENTER", span: 1 },
          { label: "Netflix", icon: "film", color: "#dc2626", action: "macro", macro: "DELAY 100\nSTRING netflix\nENTER", span: 1 },
          { label: "Search", icon: "search", color: "#8b5cf6", action: "hid", code: "ENTER", span: 1 }
        ]
      },
      {
        id: "numpad",
        name: "Numpad",
        columns: 3,
        buttons: [
          { label: "1", action: "hid", code: "KEY_1", span: 1 },
          { label: "2", action: "hid", code: "KEY_2", span: 1 },
          { label: "3", action: "hid", code: "KEY_3", span: 1 },
          { label: "4", action: "hid", code: "KEY_4", span: 1 },
          { label: "5", action: "hid", code: "KEY_5", span: 1 },
          { label: "6", action: "hid", code: "KEY_6", span: 1 },
          { label: "7", action: "hid", code: "KEY_7", span: 1 },
          { label: "8", action: "hid", code: "KEY_8", span: 1 },
          { label: "9", action: "hid", code: "KEY_9", span: 1 },
          { label: "⌫", action: "hid", code: "BACKSPACE", color: "#ef4444", span: 1 },
          { label: "0", action: "hid", code: "KEY_0", span: 1 },
          { label: "ENTER", action: "hid", code: "ENTER", color: "#3b82f6", span: 1 }
        ]
      }
    ]
  },

  pc: {
    id: "pc-media",
    name: "Windows PC & Media",
    deviceType: "pc",
    columns: 3,
    pages: [
      {
        id: "media",
        name: "Media & Navigation",
        columns: 3,
        buttons: [
          { label: "⏮ PREV", icon: "skip-back", color: "#334155", action: "hid", code: "PREV", span: 1 },
          { label: "⏯ PLAY", icon: "play", color: "#06b6d4", action: "hid", code: "PLAY_PAUSE", span: 1 },
          { label: "⏭ NEXT", icon: "skip-forward", color: "#334155", action: "hid", code: "NEXT", span: 1 },
          { label: "VOL -", icon: "volume-1", color: "#10b981", action: "hid", code: "VOL_DOWN", span: 1 },
          { label: "MUTE", icon: "volume-x", color: "#64748b", action: "hid", code: "MUTE", span: 1 },
          { label: "VOL +", icon: "volume-2", color: "#10b981", action: "hid", code: "VOL_UP", span: 1 },
          { label: "SPACE", icon: "pause", color: "#475569", action: "hid", code: "SPACE", span: 1 },
          { label: "FULLSCREEN", icon: "monitor", color: "#3b82f6", action: "hid", code: "F11", span: 1 },
          { label: "ESC", icon: "corner-down-left", color: "#ef4444", action: "hid", code: "BACK", span: 1 }
        ]
      },
      {
        id: "macros",
        name: "Ducky Macros",
        columns: 2,
        buttons: [
          { label: "Lock PC", icon: "lock", color: "#ef4444", action: "macro", macro: "GUI l", span: 1 },
          { label: "Desktop", icon: "monitor", color: "#3b82f6", action: "macro", macro: "GUI d", span: 1 },
          { label: "Task Manager", icon: "activity", color: "#f59e0b", action: "macro", macro: "CTRL SHIFT ESC", span: 1 },
          { label: "Alt+Tab", icon: "monitor", color: "#8b5cf6", action: "macro", macro: "ALT TAB", span: 1 },
          { label: "Open Terminal", icon: "terminal", color: "#10b981", action: "macro", macro: "GUI r\nDELAY 150\nSTRING cmd\nENTER", span: 2 },
          { label: "Close Window", icon: "power", color: "#dc2626", action: "macro", macro: "ALT F4", span: 2 }
        ]
      }
    ]
  },

  mac: {
    id: "mac-shortcuts",
    name: "macOS Media & Control",
    deviceType: "mac",
    columns: 3,
    pages: [
      {
        id: "main",
        name: "Controls",
        columns: 3,
        buttons: [
          { label: "MUTE", icon: "volume-x", color: "#64748b", action: "hid", code: "MUTE", span: 1 },
          { label: "VOL -", icon: "volume-1", color: "#10b981", action: "hid", code: "VOL_DOWN", span: 1 },
          { label: "VOL +", icon: "volume-2", color: "#10b981", action: "hid", code: "VOL_UP", span: 1 },
          { label: "⏮ PREV", icon: "skip-back", color: "#334155", action: "hid", code: "PREV", span: 1 },
          { label: "⏯ PLAY", icon: "play", color: "#06b6d4", action: "hid", code: "PLAY_PAUSE", span: 1 },
          { label: "⏭ NEXT", icon: "skip-forward", color: "#334155", action: "hid", code: "NEXT", span: 1 },
          { label: "Spotlight", icon: "search", color: "#3b82f6", action: "macro", macro: "GUI SPACE", span: 1 },
          { label: "Mission Ctrl", icon: "monitor", color: "#8b5cf6", action: "macro", macro: "CTRL UP", span: 1 },
          { label: "Lock Mac", icon: "lock", color: "#ef4444", action: "macro", macro: "CTRL GUI q", span: 1 }
        ]
      }
    ]
  },

  android: {
    id: "android-tv",
    name: "Android TV Controller",
    deviceType: "android",
    columns: 3,
    pages: [
      {
        id: "nav",
        name: "Remote",
        columns: 3,
        buttons: [
          { label: "POWER", icon: "power", color: "#ef4444", action: "hid", code: "POWER", span: 1 },
          { label: "▲", icon: "arrow-up", color: "#334155", action: "hid", code: "UP", span: 1 },
          { label: "MUTE", icon: "volume-x", color: "#64748b", action: "hid", code: "MUTE", span: 1 },
          { label: "◄", icon: "arrow-left", color: "#334155", action: "hid", code: "LEFT", span: 1 },
          { label: "OK", icon: "check", color: "#3b82f6", action: "hid", code: "OK", span: 1 },
          { label: "►", icon: "arrow-right", color: "#334155", action: "hid", code: "RIGHT", span: 1 },
          { label: "BACK", icon: "corner-down-left", color: "#475569", action: "hid", code: "BACK", span: 1 },
          { label: "▼", icon: "arrow-down", color: "#334155", action: "hid", code: "DOWN", span: 1 },
          { label: "HOME", icon: "home", color: "#475569", action: "hid", code: "HOME", span: 1 }
        ]
      }
    ]
  },

  kodi: {
    id: "kodi-remote",
    name: "Kodi Media Center",
    deviceType: "other",
    columns: 3,
    pages: [
      {
        id: "nav",
        name: "Playback",
        columns: 3,
        buttons: [
          { label: "Fullscreen", icon: "monitor", color: "#3b82f6", action: "hid", code: "TAB", span: 1 },
          { label: "Subtitles", icon: "info", color: "#f59e0b", action: "macro", macro: "STRING s", span: 1 },
          { label: "Audio Delay", icon: "activity", color: "#8b5cf6", action: "macro", macro: "STRING a", span: 1 },
          { label: "⏮ Prev", icon: "skip-back", color: "#334155", action: "hid", code: "PREV", span: 1 },
          { label: "⏯ Play", icon: "play", color: "#06b6d4", action: "hid", code: "PLAY_PAUSE", span: 1 },
          { label: "⏭ Next", icon: "skip-forward", color: "#334155", action: "hid", code: "NEXT", span: 1 },
          { label: "VOL -", icon: "volume-1", color: "#10b981", action: "hid", code: "VOL_DOWN", span: 1 },
          { label: "MUTE", icon: "volume-x", color: "#64748b", action: "hid", code: "MUTE", span: 1 },
          { label: "VOL +", icon: "volume-2", color: "#10b981", action: "hid", code: "VOL_UP", span: 1 }
        ]
      }
    ]
  },

  sysadmin: {
    id: "sysadmin-ducky",
    name: "SysAdmin Ducky Toolkit",
    deviceType: "pc",
    columns: 2,
    pages: [
      {
        id: "tools",
        name: "Quick Tools",
        columns: 2,
        buttons: [
          { label: "PowerShell Admin", icon: "terminal", color: "#3b82f6", action: "macro", macro: "GUI r\nDELAY 150\nSTRING powershell\nENTER", span: 2 },
          { label: "Lock Workstation", icon: "lock", color: "#ef4444", action: "macro", macro: "GUI l", span: 1 },
          { label: "Task Manager", icon: "activity", color: "#f59e0b", action: "macro", macro: "CTRL SHIFT ESC", span: 1 },
          { label: "Network Info", icon: "terminal", color: "#10b981", action: "macro", macro: "GUI r\nDELAY 150\nSTRING cmd /k ipconfig /all\nENTER", span: 2 },
          { label: "Ping 8.8.8.8", icon: "terminal", color: "#06b6d4", action: "macro", macro: "GUI r\nDELAY 150\nSTRING cmd /k ping 8.8.8.8 -t\nENTER", span: 2 }
        ]
      }
    ]
  },

  blank: {
    id: "custom-layout",
    name: "Custom Remote",
    deviceType: "other",
    columns: 3,
    pages: [
      {
        id: "page1",
        name: "Main",
        columns: 3,
        buttons: [
          { label: "POWER", icon: "power", color: "#ef4444", action: "hid", code: "POWER", span: 1 },
          { label: "OK", icon: "check", color: "#3b82f6", action: "hid", code: "OK", span: 1 },
          { label: "MUTE", icon: "volume-x", color: "#64748b", action: "hid", code: "MUTE", span: 1 }
        ]
      }
    ]
  }
};

// Current App State
let profile = JSON.parse(JSON.stringify(TEMPLATES.tv));
let currentPageIdx = 0;
let selectedBtnIdx = null;

// Initialize on Load
window.addEventListener('DOMContentLoaded', () => {
  const saved = localStorage.getItem('lilygo_studio_profile');
  if (saved) {
    try {
      profile = JSON.parse(saved);
    } catch (e) {
      console.warn('Using default template');
    }
  }
  const savedHost = localStorage.getItem('lilygo_target_host');
  if (savedHost) {
    document.getElementById('targetHost').value = savedHost;
  }
  renderApp();
  checkConnection();
});

function saveState() {
  localStorage.setItem('lilygo_studio_profile', JSON.stringify(profile));
}

// Render Complete App
function renderApp() {
  document.getElementById('profileId').value = profile.id || '';
  document.getElementById('profileName').value = profile.name || '';
  document.getElementById('profileDevice').value = profile.deviceType || 'tv';
  document.getElementById('simRoom').innerText = profile.name || 'Remote';

  renderPagesList();
  renderSimTabs();
  renderSimGrid();
  renderInspector();
  saveState();
}

// Pages Management
function renderPagesList() {
  const list = document.getElementById('pagesList');
  list.innerHTML = '';

  (profile.pages || []).forEach((p, idx) => {
    const item = document.createElement('div');
    item.className = 'page-item' + (idx === currentPageIdx ? ' active' : '');
    item.innerHTML = `
      <span>${p.name || `Page ${idx + 1}`}</span>
      <div style="display: flex; gap: 4px;">
        <span onclick="renamePage(${idx}, event)" title="Rename" style="cursor: pointer; opacity: 0.7;">✎</span>
        ${profile.pages.length > 1 ? `<span onclick="deletePage(${idx}, event)" title="Delete" style="cursor: pointer; opacity: 0.7; color: #ef4444;">✕</span>` : ''}
      </div>
    `;
    item.onclick = () => {
      currentPageIdx = idx;
      selectedBtnIdx = null;
      renderApp();
    };
    list.appendChild(item);
  });

  const curPage = profile.pages[currentPageIdx] || profile.pages[0];
  document.getElementById('pageCols').value = curPage.columns || profile.columns || 3;
}

function addNewPage() {
  const name = prompt('Enter page name:', `Page ${profile.pages.length + 1}`);
  if (!name) return;
  profile.pages.push({
    id: 'page-' + Date.now(),
    name: name,
    columns: 3,
    buttons: []
  });
  currentPageIdx = profile.pages.length - 1;
  selectedBtnIdx = null;
  renderApp();
}

function renamePage(idx, e) {
  e.stopPropagation();
  const current = profile.pages[idx].name;
  const name = prompt('Rename page:', current);
  if (name && name.trim()) {
    profile.pages[idx].name = name.trim();
    renderApp();
  }
}

function deletePage(idx, e) {
  e.stopPropagation();
  if (profile.pages.length <= 1) return;
  if (!confirm(`Delete page "${profile.pages[idx].name}"?`)) return;
  profile.pages.splice(idx, 1);
  if (currentPageIdx >= profile.pages.length) currentPageIdx = profile.pages.length - 1;
  selectedBtnIdx = null;
  renderApp();
}

function updatePageCols() {
  const cols = parseInt(document.getElementById('pageCols').value, 10);
  const curPage = profile.pages[currentPageIdx];
  if (curPage) {
    curPage.columns = cols;
    renderApp();
  }
}

function updateProfileMeta() {
  profile.id = document.getElementById('profileId').value.trim();
  profile.name = document.getElementById('profileName').value.trim();
  profile.deviceType = document.getElementById('profileDevice').value;
  document.getElementById('simRoom').innerText = profile.name;
  saveState();
}

// Phone Simulation Tabs & Grid
function renderSimTabs() {
  const tabs = document.getElementById('simTabs');
  tabs.innerHTML = '';
  if (profile.pages.length <= 1) {
    tabs.style.display = 'none';
    return;
  }
  tabs.style.display = 'flex';
  profile.pages.forEach((p, idx) => {
    const pill = document.createElement('div');
    pill.className = 'tab-pill' + (idx === currentPageIdx ? ' active' : '');
    pill.innerText = p.name || `Page ${idx + 1}`;
    pill.onclick = () => {
      currentPageIdx = idx;
      selectedBtnIdx = null;
      renderApp();
    };
    tabs.appendChild(pill);
  });
}

function renderSimGrid() {
  const grid = document.getElementById('simGrid');
  const curPage = profile.pages[currentPageIdx] || profile.pages[0];
  const cols = curPage.columns || profile.columns || 3;
  grid.style.gridTemplateColumns = `repeat(${cols}, 1fr)`;
  grid.innerHTML = '';

  (curPage.buttons || []).forEach((b, idx) => {
    if (b.action === 'none') {
      const sp = document.createElement('div');
      sp.className = 'sim-spacer' + (idx === selectedBtnIdx ? ' selected' : '');
      if (b.span) sp.style.gridColumn = `span ${b.span}`;
      sp.innerText = 'Spacer';
      sp.onclick = () => selectButton(idx);
      grid.appendChild(sp);
      return;
    }

    const el = document.createElement('div');
    el.className = 'sim-btn' + (idx === selectedBtnIdx ? ' selected' : '');
    if (b.span) el.style.gridColumn = `span ${b.span}`;
    if (b.color) el.style.background = b.color;

    let content = '';
    if (b.icon) content += `<span class="sim-btn-icon">${getIconSymbol(b.icon)}</span>`;
    if (b.label) content += `<span class="sim-btn-label">${b.label}</span>`;
    el.innerHTML = content || '&bull;';

    el.onclick = () => {
      selectButton(idx);
      if (document.getElementById('liveTestToggle').checked) {
        testButtonLive(b);
      }
    };
    grid.appendChild(el);
  });
}

function selectButton(idx) {
  selectedBtnIdx = idx;
  renderSimGrid();
  renderInspector();
}

function addButtonToPage() {
  const curPage = profile.pages[currentPageIdx];
  if (!curPage.buttons) curPage.buttons = [];
  curPage.buttons.push({
    label: "NEW",
    icon: "",
    color: "#1e293b",
    action: "hid",
    code: "OK",
    span: 1
  });
  selectedBtnIdx = curPage.buttons.length - 1;
  renderApp();
}

function addSpacerToPage() {
  const curPage = profile.pages[currentPageIdx];
  if (!curPage.buttons) curPage.buttons = [];
  curPage.buttons.push({
    label: "",
    action: "none",
    span: 1
  });
  selectedBtnIdx = curPage.buttons.length - 1;
  renderApp();
}

// Right Inspector
function renderInspector() {
  const noSel = document.getElementById('noSelectionMsg');
  const form = document.getElementById('inspectorForm');
  const curPage = profile.pages[currentPageIdx];

  if (selectedBtnIdx === null || !curPage || !curPage.buttons[selectedBtnIdx]) {
    noSel.style.display = 'block';
    form.style.display = 'none';
    return;
  }

  noSel.style.display = 'none';
  form.style.display = 'flex';

  const b = curPage.buttons[selectedBtnIdx];
  document.getElementById('btnAction').value = b.action || 'hid';
  document.getElementById('btnLabel').value = b.label || '';
  document.getElementById('btnIcon').value = b.icon || '';
  document.getElementById('btnSpan').value = b.span || 1;
  document.getElementById('hidKey').value = b.code || 'OK';

  changeBtnAction();

  if (b.action === 'macro') {
    document.getElementById('rawDuckyEditor').value = b.macro || '';
    renderMacroSteps(b.macro || '');
  }
}

function changeBtnAction() {
  const act = document.getElementById('btnAction').value;
  const visualProps = document.getElementById('btnVisualProps');
  const hidConfig = document.getElementById('hidConfig');
  const macroConfig = document.getElementById('macroConfig');

  if (act === 'none') {
    visualProps.style.display = 'none';
    hidConfig.style.display = 'none';
    macroConfig.style.display = 'none';
  } else if (act === 'hid') {
    visualProps.style.display = 'block';
    hidConfig.style.display = 'block';
    macroConfig.style.display = 'none';
  } else if (act === 'macro') {
    visualProps.style.display = 'block';
    hidConfig.style.display = 'none';
    macroConfig.style.display = 'block';
  }

  updateSelectedBtn();
}

function updateSelectedBtn() {
  const curPage = profile.pages[currentPageIdx];
  if (selectedBtnIdx === null || !curPage || !curPage.buttons[selectedBtnIdx]) return;

  const b = curPage.buttons[selectedBtnIdx];
  b.action = document.getElementById('btnAction').value;
  b.label = document.getElementById('btnLabel').value;
  b.icon = document.getElementById('btnIcon').value;
  b.span = parseInt(document.getElementById('btnSpan').value, 10);
  b.code = document.getElementById('hidKey').value;

  renderSimGrid();
  saveState();
}

function setBtnColor(hex) {
  const curPage = profile.pages[currentPageIdx];
  if (selectedBtnIdx === null || !curPage || !curPage.buttons[selectedBtnIdx]) return;
  curPage.buttons[selectedBtnIdx].color = hex;
  renderSimGrid();
  saveState();
}

function moveBtn(direction) {
  const curPage = profile.pages[currentPageIdx];
  if (selectedBtnIdx === null || !curPage) return;
  const target = selectedBtnIdx + direction;
  if (target < 0 || target >= curPage.buttons.length) return;

  const temp = curPage.buttons[selectedBtnIdx];
  curPage.buttons[selectedBtnIdx] = curPage.buttons[target];
  curPage.buttons[target] = temp;
  selectedBtnIdx = target;
  renderSimGrid();
  saveState();
}

function deleteSelectedBtn() {
  const curPage = profile.pages[currentPageIdx];
  if (selectedBtnIdx === null || !curPage) return;
  curPage.buttons.splice(selectedBtnIdx, 1);
  selectedBtnIdx = null;
  renderSimGrid();
  renderInspector();
  saveState();
}

// Macro Builder & DuckyScript
function switchMacroTab(mode) {
  const vBtn = document.getElementById('tabBtnVisual');
  const rBtn = document.getElementById('tabBtnRaw');
  const vPan = document.getElementById('macroVisualPanel');
  const rPan = document.getElementById('macroRawPanel');

  if (mode === 'visual') {
    vBtn.classList.add('active');
    rBtn.classList.remove('active');
    vPan.style.display = 'block';
    rPan.style.display = 'none';
    renderMacroSteps(document.getElementById('rawDuckyEditor').value);
  } else {
    rBtn.classList.add('active');
    vBtn.classList.remove('active');
    rPan.style.display = 'block';
    vPan.style.display = 'none';
  }
}

function onRawDuckyInput() {
  const curPage = profile.pages[currentPageIdx];
  if (selectedBtnIdx === null || !curPage) return;
  curPage.buttons[selectedBtnIdx].macro = document.getElementById('rawDuckyEditor').value;
  saveState();
}

function renderMacroSteps(script) {
  const container = document.getElementById('macroStepsList');
  container.innerHTML = '';
  const lines = script.split('\n').map(l => l.trim()).filter(l => l.length > 0 && !l.startsWith('REM'));

  if (lines.length === 0) {
    container.innerHTML = '<div style="color: var(--text-muted); font-size: 0.75rem; padding: 6px 0;">No actions yet. Click buttons below to add steps.</div>';
    return;
  }

  lines.forEach((line, idx) => {
    const item = document.createElement('div');
    item.className = 'step-item';
    item.innerHTML = `
      <div style="display: flex; align-items: center; gap: 8px;">
        <span style="font-size: 0.72rem; color: var(--accent-primary); font-weight: 700;">#${idx + 1}</span>
        <span style="font-family: monospace; font-size: 0.8rem; color: #fff;">${escapeHtml(line)}</span>
      </div>
      <button onclick="removeMacroStep(${idx})" style="padding: 2px 6px; font-size: 0.7rem; color: #ef4444; border: none; background: none;">✕</button>
    `;
    container.appendChild(item);
  });
}

function addMacroStep(type) {
  let line = '';
  if (type === 'string') {
    const text = prompt('Text to type automatically:');
    if (text === null) return;
    line = 'STRING ' + text;
  } else if (type === 'combo') {
    const combo = prompt('Key combo (e.g. GUI r, CTRL SHIFT ESC, ALT F4):', 'GUI r');
    if (!combo) return;
    line = combo.trim().toUpperCase();
  } else if (type === 'delay') {
    const ms = prompt('Delay milliseconds (e.g. 200):', '200');
    if (!ms) return;
    line = 'DELAY ' + parseInt(ms, 10);
  } else if (type === 'enter') {
    line = 'ENTER';
  }

  const editor = document.getElementById('rawDuckyEditor');
  editor.value = (editor.value.trim() ? editor.value.trim() + '\n' : '') + line;
  onRawDuckyInput();
  renderMacroSteps(editor.value);
}

function removeMacroStep(stepIdx) {
  const editor = document.getElementById('rawDuckyEditor');
  const lines = editor.value.split('\n').map(l => l.trim()).filter(l => l.length > 0 && !l.startsWith('REM'));
  lines.splice(stepIdx, 1);
  editor.value = lines.join('\n');
  onRawDuckyInput();
  renderMacroSteps(editor.value);
}

// Templates Loader
function loadSelectedTemplate() {
  const sel = document.getElementById('templateSelect');
  const key = sel.value;
  if (!key || !TEMPLATES[key]) return;
  if (!confirm('Load preset template "' + sel.options[sel.selectedIndex].text + '"? Current changes will be overwritten.')) return;

  profile = JSON.parse(JSON.stringify(TEMPLATES[key]));
  currentPageIdx = 0;
  selectedBtnIdx = null;
  renderApp();
  sel.value = '';
}

// Import / Export JSON
function exportProfile() {
  const jsonStr = JSON.stringify(profile, null, 2);
  const blob = new Blob([jsonStr], { type: 'application/json' });
  const a = document.createElement('a');
  a.href = URL.createObjectURL(blob);
  a.download = `${profile.id || 'profile'}.json`;
  a.click();
}

function importProfile() {
  document.getElementById('fileInput').click();
}

function onFileSelected(e) {
  const file = e.target.files[0];
  if (!file) return;
  const reader = new FileReader();
  reader.onload = (event) => {
    try {
      const data = JSON.parse(event.target.result);
      if (data.pages && Array.isArray(data.pages)) {
        profile = data;
        currentPageIdx = 0;
        selectedBtnIdx = null;
        renderApp();
        alert('Profile imported successfully!');
      } else {
        alert('Invalid profile JSON structure.');
      }
    } catch (err) {
      alert('Failed to parse JSON file: ' + err.message);
    }
  };
  reader.readAsText(file);
}

// Connection & Deployment to LilyGO
function getTargetHost() {
  let host = document.getElementById('targetHost').value.trim();
  if (host.endsWith('/')) host = host.slice(0, -1);
  localStorage.setItem('lilygo_target_host', host);
  return host;
}

async function checkConnection() {
  const host = getTargetHost();
  const dot = document.getElementById('connDot');
  try {
    const res = await fetch(`${host}/api/profiles`, { method: 'GET', mode: 'cors' });
    if (res.ok) {
      dot.className = 'status-dot connected';
      dot.title = 'Connected to LilyGO';
    } else {
      dot.className = 'status-dot';
    }
  } catch (e) {
    dot.className = 'status-dot';
  }
}

// Deploy over Wi-Fi
async function deployToDongle() {
  const host = getTargetHost();
  const token = localStorage.getItem('tv_remote_token') || '';

  if (!token) {
    if (confirm('Pairing token not found. You must pair this companion studio with the LilyGO first. Open pairing dialog now?')) {
      openPairModal();
    }
    return;
  }

  try {
    const res = await fetch(`${host}/api/profiles/upload?set_active=1&token=${encodeURIComponent(token)}`, {
      method: 'POST',
      mode: 'cors',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(profile)
    });
    const data = await res.json();
    if (data.status === 'ok') {
      alert(`🎉 Success! Profile "${profile.name}" deployed and set as ACTIVE on the LilyGO!`);
    } else {
      alert('Deployment failed: ' + (data.error || 'Pairing token rejected. Re-pair device.'));
    }
  } catch (e) {
    alert(`Could not connect to LilyGO at ${host}. Ensure you are on the same Wi-Fi network.`);
  }
}

// Live Test Button on Dongle
async function testButtonLive(btn) {
  const host = getTargetHost();
  const token = localStorage.getItem('tv_remote_token') || '';

  try {
    if (btn.action === 'hid') {
      await fetch(`${host}/sendkey`, {
        method: 'POST',
        mode: 'cors',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: new URLSearchParams({ key: btn.code, token: token })
      });
    } else if (btn.action === 'macro') {
      await fetch(`${host}/api/macro/run`, {
        method: 'POST',
        mode: 'cors',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: new URLSearchParams({ script: btn.macro, token: token })
      });
    }
  } catch (e) {
    console.warn('Live test dispatch failed:', e);
  }
}

async function testCurrentMacro() {
  const curPage = profile.pages[currentPageIdx];
  if (selectedBtnIdx === null || !curPage) return;
  const b = curPage.buttons[selectedBtnIdx];
  if (b.action !== 'macro' || !b.macro) return;
  testButtonLive(b);
}

// Pairing Modal & Flow
function openPairModal() {
  document.getElementById('pairModal').style.display = 'flex';
  document.getElementById('pairModalStatus').innerText = '';
}

function closePairModal() {
  document.getElementById('pairModal').style.display = 'none';
}

let pairPollTimer = null;
async function initiatePairing() {
  const host = getTargetHost();
  const devId = 'studio-' + Math.random().toString(16).substr(2, 8);
  const statusEl = document.getElementById('pairModalStatus');
  statusEl.innerText = 'Request sent! Press the button on the LilyGO stick now...';

  try {
    await fetch(`${host}/api/pair_request?device=${devId}`, { method: 'POST', mode: 'cors' });
    if (pairPollTimer) clearInterval(pairPollTimer);

    pairPollTimer = setInterval(async () => {
      try {
        const res = await fetch(`${host}/api/pair_status?device=${devId}`, { mode: 'cors' });
        const data = await res.json();
        if (data.status === 'approved' && data.token) {
          clearInterval(pairPollTimer);
          localStorage.setItem('tv_remote_token', data.token);
          statusEl.innerText = 'Device Approved & Paired! 🎉';
          checkConnection();
          setTimeout(closePairModal, 1500);
        } else if (data.status === 'expired') {
          clearInterval(pairPollTimer);
          statusEl.innerText = 'Pairing timed out. Try again.';
        }
      } catch (err) {
        // continue polling
      }
    }, 1000);
  } catch (e) {
    statusEl.innerText = 'Could not contact LilyGO at ' + host;
  }
}

// Icon Mapping Helper
function getIconSymbol(icon) {
  const map = {
    'power': '⏻',
    'volume-x': '🔇',
    'volume-1': '🔉',
    'volume-2': '🔊',
    'play': '▶',
    'pause': '⏸',
    'skip-back': '⏮',
    'skip-forward': '⏭',
    'arrow-up': '▲',
    'arrow-down': '▼',
    'arrow-left': '◀',
    'arrow-right': '▶',
    'check': '✔',
    'corner-down-left': '⏎',
    'home': '🏠',
    'info': 'ℹ',
    'lock': '🔒',
    'monitor': '🖥',
    'terminal': '▶_',
    'activity': '📊',
    'film': '🎬',
    'video': '🎥',
    'search': '🔍'
  };
  return map[icon] || icon;
}

function escapeHtml(str) {
  return str.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
}
</script>
</body>
</html>
)rawliteral";
