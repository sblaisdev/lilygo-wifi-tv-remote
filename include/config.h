#pragma once

// ==========================================
// Device Identity Defaults (Configured via Web Portal)
// ==========================================
#define DEFAULT_ROOM_NAME       "Living Room"
#define DEFAULT_MDNS_HOSTNAME   "tv-remote"
#define DEFAULT_AP_SSID         "TV-Remote-Setup"
#define FIRMWARE_VERSION        "v1.3.2-beta"
#define GITHUB_REPO             "sblaisdev/lilygo-wifi-tv-remote"

// ==========================================
// Hardware Pin Definitions (LilyGO T-Dongle-S3)
// ==========================================
#define PIN_BUTTON              0       // Onboard user/boot button (Active LOW)
#define PIN_LCD_BL              38      // ST7735 LCD Backlight control pin (Active LOW)
#define LCD_BACKLIGHT_ON        LOW     // Active LOW on LilyGO T-Dongle-S3 hardware
#define LCD_BACKLIGHT_OFF       HIGH

// ==========================================
// Timing Thresholds
// ==========================================
#define SCREEN_TIMEOUT_MS       15000   // 15 seconds screen auto-off
#define FACTORY_RESET_HOLD_MS   10000   // 10 seconds continuous hold for reset
#define RESET_CONFIRM_TIMEOUT   20000   // 20 seconds timeout to cancel reset prompt
#define PAIRING_TIMEOUT_MS      30000   // 30 seconds to confirm new phone pairing

// ==========================================
// TF / MicroSD Card Hardware Pins
// ==========================================
#define PIN_SD_CS               13
#define PIN_SD_MOSI             11
#define PIN_SD_CLK              12
#define PIN_SD_MISO             14

#define PROFILES_DIR            "/profiles"
#define ACTIVE_PROFILE_FILE     "/profiles/active.txt"
#define DEFAULT_PROFILE_ID      "default-tv"
