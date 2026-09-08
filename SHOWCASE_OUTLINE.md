# Project Showcase: Developing an ESP32-S3 USB TV Remote with Antigravity

This outline documents the complete end-to-end development cycle of the **LilyGO T-Dongle-S3 TV Remote & Keyboard**, created collaboratively between human developer and **Google Antigravity**. It highlights how Antigravity accelerates embedded systems engineering—from initial hardware mapping and firmware coding to cryptographic design, hardware-level debugging, automated flashing, bilingual documentation, and Git DevOps.

---

## Executive Summary & Engineering Metrics

| Metric | Measurement |
| :--- | :--- |
| **Total Transcript Steps** | **1,222 steps** |
| **User Interaction Turns** | **32 turns** |
| **Active Assistant Work Time** | **~68 minutes** (automated research, compilation, debugging, coding) |
| **Firmware Compilations** | **8 successful builds** |
| **On-Device Flashes** | **6 hardware flash deployments** |
| **Files Created / Maintained** | **13 project files** |
| **Target Hardware** | LilyGO T-Dongle-S3 (ESP32-S3 dual-core @ 240 MHz, 80×160 ST7735 LCD, USB HID) |
| **Repository URL** | [github.com/sblaisdev/t-dongle-s3-tv-remote](https://github.com/sblaisdev/t-dongle-s3-tv-remote) |

```mermaid
gantt
    title End-to-End Development Timeline
    dateFormat YYYY-MM-DDTHH:mm:ssZ
    axisFormat %H:%M

    section Phase 1: Inception & Drivers
    Hardware Inception & USB HID Setup       :2026-09-07T22:08:09Z, 2026-09-07T22:21:51Z
    Firmware Architecture & TFT Display     :2026-09-07T22:21:51Z, 2026-09-07T22:52:47Z

    section Phase 2: Remote & WebSockets
    Sub-millisecond Keystroke Streaming     :2026-09-07T22:52:47Z, 2026-09-07T23:34:16Z
    Mobile UX & Focus-Stealing Fixes        :2026-09-07T23:34:16Z, 2026-09-08T00:11:15Z

    section Phase 3: Security & Cryptography
    Multi-Device & UI Icon Design          :2026-09-08T00:11:15Z, 2026-09-08T00:30:01Z
    Hardware HMAC & AES-256 CTR Engine     :2026-09-08T00:30:01Z, 2026-09-08T01:16:13Z
    Provisioning Portal & Safe Reset        :2026-09-08T01:16:13Z, 2026-09-08T01:31:27Z

    section Phase 4: Hardware Debugging
    Display Backlight Investigation         :2026-09-08T01:31:27Z, 2026-09-08T01:51:22Z
    Bootloader Flash & LCD Activation       :2026-09-08T01:51:22Z, 2026-09-08T02:03:35Z

    section Phase 5: Docs & Production
    Bilingual Technical User Guides         :2026-09-08T02:03:35Z, 2026-09-08T02:05:51Z
    Simple Getting Started Guides           :2026-09-08T02:05:51Z, 2026-09-08T11:24:38Z
    Git Init, GitHub Repo & Secret Audit    :2026-09-08T11:24:38Z, 2026-09-08T13:37:38Z
```

---

## Chronological Project Log & Milestones

### Phase 1: Hardware Exploration & Architecture Design
*From a bare USB dongle to a compiling firmware skeleton.*

- **Turn 1 (22:08:09 UTC | Duration: 8m 21s)**  
  *User*: Introduced the LilyGO T-Dongle-S3 hardware specifications.  
  *Antigravity Action*: Researched chip architecture (ESP32-S3 dual-core, native USB OTG, 0.96" ST7735 SPI display on GPIOs 1-5, 38). Recommended PlatformIO setup with `esp32-s3-devkitc-1`, `TFT_eSPI`, and native USB CDC/HID flags.
- **Turn 2 (22:16:30 UTC | Duration: 5m 21s)**  
  *User*: Requested a remote keyboard emulator for TV navigation and typing.  
  *Antigravity Action*: Designed the USB HID architecture utilizing `USBHIDKeyboard` and `USBHIDConsumerControl`, defining keyboard shortcuts, media playback, and volume control.
- **Turn 3 (22:21:51 UTC | Duration: 30m 56s)**  
  *User*: Requested the on-board display stay dark by default with a physical button to wake it up for 15 seconds.  
  *Antigravity Action*: Created [config.h](file:///c:/antigravity/lilygo/include/config.h), [main.cpp](file:///c:/antigravity/lilygo/src/main.cpp), and initial web UI. Added power-saving display state machine. Compiled firmware successfully.

---

### Phase 2: Live Keystroke Streaming & Mobile UX
*Achieving sub-millisecond typing and mobile browser optimization.*

- **Turn 4 (22:52:47 UTC | Duration: 8m 03s)**  
  *User*: Requested live typing where keystrokes stream to the TV as the user types on their phone.  
  *Antigravity Action*: Upgraded HTTP polling to a dedicated **WebSocket server on port 81** (`WebSocketsServer`). Implemented dual-mode typing: real-time character streaming + bulk text submission.
- **Turns 5–7 (23:00:50 – 23:27:59 UTC | Duration: 33m 26s)**  
  *User & Antigravity*: Connected the hardware over COM port, compiled, and deployed the live typing firmware.
- **Turn 8 (23:34:16 UTC | Duration: 4m 25s)**  
  *User*: Reported that clicking D-pad buttons dismissed the phone's virtual keyboard.  
  *Antigravity Action*: Refactored the web frontend JavaScript: prevented default button focus loss with `preventDefault()` and `touch-action: manipulation`, keeping the mobile keyboard permanently open during navigation.
- **Turns 9–10 (23:38:41 – 23:45:27 UTC | Duration: 32m 34s)**  
  *User*: Inquired whether Windows recognized the device as a genuine HID keyboard.  
  *Antigravity Action*: Queried the Windows device registry via PowerShell, verifying official registration under `HIDClass` (`VID_303A`, `PID_1001`). Corrected USB HID descriptor endpoints.

---

### Phase 3: Scalability, UI Polish & Progressive Web App
*Multi-device house support and mobile app installation.*

- **Turn 11 (00:11:15 UTC | Duration: 7m 10s)**  
  *User*: Asked how multiple TV remotes would coexist on the same home Wi-Fi network.  
  *Antigravity Action*: Implemented dynamic device naming: configurable **Room Names** (e.g. `Living Room TV`) and dynamic **mDNS hostnames** (`http://<custom-name>.local`).
- **Turn 12 (00:18:25 UTC | Duration: 11m 36s)**  
  *User*: Requested an app icon.  
  *Antigravity Action*: Generated a custom neon TV remote SVG icon ([icon.svg](file:///c:/antigravity/lilygo/icon.svg)), embedded a PWA manifest, and created an interactive HTML preview artifact.

---

### Phase 4: Embedded Cryptography & Hardware Security Architecture
*Adopting the zero-cleartext standard using the ESP32-S3 Hardware HMAC engine.*

- **Turns 13–16 (00:30:01 – 00:54:45 UTC | Duration: 32m 02s)**  
  *User*: Inquired about eliminating hardcoded Wi-Fi credentials and how to securely provision secrets via a web portal.  
  *Antigravity Action*: Explored hardware flash encryption, eFuse block burning vs. NVS, and potential attack vectors (physical flash dumping).
- **Turns 17–20 (01:01:59 – 01:13:38 UTC | Duration: 14m 14s)**  
  *User*: Proposed using an ESP32-S3 eFuse slot for HMAC key derivation, with automatic detection/reuse of existing keys across future projects, and requested Antigravity record this as the permanent project rule.  
  *Antigravity Action*: Architected the **ESP32 Security Architecture** standard in [GEMINI.md](file:///c:/antigravity/lilygo/GEMINI.md):
  1. *Zero-Cleartext Credential Storage* in NVS.
  2. *Hardware HMAC Auto-Detection*: scans `KEY0`–`KEY5` for `ESP_EFUSE_KEY_PURPOSE_HMAC_UP` and reuses without burning new fuses.
  3. *Domain Separation*: passes feature context (e.g., `"project-wifi-v1"`) into `esp_hmac_calculate()` to derive 256-bit AES keys.
  4. *Physical Proximity Barrier*: TRNG-generated 8-character random password displayed exclusively on the physical LCD.
  5. *10-Second Button Hold Factory Reset*: two-step confirmation protecting eFuses.
- **Turn 21 (01:16:13 UTC | Duration: 15m 14s)**  
  *User*: Approved the plan.  
  *Antigravity Action*: Implemented the complete cryptographic suite (`soc/esp32s3/esp_hmac.h`, `mbedtls/aes.h`), web setup portal (`/setup`, `/savewifi`), and factory reset state machine.

---

### Phase 5: Autonomous Hardware Debugging & Polarity Fix
*Solving the active-LOW display backlight anomaly.*

- **Turns 22–25 (01:31:27 – 01:35:59 UTC | Duration: 6m 29s)**  
  *User*: Requested keeping the display permanently ON in AP mode, enlarging the password, and removing "Setup AP MODE". After flashing, reported: *"the display is black"*.
- **Turn 26 (01:37:56 UTC | Duration: 13m 26s)**  
  *User*: *"it does not. I unplugged it and plugged in back in and still nothing"*.  
  *Antigravity Autonomous Debugging*:
  1. Tested serial output on `COM7` via Python `pyserial` to rule out CPU panics or eFuse write aborts.
  2. Searched the web and fetched LilyGO's official factory firmware (`factory_screen.ino`) and driver (`esp_lcd_st7735.c`) using GitHub API.
  3. **Discovered the hardware anomaly**: Lines 60–61 of `factory_screen.ino` explicitly define:
     ```c
     #define LCD_BK_LIGHT_ON   0   // Active LOW!
     #define LCD_BK_LIGHT_OFF  1
     ```
  4. Setting GPIO 38 to `HIGH` was turning the backlight **OFF**.
  5. Updated `config.h`, `platformio.ini` (`-D TFT_BACKLIGHT_ON=LOW`), and `main.cpp`.
- **Turn 27 (01:51:22 UTC | Duration: 12m 13s)**  
  *User & Antigravity*: Guided user to enter ROM bootloader mode (holding button while plugging into USB) -> flashed `COM5` -> Display lit up with full clarity!

---

### Phase 6: Bilingual Documentation & Plain-Language Quick Starts
*Creating professional technical guides and accessible user documentation.*

- **Turn 28 (02:03:35 UTC | Duration: 2m 16s)**  
  *User*: *"create an english and french user guide"*.  
  *Antigravity Action*: Authored exhaustive technical manuals covering hardware compatibility, provisioning, PWA install, security architecture, and troubleshooting:
  - 🇬🇧 [USER_GUIDE_EN.md](file:///c:/antigravity/lilygo/USER_GUIDE_EN.md) (12.5 KB)
  - 🇫🇷 [USER_GUIDE_FR.md](file:///c:/antigravity/lilygo/USER_GUIDE_FR.md) (15.1 KB)
  - 📋 [README.md](file:///c:/antigravity/lilygo/README.md)
- **Turn 29 (02:05:51 UTC | Duration: 2m 47s)**  
  *User*: *"create a getting started guide (how to configure and accessing the remote page) use simple terms"*.  
  *Antigravity Action*: Authored jargon-free, 4-step quick-start guides with clear analogies and step-by-step instructions:
  - 🇬🇧 [GETTING_STARTED.md](file:///c:/antigravity/lilygo/GETTING_STARTED.md)
  - 🇫🇷 [DEMARRAGE_RAPIDE.md](file:///c:/antigravity/lilygo/DEMARRAGE_RAPIDE.md)

---

### Phase 7: Production DevOps & Comprehensive Security Audit
*Git repository creation, remote publishing, and automated secret scanning.*

- **Turn 30 (11:24:38 UTC | Duration: 1m 59s)**  
  *User*: *"do you have a github repo for this project? if not create one"*.  
  *Antigravity Action*: Checked GitHub CLI authentication, created `.gitignore`, initialized Git repository, created remote repository on GitHub via `gh repo create`, and pushed `main` branch:
  - **Live Repo**: [github.com/sblaisdev/t-dongle-s3-tv-remote](https://github.com/sblaisdev/t-dongle-s3-tv-remote)
- **Turn 31 (11:26:37 UTC | Duration: 1m 01s)**  
  *User*: *"search the repo for secrets"*.  
  *Antigravity Action*: Executed a comprehensive regex and commit-history scan across all 12 tracked files for Wi-Fi credentials, tokens, API keys, private keys, and IP leaks. Confirmed **100% clean repository**.

---

## Why Antigravity Excels at Embedded ESP32 Development

```
┌────────────────────────────────────────────────────────────────────────┐
│                   ANTIGRAVITY EMBEDDED WORKFLOW                        │
├──────────────────┬──────────────────┬──────────────────┬───────────────┤
│ 1. HARDWARE      │ 2. CRYPTO        │ 3. AUTONOMOUS    │ 4. FULL-STACK │
│    INTELLIGENCE  │    ENGINEERING   │    DEBUGGING     │    LIFECYCLE  │
├──────────────────┼──────────────────┼──────────────────┼───────────────┤
│ • Pinout mapping │ • Hardware HMAC  │ • Serial testing │ • PWA Remote  │
│ • Display timing │ • AES-256 CTR    │ • Driver reverse │ • C++ / ESP32 │
│ • USB HID stacks │ • eFuse reuse    │   engineering    │ • DevOps / Git│
│ • PlatformIO     │ • Proximity TRNG │ • Active-LOW fix │ • Docs (EN/FR)│
└──────────────────┴──────────────────┴──────────────────┴───────────────┘
```

1. **Hardware-Aware Pair Programming**: Instead of generic code, Antigravity directly understands ESP32-S3 registers, USB OTG modes, SPI buses, and eFuse controllers.
2. **Autonomous Physical Root-Cause Debugging**: When the screen went black, rather than guessing, Antigravity fetched the manufacturer's low-level driver repository, discovered the active-LOW GPIO 38 inversion, and fixed it directly.
3. **Hardware Cryptography without Compromise**: Designed a zero-cleartext embedded security model using the on-chip HMAC peripheral that protects user privacy even against raw SPI flash dumping.
4. **End-to-End Delivery**: Took a bare-metal microcontroller project and delivered a complete product: firmware, responsive mobile web app, bilingual guides, and a GitHub repository.
