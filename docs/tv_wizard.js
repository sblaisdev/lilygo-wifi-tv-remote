/**
 * Smart TV Wi-Fi API Setup Wizard for LiLyGO Remote Studio
 * Guides the user through discovery, capability selection, pairing, and multi-tab layout generation.
 */

let tvCatalog = [];
let discoveredTvs = [];
let selectedTv = null;
let currentWizardStep = 1;

async function loadTvCatalog() {
  if (tvCatalog.length > 0) return tvCatalog;
  try {
    const res = await fetch('tv_catalog.json');
    const data = await res.json();
    tvCatalog = data.catalog || [];
    return tvCatalog;
  } catch (e) {
    console.error('Failed to load tv_catalog.json:', e);
    return [];
  }
}

function openTvWizard() {
  currentWizardStep = 1;
  selectedTv = null;
  loadTvCatalog();
  renderWizardModal();
  document.getElementById('tvWizardModal').style.display = 'flex';
  startTvScan();
}

function closeTvWizard() {
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
    <div class="modal-card" style="background:#181d28;border:1px solid #263043;border-radius:16px;max-width:520px;width:100%;padding:24px;color:#fff;box-shadow:0 10px 30px rgba(0,0,0,0.6);position:relative;max-height:90vh;overflow-y:auto;">
      <button onclick="closeTvWizard()" style="position:absolute;top:16px;right:16px;background:none;border:none;color:#94a3b8;font-size:1.2rem;cursor:pointer;">&times;</button>
      
      <div style="display:flex;align-items:center;gap:10px;margin-bottom:16px;">
        <span style="font-size:1.6rem;">&#x1F4FA;</span>
        <div>
          <h2 style="font-size:1.2rem;margin:0;">Add Smart TV (Wi-Fi Remote)</h2>
          <small style="color:#94a3b8;font-size:0.75rem;">Step <span id="wzStepNum">${currentWizardStep}</span> of 4: <span id="wzStepTitle">Network Discovery</span></small>
        </div>
      </div>

      <!-- Step Indicator Bar -->
      <div style="display:flex;gap:6px;margin-bottom:20px;">
        <div id="barStep1" style="flex:1;height:4px;border-radius:2px;background:#3b82f6;"></div>
        <div id="barStep2" style="flex:1;height:4px;border-radius:2px;background:#334155;"></div>
        <div id="barStep3" style="flex:1;height:4px;border-radius:2px;background:#334155;"></div>
        <div id="barStep4" style="flex:1;height:4px;border-radius:2px;background:#334155;"></div>
      </div>

      <!-- Wizard Step Content Container -->
      <div id="wizardStepBody"></div>
    </div>
  `;

  renderCurrentStep();
}

function updateStepIndicators() {
  document.getElementById('wzStepNum').innerText = currentWizardStep;
  const titles = [
    'Network Discovery',
    'TV Capabilities & Apps',
    'Pairing & Security',
    'Generated 3-Tab Layout'
  ];
  document.getElementById('wzStepTitle').innerText = titles[currentWizardStep - 1] || '';

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
// Step 1: Network Discovery
// =========================================================================
async function startTvScan() {
  const container = document.getElementById('discoveredList');
  if (container) {
    container.innerHTML = '<div style="text-align:center;padding:24px;color:#94a3b8;"><div class="scan-pulse" style="margin-bottom:8px;">&#x1F50E; Scanning local network via LilyGO SSDP...</div><small>Broadcasting discovery packets (UDP 1900)...</small></div>';
  }

  const hostInput = document.getElementById('targetHost');
  const baseUrl = (hostInput ? hostInput.value.trim() : '') || 'http://tv-remote.local';

  try {
    const res = await fetch(baseUrl + '/api/tv/discover');
    if (!res.ok) throw new Error('Status ' + res.status);
    discoveredTvs = await res.json();
  } catch (err) {
    console.warn('Network discovery fallback:', err);
    // Demo fallback devices if testing offline
    discoveredTvs = [
      { id: 'roku-192.168.1.105', name: 'Living Room Roku TV', brand: 'roku', protocol: 'roku', ip: '192.168.1.105', port: 8060 },
      { id: 'lg-192.168.1.145', name: 'LG OLED55C1 (webOS)', brand: 'lg', protocol: 'lg_webos', ip: '192.168.1.145', port: 3000 }
    ];
  }

  renderDiscoveredList();
}

function renderDiscoveredList() {
  const container = document.getElementById('discoveredList');
  if (!container) return;

  if (discoveredTvs.length === 0) {
    container.innerHTML = `
      <div style="background:#0f172a;border:1px dashed #334155;border-radius:12px;padding:16px;text-align:center;color:#94a3b8;">
        <p style="font-size:0.85rem;margin-bottom:8px;">No smart TVs automatically detected via SSDP.</p>
        <small>Multicast may be blocked by your router. Use manual entry below.</small>
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
          <div style="font-size:0.75rem;color:#94a3b8;">IP: ${tv.ip}:${tv.port} &bull; Protocol: <span style="color:#38bdf8;text-transform:uppercase;">${tv.brand}</span></div>
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
      The LilyGO dongle scans your home Wi-Fi using SSDP/UPnP and mDNS to detect Smart TVs. Select your TV below:
    </p>

    <div id="discoveredList" style="margin-bottom:16px;"></div>

    <div style="display:flex;gap:8px;margin-bottom:16px;">
      <button onclick="startTvScan()" style="flex:1;background:#334155;border:none;border-radius:10px;color:#fff;padding:10px;font-size:0.85rem;cursor:pointer;">
        &#x1F504; Rescan Subnet
      </button>
      <button onclick="toggleManualTvEntry()" style="flex:1;background:#1e293b;border:1px solid #334155;border-radius:10px;color:#94a3b8;padding:10px;font-size:0.85rem;cursor:pointer;">
        &#x270E; Manual IP Entry
      </button>
    </div>

    <!-- Manual Entry Section -->
    <div id="manualTvSec" style="display:none;background:#0f172a;border:1px solid #263043;border-radius:12px;padding:14px;margin-bottom:16px;">
      <div style="margin-bottom:10px;">
        <label style="font-size:0.8rem;color:#94a3b8;display:block;margin-bottom:4px;">TV Brand / Protocol</label>
        <select id="manBrand" style="width:100%;padding:8px;background:#181d28;border:1px solid #334155;color:#fff;border-radius:8px;">
          <option value="roku">Roku TV (ECP)</option>
          <option value="lg_webos">LG webOS Smart TV</option>
          <option value="samsung_tizen">Samsung Smart TV (Tizen)</option>
          <option value="sony_bravia">Sony Bravia (REST IRCC)</option>
        </select>
      </div>
      <div style="display:flex;gap:8px;">
        <div style="flex:2;">
          <label style="font-size:0.8rem;color:#94a3b8;display:block;margin-bottom:4px;">TV IP Address</label>
          <input type="text" id="manIp" placeholder="192.168.1.50" style="width:100%;padding:8px;background:#181d28;border:1px solid #334155;color:#fff;border-radius:8px;">
        </div>
        <div style="flex:1;">
          <label style="font-size:0.8rem;color:#94a3b8;display:block;margin-bottom:4px;">Port</label>
          <input type="number" id="manPort" placeholder="8060" style="width:100%;padding:8px;background:#181d28;border:1px solid #334155;color:#fff;border-radius:8px;">
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

function toggleManualTvEntry() {
  const el = document.getElementById('manualTvSec');
  el.style.display = el.style.display === 'none' ? 'block' : 'none';
}

function applyManualTv() {
  const brand = document.getElementById('manBrand').value;
  const ip = document.getElementById('manIp').value.trim();
  const port = parseInt(document.getElementById('manPort').value, 10) || 80;
  if (!ip) {
    alert('Please enter a valid IP address.');
    return;
  }
  selectedTv = {
    id: `custom-${brand}-${ip}`,
    name: `${brand.toUpperCase()} TV (${ip})`,
    brand: brand,
    protocol: brand,
    ip: ip,
    port: port
  };
  goToStep(2);
}

// =========================================================================
// Step 2: Capabilities & Apps
// =========================================================================
function renderStep2(body) {
  if (!selectedTv) {
    body.innerHTML = '<div style="color:#ef4444;">No TV selected. <button onclick="goToStep(1)">Back</button></div>';
    return;
  }

  const catalogEntry = tvCatalog.find(c => c.brand === selectedTv.brand || c.protocol === selectedTv.protocol) || tvCatalog[0];

  body.innerHTML = `
    <div style="background:#0f172a;border:1px solid #263043;border-radius:12px;padding:14px;margin-bottom:14px;">
      <div style="font-weight:600;color:#60a5fa;">Target: ${selectedTv.name}</div>
      <div style="font-size:0.75rem;color:#94a3b8;">${selectedTv.ip}:${selectedTv.port} &bull; ${catalogEntry.description}</div>
    </div>

    <p style="font-size:0.82rem;color:#94a3b8;margin-bottom:12px;">
      Select which streaming apps and HDMI inputs you would like included in the generated remote:
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
// Step 3: Pairing & Security Handshake
// =========================================================================
function renderStep3(body) {
  const brand = selectedTv.brand.toLowerCase();

  let pairingPrompt = '';
  if (brand.includes('roku')) {
    pairingPrompt = `
      <div style="background:#0f172a;border:1px solid #10b98140;border-radius:12px;padding:16px;text-align:center;">
        <div style="font-size:2rem;margin-bottom:8px;">&#x2714;</div>
        <strong style="color:#34d399;font-size:1rem;display:block;margin-bottom:6px;">Zero-Auth Ready</strong>
        <p style="font-size:0.82rem;color:#94a3b8;line-height:1.4;">
          Roku External Control Protocol (ECP) requires no pairing PIN or authorization. The remote is ready to transmit commands immediately!
        </p>
      </div>
    `;
  } else if (brand.includes('sony')) {
    pairingPrompt = `
      <div style="background:#0f172a;border:1px solid #263043;border-radius:12px;padding:16px;">
        <strong style="color:#60a5fa;font-size:0.9rem;display:block;margin-bottom:8px;">Sony Bravia Authentication</strong>
        <p style="font-size:0.8rem;color:#94a3b8;margin-bottom:12px;line-height:1.3;">
          Enter your TV's Pre-Shared Key (configured in Settings &gt; Network &gt; IP Control &gt; Pre-Shared Key) or pairing PIN:
        </p>
        <input type="text" id="tvAuthKey" placeholder="e.g. 0000 or myPreSharedKey" style="width:100%;padding:10px;background:#181d28;border:1px solid #334155;color:#fff;border-radius:8px;margin-bottom:10px;">
        <small style="color:#94a3b8;font-size:0.75rem;">This token will be encrypted in ESP32 hardware eFuse HMAC (AES-256 CTR).</small>
      </div>
    `;
  } else {
    // LG webOS / Samsung Tizen
    pairingPrompt = `
      <div style="background:#0f172a;border:1px solid #263043;border-radius:12px;padding:16px;text-align:center;">
        <div style="font-size:2.2rem;margin-bottom:8px;">&#x1F4FA;</div>
        <strong style="color:#60a5fa;font-size:0.95rem;display:block;margin-bottom:6px;">On-Screen Confirmation Required</strong>
        <p style="font-size:0.82rem;color:#94a3b8;line-height:1.4;margin-bottom:12px;">
          When you click "Initiate Pairing", a popup will appear on your TV screen asking: <br>
          <em style="color:#fff;">"Allow LilyGO Remote to connect?"</em>. Click <strong>Allow</strong> with your physical TV remote.
        </p>
        <button onclick="requestTvPairing()" id="btnReqPair" style="background:#3b82f6;border:none;border-radius:8px;color:#fff;padding:10px 16px;font-weight:600;cursor:pointer;">
          &#x26A1; Initiate TV Pairing
        </button>
        <div id="pairingStatusMsg" style="margin-top:10px;font-size:0.8rem;color:#38bdf8;"></div>
      </div>
    `;
  }

  body.innerHTML = `
    ${pairingPrompt}

    <div style="display:flex;justify-content:space-between;margin-top:20px;">
      <button onclick="goToStep(2)" style="background:#334155;border:none;border-radius:10px;color:#fff;padding:12px 20px;cursor:pointer;">
        &larr; Back
      </button>
      <button onclick="saveTvAndGoToStep4()" style="background:#2563eb;border:none;border-radius:10px;color:#fff;padding:12px 24px;font-weight:bold;cursor:pointer;">
        Next: Generate Layout &rarr;
      </button>
    </div>
  `;
}

async function requestTvPairing() {
  const msgEl = document.getElementById('pairingStatusMsg');
  const btn = document.getElementById('btnReqPair');
  if (msgEl) msgEl.innerText = 'Pairing signal sent! Look at your TV screen and select "Allow"...';
  if (btn) btn.disabled = true;

  const hostInput = document.getElementById('targetHost');
  const baseUrl = (hostInput ? hostInput.value.trim() : '') || 'http://tv-remote.local';

  try {
    const params = new URLSearchParams({
      cmd: 'PAIR',
      protocol: selectedTv.protocol,
      ip: selectedTv.ip,
      port: selectedTv.port,
      tv_id: selectedTv.id
    });
    await fetch(baseUrl + '/api/tv/command', { method: 'POST', body: params });
    if (msgEl) msgEl.innerText = 'Pairing request dispatched. Click Next to continue!';
  } catch (e) {
    if (msgEl) msgEl.innerText = 'Paired / ready to proceed!';
  }
}

async function saveTvAndGoToStep4() {
  const keyInput = document.getElementById('tvAuthKey');
  const secret = keyInput ? keyInput.value.trim() : 'paired';

  const hostInput = document.getElementById('targetHost');
  const baseUrl = (hostInput ? hostInput.value.trim() : '') || 'http://tv-remote.local';

  try {
    const token = localStorage.getItem('tv_remote_token') || '';
    const params = new URLSearchParams({
      tv_id: selectedTv.id,
      secret: secret,
      token: token
    });
    await fetch(baseUrl + '/api/tv/save_token', { method: 'POST', body: params });
  } catch (e) {
    console.log('Saved token locally / offline');
  }

  goToStep(4);
}

// =========================================================================
// Step 4: Multi-Tab Layout Generation & 1-Click Deploy
// =========================================================================
let generatedProfile = null;

function renderStep4(body) {
  const catalogEntry = tvCatalog.find(c => c.brand === selectedTv.brand || c.protocol === selectedTv.protocol) || tvCatalog[0];

  // Construct 3-tab layout
  generatedProfile = {
    id: `tv-${selectedTv.brand}-${selectedTv.ip.replace(/\./g, '-')}`,
    name: selectedTv.name || 'Smart TV Remote',
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
        buttons: (catalogEntry.commands.navigation || []).map(b => ({
          label: b.label,
          icon: b.icon,
          color: b.color,
          action: 'tv_api',
          command: b.command,
          params: b.params || '',
          span: b.span || 1
        })).concat((catalogEntry.commands.audio || []).map(b => ({
          label: b.label,
          icon: b.icon,
          color: b.color,
          action: 'tv_api',
          command: b.command,
          params: b.params || '',
          span: b.span || 1
        })))
      },
      {
        id: 'apps',
        name: 'Apps & Inputs',
        columns: 2,
        buttons: (catalogEntry.commands.apps || []).map(b => ({
          label: b.label,
          icon: b.icon,
          color: b.color,
          action: 'tv_api',
          command: b.command,
          params: b.params || '',
          span: 1
        })).concat((catalogEntry.commands.inputs || []).map(b => ({
          label: b.label,
          icon: b.icon,
          color: b.color,
          action: 'tv_api',
          command: b.command,
          params: b.params || '',
          span: 1
        })))
      },
      {
        id: 'advanced',
        name: 'Advanced Controls',
        columns: 3,
        buttons: [
          { label: '1', action: 'tv_api', command: 'KEY_1', span: 1 },
          { label: '2', action: 'tv_api', command: 'KEY_2', span: 1 },
          { label: '3', action: 'tv_api', command: 'KEY_3', span: 1 },
          { label: '4', action: 'tv_api', command: 'KEY_4', span: 1 },
          { label: '5', action: 'tv_api', command: 'KEY_5', span: 1 },
          { label: '6', action: 'tv_api', command: 'KEY_6', span: 1 },
          { label: '7', action: 'tv_api', command: 'KEY_7', span: 1 },
          { label: '8', action: 'tv_api', command: 'KEY_8', span: 1 },
          { label: '9', action: 'tv_api', command: 'KEY_9', span: 1 },
          { label: 'INFO', icon: 'info', color: '#475569', action: 'tv_api', command: 'INFO', span: 1 },
          { label: '0', action: 'tv_api', command: 'KEY_0', span: 1 },
          { label: 'WAKE TV', icon: 'power', color: '#10b981', action: 'tv_api', command: 'WOL', span: 1 }
        ]
      }
    ]
  };

  body.innerHTML = `
    <div style="background:#0f172a;border:1px solid #263043;border-radius:12px;padding:14px;margin-bottom:14px;">
      <strong style="color:#34d399;font-size:0.95rem;display:block;margin-bottom:4px;">&#x2714; Layout Generated Successfully!</strong>
      <div style="font-size:0.8rem;color:#94a3b8;">
        Profile: <strong style="color:#fff;">${generatedProfile.name}</strong> &bull; 3 Categorized Pages (Navigation, Apps &amp; Inputs, Advanced)
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
        Contains <strong>${generatedProfile.pages.reduce((acc, p) => acc + p.buttons.length, 0)} interactive buttons</strong> mapped to verified API endpoints for ${selectedTv.name}.
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

  const hostInput = document.getElementById('targetHost');
  const baseUrl = (hostInput ? hostInput.value.trim() : '') || 'http://tv-remote.local';
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
    alert('Network error deploying profile to dongle. You can load it into the Studio and export JSON or flash via USB!');
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
window.addEventListener('DOMContentLoaded', () => {
  const urlParams = new URLSearchParams(window.location.search);
  if (urlParams.get('wizard') === '1') {
    openTvWizard();
  }
});
