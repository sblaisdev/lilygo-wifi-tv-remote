# LilyGO T-Dongle-S3 TV Remote & Keyboard

Firmware for the **LilyGO T-Dongle-S3** (ESP32-S3) turning the USB dongle into a zero-driver USB HID remote control and wireless keyboard for Smart TVs, streaming boxes, and PCs, controllable from any smartphone or web browser.

---

## 📖 User Guides / Guides d'utilisation

### 🚀 Simple Quick Start / Démarrage Rapide
- 🇬🇧 **[Getting Started Guide (Simple Terms)](GETTING_STARTED.md)** : Step-by-step setup and remote access in plain, non-technical language.
- 🇫🇷 **[Guide de Démarrage Rapide (Termes Simples)](DEMARRAGE_RAPIDE.md)** : Configuration et accès à la télécommande expliqués simplement.

### 📚 Full Technical Documentation / Documentation Complète
- 🇬🇧 **[Full User Guide](USER_GUIDE_EN.md)**: Hardware architecture, PWA installation, security & crypto, factory reset, troubleshooting.
- 🇫🇷 **[Guide Complet de l'utilisateur](USER_GUIDE_FR.md)** : Architecture matérielle, installation PWA, sécurité et cryptographie, réinitialisation, dépannage.

---

## 🚀 Key Highlights

1. **Physical Proximity Barrier & Device Pairing**: Setup Access Point (`TV-Remote-Setup`) uses a cryptographically secure 8-character random password generated via the ESP32-S3 TRNG and shown **exclusively on the physical 160×80 LCD**. Optional **Hardware Device Pairing (Method B)** requires physical button confirmation on the dongle before any client device can transmit keystrokes.
2. **Zero Cleartext Credentials & Fail-Closed Security**: Wi-Fi credentials are encrypted using **AES-256 CTR** via the ESP32-S3 **Hardware HMAC Peripheral** (`KEY0`–`KEY5`) with automatic key slot reuse and strict fail-closed validation (no unencrypted fallbacks).
3. **In-Transit Keystroke Encryption**: Keystrokes transmitted over WebSocket/HTTP are encrypted client-side using browser-native **WebCrypto AES-CTR** before broadcast over the local network.
4. **Sub-Millisecond Web Remote**: WebSocket-powered remote control with D-Pad navigation, media controls, volume, power, and live phone-to-TV typing.
5. **Standalone AP or Station Mode**: Connect to your existing home Wi-Fi or configure a permanent standalone hotspot for direct phone-to-TV control while traveling.
6. **PWA Ready**: Installable to iOS and Android home screens with a custom neon TV remote icon.
7. **Fail-Safe Factory Reset**: 10-second button hold with two-step confirmation erases NVS flash while preserving hardware eFuses.
8. **Universal Companion Studio & DuckyScript Macros**: An intuitive layout & macro designer companion app (`/designer` or `companion/index.html`) to visually build custom button grids and inject automated keystroke macros directly over Wi-Fi. Profiles are saved on-chip in LittleFS with automatic MicroSD card detection.

