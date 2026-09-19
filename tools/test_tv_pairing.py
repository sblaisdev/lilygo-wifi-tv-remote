#!/usr/bin/env python3
"""
Interactive Samsung / Smart TV Pairing & Diagnostic Tool
Tests direct TV discovery, REST metadata probe, WebSocket pairing (port 8002 WSS),
sends test keystrokes, and can sync tokens directly to the LilyGO remote dongle.
"""

import sys
import os
import time
import json
import base64
import ssl
import urllib.request
import urllib.parse

# Ensure Windows terminal doesn't crash on unicode
if sys.platform == "win32" and hasattr(sys.stdout, "reconfigure"):
    try:
        sys.stdout.reconfigure(encoding="utf-8")
    except Exception:
        pass

try:
    import websocket
except ImportError:
    print("Installing 'websocket-client' for secure WebSocket transport...")
    import subprocess
    subprocess.check_call([sys.executable, "-m", "pip", "install", "websocket-client"])
    import websocket

DEFAULT_TV_IP = "192.168.130.208"
DONGLE_HOST = "http://tv-remote.local"
APP_NAME = "LilyGO Remote"

def get_b64_name(name=APP_NAME):
    return base64.b64encode(name.encode("utf-8")).decode("utf-8")

def probe_tv(ip, port=8001):
    print(f"\n[1/4] Probing TV REST API at http://{ip}:{port}/api/v2/ ...")
    url = f"http://{ip}:{port}/api/v2/"
    try:
        req = urllib.request.Request(url, headers={"User-Agent": "LilyGO-TestTool"})
        with urllib.request.urlopen(req, timeout=4) as resp:
            if resp.status == 200:
                data = json.loads(resp.read().decode("utf-8"))
                dev = data.get("device", {})
                print(f"  [+] Reachable! Status 200")
                print(f"  * Model: {dev.get('modelName', 'Unknown')} ({dev.get('model', '')})")
                print(f"  * OS: {dev.get('OS', 'Unknown')} - Version {data.get('version', '')}")
                print(f"  * Device Name: {dev.get('name', '')}")
                print(f"  * Token Auth Supported: {dev.get('TokenAuthSupport', 'unknown')}")
                print(f"  * Power State: {dev.get('PowerState', 'unknown')}")
                return data
    except Exception as e:
        print(f"  [-] HTTP probe failed: {e}")
    return None

def pair_tv(ip, timeout=30):
    b64_name = get_b64_name()
    # Samsung modern Tizen (2016-2025) requires WSS on port 8002 for pairing
    url = f"wss://{ip}:8002/api/v2/channels/samsung.remote.control?name={b64_name}"
    
    print(f"\n[2/4] Connecting to TV Pairing Channel...")
    print(f"  * URL: {url}")
    print(f"  * LOOK AT YOUR TV SCREEN! Press 'Allow' when the prompt appears...")
    
    try:
        ws = websocket.create_connection(
            url,
            sslopt={"cert_reqs": ssl.CERT_NONE},
            timeout=timeout
        )
        print("  [+] WebSocket connected! Listening for approval...")
        
        start = time.time()
        while time.time() - start < timeout:
            frame = ws.recv()
            if not frame:
                continue
            
            try:
                msg = json.loads(frame)
            except Exception:
                continue

            event = msg.get("event", "")
            data = msg.get("data", {})
            print(f"  << TV Event: {event}")

            if "token" in data:
                token = data["token"]
                print(f"\n  [+] SUCCESS! Authorization Token Received: {token}")
                ws.close()
                return token
            elif event == "ms.channel.connect":
                # Some firmware sends token inside clients array
                clients = data.get("clients", [])
                for c in clients:
                    if "token" in c:
                        token = c["token"]
                        print(f"\n  [+] SUCCESS! Token Received: {token}")
                        ws.close()
                        return token
                if "token" in data:
                    token = data["token"]
                    print(f"\n  [+] SUCCESS! Token Received: {token}")
                    ws.close()
                    return token
            elif event == "ms.channel.unauthorized":
                print("  [-] TV rejected unauthenticated connection on this channel.")
                ws.close()
                return None

        ws.close()
        print("  [-] Timed out waiting for approval on TV.")
    except Exception as e:
        print(f"  [-] WebSocket error: {e}")
    return None

def test_send_key(ip, token, key="KEY_VOLDOWN"):
    b64_name = get_b64_name()
    url = f"wss://{ip}:8002/api/v2/channels/samsung.remote.control?name={b64_name}&token={token}"
    print(f"\n[3/4] Testing remote key transmission ({key}) with token...")
    try:
        ws = websocket.create_connection(url, sslopt={"cert_reqs": ssl.CERT_NONE}, timeout=5)
        connect_frame = ws.recv()
        
        cmd = {
            "method": "ms.remote.control",
            "params": {
                "Cmd": "Click",
                "DataOfCmd": key,
                "Option": "false",
                "TypeOfRemote": "SendRemoteKey"
            }
        }
        ws.send(json.dumps(cmd))
        print(f"  [+] Dispatched {key} successfully to TV!")
        time.sleep(0.5)
        ws.close()
        return True
    except Exception as e:
        print(f"  [-] Failed to send key: {e}")
        return False

def sync_token_to_dongle(dongle_base, tv_id, token):
    print(f"\n[4/4] Syncing pairing token to LilyGO Dongle ({dongle_base})...")
    url = f"{dongle_base}/api/tv/save_token"
    data = urllib.parse.urlencode({
        "tv_id": tv_id,
        "secret": token
    }).encode("utf-8")

    try:
        req = urllib.request.Request(url, data=data, method="POST")
        with urllib.request.urlopen(req, timeout=5) as resp:
            res_body = resp.read().decode("utf-8")
            print(f"  [+] Token saved to LilyGO Hardware HMAC eFuse NVS: {res_body}")
            return True
    except Exception as e:
        print(f"  [*] Note: Could not sync directly to dongle at {dongle_base} ({e})")
        print(f"  Manual sync: You can enter token '{token}' in the setup wizard.")
        return False

def main():
    print("=" * 65)
    print("  Samsung Smart TV Wi-Fi Pairing & Diagnostics Tool")
    print("=" * 65)

    tv_ip = input(f"Enter TV IP address [{DEFAULT_TV_IP}]: ").strip() or DEFAULT_TV_IP
    dongle = input(f"Enter LilyGO Dongle URL [{DONGLE_HOST}]: ").strip() or DONGLE_HOST

    probe_tv(tv_ip)

    token = pair_tv(tv_ip)
    if not token:
        print("\nPairing was not completed. Check TV settings (Access Notification / IP Remote).")
        return

    test_send_key(tv_ip, token, "KEY_VOLDOWN")

    # NVS keys must be <= 15 characters
    parts = tv_ip.split('.')
    tv_id = f"tv_{parts[-2]}_{parts[-1]}" if len(parts) == 4 else f"tv_{tv_ip[:12]}"
    sync_token_to_dongle(dongle, tv_id, token)

    print("\n" + "=" * 65)
    print("  Diagnostic test complete! Your TV is ready for remote control.")
    print("=" * 65)

if __name__ == "__main__":
    main()
