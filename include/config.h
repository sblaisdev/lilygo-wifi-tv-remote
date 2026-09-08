#pragma once

// ==========================================
// Device Identity Defaults (Configured via Web Portal)
// ==========================================
#define DEFAULT_ROOM_NAME       "Living Room"
#define DEFAULT_MDNS_HOSTNAME   "tv-remote"
#define DEFAULT_AP_SSID         "TV-Remote-Setup"

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
