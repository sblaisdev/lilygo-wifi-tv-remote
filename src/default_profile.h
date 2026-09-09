#pragma once
#include <Arduino.h>

// Built-in Smart TV Remote Profile
const char DEFAULT_PROFILE_TV[] PROGMEM = R"rawliteral({
  "id": "default-tv",
  "name": "Smart TV Remote",
  "deviceType": "tv",
  "icon": "tv",
  "columns": 3,
  "pages": [
    {
      "id": "nav",
      "name": "Navigation",
      "columns": 3,
      "buttons": [
        { "label": "POWER", "icon": "power", "color": "#ef4444", "action": "hid", "code": "POWER", "span": 1 },
        { "label": "", "icon": "", "action": "none", "span": 1 },
        { "label": "MUTE", "icon": "volume-x", "color": "#64748b", "action": "hid", "code": "MUTE", "span": 1 },
        { "label": "", "icon": "", "action": "none", "span": 1 },
        { "label": "▲", "icon": "arrow-up", "color": "#334155", "action": "hid", "code": "UP", "span": 1 },
        { "label": "", "icon": "", "action": "none", "span": 1 },
        { "label": "◄", "icon": "arrow-left", "color": "#334155", "action": "hid", "code": "LEFT", "span": 1 },
        { "label": "OK", "icon": "check", "color": "#3b82f6", "action": "hid", "code": "OK", "span": 1 },
        { "label": "►", "icon": "arrow-right", "color": "#334155", "action": "hid", "code": "RIGHT", "span": 1 },
        { "label": "", "icon": "", "action": "none", "span": 1 },
        { "label": "▼", "icon": "arrow-down", "color": "#334155", "action": "hid", "code": "DOWN", "span": 1 },
        { "label": "", "icon": "", "action": "none", "span": 1 },
        { "label": "BACK", "icon": "corner-down-left", "color": "#475569", "action": "hid", "code": "BACK", "span": 1 },
        { "label": "HOME", "icon": "home", "color": "#475569", "action": "hid", "code": "HOME", "span": 1 },
        { "label": "INFO", "icon": "info", "color": "#475569", "action": "hid", "code": "TAB", "span": 1 },
        { "label": "VOL +", "icon": "volume-2", "color": "#10b981", "action": "hid", "code": "VOL_UP", "span": 1 },
        { "label": "VOL -", "icon": "volume-1", "color": "#10b981", "action": "hid", "code": "VOL_DOWN", "span": 1 },
        { "label": "PLAY", "icon": "play", "color": "#06b6d4", "action": "hid", "code": "PLAY_PAUSE", "span": 1 },
        { "label": "YouTube", "icon": "video", "color": "#e11d48", "action": "macro", "macro": "DELAY 100\nSTRING youtube\nENTER", "span": 1 },
        { "label": "Netflix", "icon": "film", "color": "#dc2626", "action": "macro", "macro": "DELAY 100\nSTRING netflix\nENTER", "span": 1 },
        { "label": "Search", "icon": "search", "color": "#8b5cf6", "action": "hid", "code": "ENTER", "span": 1 }
      ]
    },
    {
      "id": "numpad",
      "name": "Numpad",
      "columns": 3,
      "buttons": [
        { "label": "1", "action": "hid", "code": "KEY_1", "span": 1 },
        { "label": "2", "action": "hid", "code": "KEY_2", "span": 1 },
        { "label": "3", "action": "hid", "code": "KEY_3", "span": 1 },
        { "label": "4", "action": "hid", "code": "KEY_4", "span": 1 },
        { "label": "5", "action": "hid", "code": "KEY_5", "span": 1 },
        { "label": "6", "action": "hid", "code": "KEY_6", "span": 1 },
        { "label": "7", "action": "hid", "code": "KEY_7", "span": 1 },
        { "label": "8", "action": "hid", "code": "KEY_8", "span": 1 },
        { "label": "9", "action": "hid", "code": "KEY_9", "span": 1 },
        { "label": "⌫", "action": "hid", "code": "BACKSPACE", "color": "#ef4444", "span": 1 },
        { "label": "0", "action": "hid", "code": "KEY_0", "span": 1 },
        { "label": "ENTER", "action": "hid", "code": "ENTER", "color": "#3b82f6", "span": 1 }
      ]
    }
  ]
})rawliteral";

// Built-in PC Media & Productivity Profile with DuckyScript Macros
const char DEFAULT_PROFILE_PC[] PROGMEM = R"rawliteral({
  "id": "pc-media",
  "name": "Windows PC & Media",
  "deviceType": "pc",
  "icon": "monitor",
  "columns": 3,
  "pages": [
    {
      "id": "media",
      "name": "Media & Navigation",
      "columns": 3,
      "buttons": [
        { "label": "⏮ PREV", "icon": "skip-back", "color": "#334155", "action": "hid", "code": "PREV", "span": 1 },
        { "label": "⏯ PLAY", "icon": "play", "color": "#06b6d4", "action": "hid", "code": "PLAY_PAUSE", "span": 1 },
        { "label": "⏭ NEXT", "icon": "skip-forward", "color": "#334155", "action": "hid", "code": "NEXT", "span": 1 },
        { "label": "VOL -", "icon": "volume-1", "color": "#10b981", "action": "hid", "code": "VOL_DOWN", "span": 1 },
        { "label": "MUTE", "icon": "volume-x", "color": "#64748b", "action": "hid", "code": "MUTE", "span": 1 },
        { "label": "VOL +", "icon": "volume-2", "color": "#10b981", "action": "hid", "code": "VOL_UP", "span": 1 },
        { "label": "SPACE", "icon": "pause", "color": "#475569", "action": "hid", "code": "SPACE", "span": 1 },
        { "label": "FULLSCREEN", "icon": "maximize", "color": "#3b82f6", "action": "hid", "code": "F11", "span": 1 },
        { "label": "ESC", "icon": "x", "color": "#ef4444", "action": "hid", "code": "ESCAPE", "span": 1 }
      ]
    },
    {
      "id": "macros",
      "name": "Ducky Macros",
      "columns": 2,
      "buttons": [
        { "label": "Lock PC", "icon": "lock", "color": "#ef4444", "action": "macro", "macro": "GUI l", "span": 1 },
        { "label": "Desktop", "icon": "monitor", "color": "#3b82f6", "action": "macro", "macro": "GUI d", "span": 1 },
        { "label": "Task Manager", "icon": "activity", "color": "#f59e0b", "action": "macro", "macro": "CTRL SHIFT ESC", "span": 1 },
        { "label": "Alt+Tab", "icon": "layers", "color": "#8b5cf6", "action": "macro", "macro": "ALT TAB", "span": 1 },
        { "label": "Open Terminal", "icon": "terminal", "color": "#10b981", "action": "macro", "macro": "GUI r\nDELAY 150\nSTRING cmd\nENTER", "span": 2 },
        { "label": "Close Window", "icon": "x-square", "color": "#dc2626", "action": "macro", "macro": "ALT F4", "span": 2 }
      ]
    }
  ]
})rawliteral";
