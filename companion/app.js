// =========================================================================
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
  if (window.location.protocol === 'https:') {
    const banner = document.getElementById('httpsBanner');
    if (banner) banner.style.display = 'flex';
  }

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
function showToast(msg) {
  const toast = document.getElementById('toast');
  if (!toast) return;
  toast.innerText = msg;
  toast.style.display = 'block';
  setTimeout(() => { toast.style.display = 'none'; }, 2500);
}

function copyProfileJson() {
  const jsonStr = JSON.stringify(profile, null, 2);
  if (navigator.clipboard && navigator.clipboard.writeText) {
    navigator.clipboard.writeText(jsonStr).then(() => {
      showToast('Profile JSON copied to clipboard! 📋');
    }).catch(err => {
      prompt('Copy the profile JSON below:', jsonStr);
    });
  } else {
    prompt('Copy the profile JSON below:', jsonStr);
  }
}

async function deployViaUsb() {
  if (!('serial' in navigator)) {
    alert('WebSerial is not supported in this browser. Please open this page in Google Chrome or Microsoft Edge to deploy directly via USB.');
    return;
  }
  try {
    const port = await navigator.serial.requestPort();
    await port.open({ baudRate: 115200 });
    const writer = port.writable.getWriter();
    const encoder = new TextEncoder();
    
    const jsonPayload = JSON.stringify(profile);
    const cmd = `PROFILE_UPLOAD:${jsonPayload}\n`;
    await writer.write(encoder.encode(cmd));
    writer.releaseLock();
    await port.close();
    
    showToast('🎉 Profile sent to LilyGO via USB!');
    alert(`🎉 Profile "${profile.name}" successfully sent to LilyGO over USB Serial!`);
  } catch (err) {
    if (err.name !== 'NotFoundError') {
      alert('USB Deploy notice: ' + err.message);
    }
  }
}

async function deployToDongle() {
  const host = getTargetHost();

  if (window.location.protocol === 'https:' && host.startsWith('http://')) {
    alert(
      '⚠️ HTTPS Mixed-Content Notice:\n\n' +
      'Browsers block direct Wi-Fi network requests from HTTPS sites (like GitHub Pages) to local HTTP devices.\n\n' +
      'To deploy to your dongle:\n' +
      '1. Click "Export JSON" (or "Copy JSON").\n' +
      '2. Open your dongle setup page (http://tv-remote.local/setup).\n' +
      '3. Select the file in "Upload Profile JSON" and click Save.\n\n' +
      'Or click "⚡ USB Deploy" to sync directly over USB!'
    );
    return;
  }

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
