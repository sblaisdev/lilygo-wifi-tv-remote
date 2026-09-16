# User Preferences & Standards: ESP32 Security Architecture

For all ESP32 / ESP32-S3 firmware and embedded projects, always enforce the following security, provisioning, and key-management standards:

## 1. Zero-Cleartext Credential Storage
- Never hardcode personal Wi-Fi SSIDs, passwords, API tokens, or secrets in source files (e.g. `config.h`, `main.cpp`) or git commits.
- Credentials must be provisioned dynamically via a web setup portal / captive portal and stored in ESP32 NVS (Non-Volatile Storage) in flash memory.

## 2. Hardware HMAC Master Key & Auto-Detection Pattern
- Use the ESP32-S3 **Hardware HMAC Peripheral** (`esp_hmac.h`) to derive encryption keys for stored credentials (AES-256).
- **Auto-Detect & Reuse**:
  - Always scan eFuse key blocks (`KEY0` through `KEY5`) for `ESP_EFUSE_KEY_PURPOSE_HMAC_UP`.
  - If an HMAC key block already exists on the chip (from a previous flash or project), **immediately reuse it** without burning any new fuses.
  - If no HMAC slot exists, allocate the first empty key block (scanning in reverse starting from `KEY5` downwards to leave `KEY0`/`KEY1` free for standard Espressif tooling) and provision it with a cryptographically secure 256-bit random key using the hardware TRNG (`esp_fill_random()`).
- **Domain Separation**:
  - Never use the raw master secret directly. Always pass a project/context string (e.g., `"project-wifi-v1"`) into `esp_hmac_calculate()` to derive independent 256-bit keys for each feature or project.

## 3. Fallback AP Security & Physical Proximity Barrier
- In unconfigured or fallback Access Point mode, generate an 8-character random WPA2 password using the hardware TRNG (`esp_random()`).
- Display this password **only** on the device's physical screen (ST7735 / OLED / e-Paper).
- Require physical proximity to the device to configure it; prevent neighborhood snooping.

## 4. Hardware Button Factory Reset Safety
- Require a **10-second continuous button press** to initiate factory reset.
- Display an explicit warning on the screen: *"Press button to reset to factory default. Unplug to cancel."*
- Only perform `preferences.clear()` upon a second confirmed press; preserve hardware eFuses.

## 5. Release Strategy & OTA Gatekeeping
- **Strict Development / Release Separation**:
  - Commits to `main` or feature branches must **never** trigger OTA updates to end-user devices.
  - Releases are triggered **exclusively** by explicitly publishing a GitHub Release (`release: [published]` in GitHub Actions).
  - Production users must only see official, non-draft, non-prerelease versions (`!draft && !prerelease`).
- **Version Selector & Rollback**:
  - The firmware setup portal must always pre-select the latest stable release by default, while keeping previous releases selectable for rollback.
- **Test Device Channel**:
  - Beta, release candidates (`-rc`, `-beta`), and custom firmware URLs must remain isolated to devices with persistent **Test Device Mode** (`test_dev` in NVS) enabled.
- **Version Bumping**:
  - Update `#define FIRMWARE_VERSION` in `include/config.h` only when explicitly preparing a new release or version milestone, ensuring it matches the GitHub release tag.

