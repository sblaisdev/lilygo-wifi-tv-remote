#!/usr/bin/env python3
"""
LilyGO TV Remote - Local Web Preview Server
Serves the firmware setup page and remote control interface locally
with live layout switching between Integrated and Top-Row iOS back buttons.
"""

import http.server
import socketserver
import os
import re
import sys

PORT = 8080
BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
WEBPAGE_H = os.path.join(BASE_DIR, "src", "webpage.h")
ICON_SVG = os.path.join(BASE_DIR, "icon.svg")
TV_WIZARD_JS = os.path.join(BASE_DIR, "companion", "tv_wizard.js")


def extract_template(name):
    if not os.path.exists(WEBPAGE_H):
        return f"<h1>Error: {WEBPAGE_H} not found</h1>"
    with open(WEBPAGE_H, "r", encoding="utf-8") as f:
        content = f.read()

    pattern = rf"const char {name}\[\] PROGMEM = R\"rawliteral\((.*?)\)rawliteral\";"
    match = re.search(pattern, content, re.DOTALL)
    if match:
        return match.group(1)
    return f"<h1>Error: Template {name} not found</h1>"


def render_setup_page(layout="integrated"):
    html = extract_template("PAGE_SETUP_TEMPLATE")

    # Replace placeholders
    wifi_opts = """
    <option value="Home-5G-LivingRoom">Home-5G-LivingRoom (-44 dBm)</option>
    <option value="Home-IoT-Mesh">Home-IoT-Mesh (-58 dBm)</option>
    <option value="Office-Guest">Office-Guest (-72 dBm)</option>
    """
    profile_opts = """
    <option value="samsung_tizen" selected>Samsung Smart TV (Tizen)</option>
    <option value="lg_webos">LG webOS</option>
    <option value="roku">Roku TV</option>
    """

    html = html.replace("%WIFI_OPTIONS%", wifi_opts)
    html = html.replace("%ROOM_NAME%", "Living Room")
    html = html.replace("%MDNS_HOSTNAME%", "tv-remote")
    html = html.replace("%AP_SSID%", "Living-Room-TV-Remote")
    html = html.replace("%MODE_STA_SELECTED%", "selected")
    html = html.replace("%MODE_AP_SELECTED%", "")
    html = html.replace("%AUTH_CHECKED%", "")
    html = html.replace("%FIRMWARE_VERSION%", "1.3.0")
    html = html.replace("%GITHUB_REPO%", "sblaisdev/lilygo-wifi-tv-remote")
    html = html.replace("%TEST_DEVICE_CHECKED%", "")
    html = html.replace("%TEST_DEVICE_BADGE_DISPLAY%", "none")
    html = html.replace("%CUSTOM_URL_DISPLAY%", "none")
    html = html.replace("%PROFILE_OPTIONS%", profile_opts)
    html = html.replace("%CRYPTO_STATUS_BADGE%", '<span class="badge">&#x1F512; Hardware eFuse HMAC Active</span>')

    # Remove old back link at the bottom
    html = html.replace("%BACK_LINK%", "")

    # Inject iOS back button CSS
    ios_css = """
    /* --- iOS Back Button Styles --- */
    .ios-back-btn {
      display: inline-flex;
      align-items: center;
      gap: 3px;
      color: var(--primary);
      text-decoration: none;
      font-size: 0.98rem;
      font-weight: 500;
      padding: 6px 8px 6px 0;
      border-radius: 6px;
      cursor: pointer;
      user-select: none;
      transition: opacity 0.15s ease, transform 0.1s ease;
      flex-shrink: 0;
      -webkit-tap-highlight-color: transparent;
    }
    .ios-back-btn:hover {
      color: #60a5fa;
    }
    .ios-back-btn:active {
      opacity: 0.45;
      transform: scale(0.96);
    }
    .ios-back-chevron {
      font-size: 1.55rem;
      line-height: 1;
      margin-top: -2px;
      font-weight: 300;
      display: inline-block;
    }
    .header-divider {
      width: 1px;
      height: 22px;
      background: rgba(255, 255, 255, 0.12);
      margin: 0 4px;
      flex-shrink: 0;
    }
    .header-brand {
      display: flex;
      align-items: center;
      gap: 10px;
      min-width: 0;
    }
    .top-nav-bar {
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 4px 0 8px 0;
      border-bottom: 1px solid rgba(255, 255, 255, 0.08);
      margin-bottom: 8px;
    }

    /* Live Preview Banner */
    .preview-switcher {
      position: sticky;
      top: 0;
      z-index: 999;
      background: #1e2536;
      border: 1px solid #3b82f640;
      border-radius: 12px;
      padding: 8px 14px;
      margin-bottom: 14px;
      display: flex;
      align-items: center;
      justify-content: space-between;
      box-shadow: 0 4px 12px rgba(0,0,0,0.4);
    }
    .preview-switcher span {
      font-size: 0.8rem;
      color: #94a3b8;
      font-weight: 600;
      text-transform: uppercase;
      letter-spacing: 0.05em;
    }
    .preview-switcher-btns {
      display: flex;
      gap: 6px;
    }
    .preview-btn {
      padding: 5px 12px;
      font-size: 0.8rem;
      font-weight: 600;
      border-radius: 6px;
      text-decoration: none;
      border: 1px solid transparent;
      transition: all 0.15s;
    }
    .preview-btn.active {
      background: #3b82f6;
      color: #ffffff;
    }
    .preview-btn:not(.active) {
      background: #141923;
      color: #94a3b8;
      border-color: #263043;
    }
    .preview-btn:not(.active):hover {
      color: #ffffff;
    }
    """
    html = html.replace("</style>", ios_css + "\n  </style>")

    # Top Switcher Bar (so user can toggle live in browser)
    is_integrated = (layout != "toprow")
    switcher_html = f"""
    <div class="preview-switcher">
      <span>Layout Style:</span>
      <div class="preview-switcher-btns">
        <a href="/setup?layout=integrated" class="preview-btn {'active' if is_integrated else ''}">Integrated</a>
        <a href="/setup?layout=toprow" class="preview-btn {'active' if not is_integrated else ''}">Top Row</a>
      </div>
    </div>
    """

    if is_integrated:
        # Integrated layout
        new_header = """
    <header style="display: flex; align-items: center; gap: 8px; padding: 4px 0 12px 0;">
      <a href="/" class="ios-back-btn" title="Back to Remote Control">
        <span class="ios-back-chevron">&#x2039;</span>
        <span>Remote</span>
      </a>
      <div class="header-divider"></div>
      <div class="header-brand">
        <img src="/icon.svg" class="brand-icon" alt="TV icon">
        <div>
          <h1>Wi-Fi &amp; Device Setup</h1>
          <small style="color: var(--text-muted); font-size: 0.75rem;">%ROOM_NAME% &bull; LilyGO T-Dongle-S3</small>
        </div>
      </div>
    </header>
        """.replace("%ROOM_NAME%", "Living Room")
    else:
        # Dedicated top row layout
        new_header = """
    <div class="top-nav-bar">
      <a href="/" class="ios-back-btn" title="Back to Remote Control">
        <span class="ios-back-chevron">&#x2039;</span>
        <span>Remote</span>
      </a>
      <span style="font-size: 0.85rem; font-weight: 500; color: var(--text-muted);">Wi-Fi &amp; Device Setup</span>
      <div style="width: 60px;"></div>
    </div>
    <header style="display: flex; align-items: center; gap: 10px; padding: 4px 0 10px 0;">
      <img src="/icon.svg" class="brand-icon" alt="TV icon">
      <div>
        <h1>Wi-Fi &amp; Device Setup</h1>
        <small style="color: var(--text-muted); font-size: 0.75rem;">%ROOM_NAME% &bull; LilyGO T-Dongle-S3</small>
      </div>
    </header>
        """.replace("%ROOM_NAME%", "Living Room")

    # Replace existing <header> block
    header_pattern = r"<header>.*?</header>"
    html = re.sub(header_pattern, new_header, html, flags=re.DOTALL)

    # Insert switcher right inside <div class="container">
    html = html.replace('<div class="container">', '<div class="container">\n' + switcher_html)

    return html


def render_remote_page():
    html = extract_template("PAGE_INDEX_TEMPLATE")
    html = html.replace("%ROOM_NAME%", "Living Room")
    html = html.replace("%AUTH_REQUIRED%", "false")
    html = html.replace("%ACTIVE_PROFILE_JSON%", '{"id":"samsung_tizen","name":"Samsung Smart TV","active":true}')
    # Add a quick link to /setup so the user can easily navigate back and forth
    link_html = '<div style="text-align:center; padding:12px;"><a href="/setup" style="color:#3b82f6; text-decoration:none; font-size:0.9rem;">⚙️ Open Setup Page</a></div>'
    html = html.replace('</body>', link_html + '</body>')
    return html


class PreviewHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        url = self.path
        query = ""
        if "?" in url:
            url, query = url.split("?", 1)

        if url == "/setup":
            layout = "toprow" if "layout=toprow" in query else "integrated"
            body = render_setup_page(layout).encode("utf-8")
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)
            return

        if url == "/" or url == "/index.html":
            body = render_remote_page().encode("utf-8")
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)
            return

        if url == "/icon.svg":
            if os.path.exists(ICON_SVG):
                with open(ICON_SVG, "rb") as f:
                    body = f.read()
                self.send_response(200)
                self.send_header("Content-Type", "image/svg+xml")
                self.send_header("Content-Length", str(len(body)))
                self.end_headers()
                self.wfile.write(body)
                return

        if url == "/tv_wizard.js":
            if os.path.exists(TV_WIZARD_JS):
                with open(TV_WIZARD_JS, "rb") as f:
                    body = f.read()
                self.send_response(200)
                self.send_header("Content-Type", "application/javascript")
                self.send_header("Content-Length", str(len(body)))
                self.end_headers()
                self.wfile.write(body)
                return

        # Mock API responses
        if url == "/api/profile/active":
            body = b'{"id":"samsung_tizen","name":"Samsung Smart TV","active":true}'
            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)
            return

        # Default fallback
        super().do_GET()

    def do_POST(self):
        # Mock successful POST response
        self.send_response(200)
        self.send_header("Content-Type", "text/html")
        body = b"<h1>Saved (Simulation)</h1><a href='/setup'>Back to Setup</a>"
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, format, *args):
        # Suppress noisy standard logs, print clean request line
        print(f"[Preview Server] {self.command} {self.path} -> {args[1] if len(args) > 1 else ''}")


def get_lan_ip():
    import socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('8.8.8.8', 80))
        ip = s.getsockname()[0]
    except Exception:
        ip = '127.0.0.1'
    finally:
        s.close()
    return ip


def run_server():
    socketserver.TCPServer.allow_reuse_address = True
    lan_ip = get_lan_ip()
    with socketserver.TCPServer(("0.0.0.0", PORT), PreviewHandler) as httpd:
        print(f"============================================================")
        print(f"  LilyGO Preview Server Running (LAN Access Enabled)!")
        print(f"  LAN Setup URL:   http://{lan_ip}:{PORT}/setup")
        print(f"  Local Setup URL: http://127.0.0.1:{PORT}/setup")
        print(f"  LAN Remote URL:  http://{lan_ip}:{PORT}/")
        print(f"============================================================", flush=True)
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            pass


if __name__ == "__main__":
    run_server()
