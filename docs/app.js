// =========================================================================
// LilyGO TV Remote & Macro Studio - Core Engine & Multilingual System
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
          { label: "", icon: "arrow-up", color: "#334155", action: "hid", code: "UP", span: 1 },
          { label: "", action: "none", span: 1 },
          { label: "", icon: "arrow-left", color: "#334155", action: "hid", code: "LEFT", span: 1 },
          { label: "OK", icon: "check", color: "#3b82f6", action: "hid", code: "OK", span: 1 },
          { label: "", icon: "arrow-right", color: "#334155", action: "hid", code: "RIGHT", span: 1 },
          { label: "", action: "none", span: 1 },
          { label: "", icon: "arrow-down", color: "#334155", action: "hid", code: "DOWN", span: 1 },
          { label: "", action: "none", span: 1 },
          { label: "BACK", icon: "corner-down-left", color: "#475569", action: "hid", code: "BACK", span: 1 },
          { label: "HOME", icon: "home", color: "#475569", action: "hid", code: "HOME", span: 1 },
          { label: "INFO", icon: "info", color: "#475569", action: "hid", code: "TAB", span: 1 },
          { label: "VOL +", icon: "volume-2", color: "#10b981", action: "hid", code: "VOL_UP", span: 1 },
          { label: "VOL -", icon: "volume-1", color: "#10b981", action: "hid", code: "VOL_DOWN", span: 1 },
          { label: "PLAY", icon: "play", color: "#06b6d4", action: "hid", code: "PLAY_PAUSE", span: 1 },
          { label: "YouTube", icon: "video", color: "#e11d48", action: "macro", macro: "DELAY 100\nSTRING youtube\nENTER", span: 1 },
          { label: "Netflix", icon: "film", color: "#dc2626", action: "macro", macro: "DELAY 100\nSTRING netflix\nENTER", span: 1 },
          { label: "CLEAR", icon: "x-square", color: "#8b5cf6", action: "hid", code: "CLEAR", span: 1 }
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

// =========================================================================
// Internationalization (I18N)
// =========================================================================
const I18N = {
  en: {
    langBtn: "🇫🇷 FR",
    btnModeSimple: "⚡ Simple",
    btnModeAdvanced: "🛠️ Advanced",
    httpsBanner: '🌐 <strong>Hosted on GitHub Pages (HTTPS)</strong>: Web browsers block direct Wi-Fi network calls to local HTTP devices. Use <strong>"Export JSON"</strong> to upload via <a href="http://tv-remote.local/setup" target="_blank">tv-remote.local/setup</a>, or use <strong>"Deploy via USB"</strong>!',
    studioTitle: "LiLyGO Remote Studio",
    studioSubtitle: "Universal Layout & DuckyScript Macro Designer",
    targetHostPlaceholder: "Target IP or mDNS",
    btnConnect: "Connect",
    btnLoadDongle: "📥 Load from LilyGO",
    btnWifiDeploy: "🚀 Save to LilyGO",
    btnPair: "🔒 Pair Dongle",
    btnImport: "📂 Import JSON",
    btnCopy: "📋 Copy JSON",
    btnExport: "💾 Export JSON",
    btnUsbDeploy: "⚡ USB Deploy",
    loadTemplate: "Load Template",
    optTplChoose: "-- Choose a Preset Template --",
    optGroupSaved: "Saved on LilyGO",
    optGroupDefaults: "Default Templates",
    optTplTv: "Smart TV Remote",
    optTplPc: "Windows PC & Media Center",
    optTplMac: "macOS Media & Shortcuts",
    optTplAndroid: "Android / Google TV",
    optTplKodi: "Kodi / Home Theater PC",
    optTplSysadmin: "SysAdmin / Ducky Toolkit",
    optTplBlank: "Blank Layout (Custom)",
    profileSettings: "Profile Settings",
    profileId: "Profile ID (alphanumeric)",
    profileName: "Profile Name",
    profileDevice: "Target Device",
    optDevTv: "Smart TV",
    optDevPc: "Windows PC",
    optDevMac: "Apple Mac",
    optDevAndroid: "Android TV",
    optDevOther: "Universal Device",
    pagesTitle: "Pages",
    btnAddPage: "+ Add Page",
    pageCols: "Grid Columns for This Page",
    optCols2: "2 Columns",
    optCols3: "3 Columns (Default)",
    optCols4: "4 Columns",
    optCols5: "5 Columns",
    optCols6: "6 Columns",
    btnAddButton: "+ Add Button",
    btnAddSpacer: "+ Add Spacer",
    liveTestDongle: "⚡ Live Test on Dongle",
    simEncrypted: "🔒 Encrypted",
    buttonInspector: "Button Inspector",
    noSelectionMsg: "Select any button or spacer on the phone screen to edit its properties and macros.",
    actionType: "Action Type",
    optActHid: "Single HID Key (TV / PC Key)",
    optActMacro: "DuckyScript Macro (Script)",
    optActNone: "Empty Spacer",
    btnLabel: "Button Label",
    btnIcon: "Icon / Symbol",
    optIconNone: "None (Label only)",
    optIconUp: "▲ Up",
    optIconDown: "▼ Down",
    optIconLeft: "◀ Left",
    optIconRight: "▶ Right",
    optIconCheck: "✔ Check / OK",
    optIconPower: "⏻ Power",
    optIconVolUp: "🔊 Vol +",
    optIconVolDown: "🔉 Vol -",
    optIconMute: "🔇 Mute",
    optIconPlay: "▶ Play",
    optIconPause: "⏸ Pause",
    optIconPrev: "⏮ Prev Track",
    optIconNext: "⏭ Next Track",
    optIconHome: "🏠 Home",
    optIconBack: "⏎ Back / Return",
    optIconLock: "🔒 Lock",
    optIconTerminal: "▶_ Terminal",
    optIconMonitor: "🖥 Desktop / Monitor",
    optIconActivity: "📊 Task Manager",
    optIconVideo: "🎥 YouTube / Video",
    optIconFilm: "🎬 Netflix / Film",
    optIconSearch: "🔍 Search",
    btnImportCustomIcon: "🖼️ Import Image Icon",
    customIconLoaded: "Custom image loaded",
    btnSpan: "Grid Span (Width)",
    optSpan1: "1 Column (Standard)",
    optSpan2: "2 Columns (Wide)",
    optSpan3: "3 Columns (Full Width on 3-col)",
    optSpan4: "4 Columns (Full Width on 4-col)",
    colorPalette: "Color Palette",
    hidKeyLabel: "Target HID Key",
    tabBtnVisual: "Visual Builder",
    tabBtnRaw: "Raw DuckyScript",
    btnAddTypeText: "+ Type Text",
    btnAddCombo: "+ Key Combo",
    btnAddDelay: "+ Delay (ms)",
    btnAddEnter: "+ Enter",
    rawDuckyHelp: "Supports standard DuckyScript syntax: STRING, DELAY, GUI, CTRL, ALT, SHIFT, ENTER, REPEAT.",
    btnTestMacro: "⚡ Test Macro Now",
    btnMoveUp: "◀ Move Up",
    btnMoveDown: "Move Down ▶",
    btnDelete: "Delete",
    pairModalTitle: "Pair with LilyGO T-Dongle-S3",
    pairModalDesc: 'To manage layouts and inject macros, click <strong>"Request Pairing"</strong> below, then <strong>press the physical button on the TV stick</strong> to approve.',
    btnRequestPairing: "Request Pairing",
    btnCancel: "Cancel",
    spacer: "Spacer",
    page: "Page",
    rename: "Rename",
    delete: "Delete",
    enterPageName: "Enter page name:",
    renamePagePrompt: "Rename page:",
    deletePageConfirm: 'Delete page "{name}"?',
    noActionsYet: "No actions yet. Click buttons below to add steps.",
    typeTextPrompt: "Text to type automatically:",
    keyComboPrompt: "Key combo (e.g. GUI r, CTRL SHIFT ESC, ALT F4):",
    delayPrompt: "Delay milliseconds (e.g. 200):",
    tplConfirm: 'Load preset template "{name}"? Current changes will be overwritten.',
    profileImportSuccess: "Profile imported successfully!",
    invalidProfileJson: "Invalid profile JSON structure.",
    failParseJson: "Failed to parse JSON file: ",
    copiedToast: "Profile JSON copied to clipboard! 📋",
    copyPrompt: "Copy the profile JSON below:",
    webSerialNotSupported: "WebSerial is not supported in this browser. Please open this page in Google Chrome or Microsoft Edge to deploy directly via USB.",
    usbProfileSentToast: "🎉 Profile sent to LilyGO via USB!",
    usbProfileSentAlert: '🎉 Profile "{name}" successfully sent to LilyGO over USB Serial!',
    usbNotice: "USB Deploy notice: ",
    httpsMixedContentAlert: '⚠️ HTTPS Mixed-Content Notice:\n\nBrowsers block direct Wi-Fi network requests from HTTPS sites (like GitHub Pages) to local HTTP devices.\n\nTo deploy to your dongle:\n1. Click "Export JSON" (or "Copy JSON").\n2. Open your dongle setup page (http://tv-remote.local/setup).\n3. Select the file in "Upload Profile JSON" and click Save.\n\nOr click "⚡ USB Deploy" to sync directly over USB!',
    tokenNotFoundConfirm: "Pairing token not found. You must pair this companion studio with the LilyGO first. Open pairing dialog now?",
    deploySuccessAlert: '🎉 Success! Profile "{name}" deployed and set as ACTIVE on the LilyGO!',
    deployFailedAlert: "Deployment failed: ",
    cannotConnectAlert: "Could not connect to LilyGO at {host}. Ensure you are on the same Wi-Fi network.",
    pairRequestSent: "Request sent! Press the button on the LilyGO stick now...",
    pairApproved: "Device Approved & Paired! 🎉",
    pairTimedOut: "Pairing timed out. Try again.",
    pairCannotContact: "Could not contact LilyGO at ",
    connDotConnected: "Connected to LilyGO",
    loadingFromDongle: "Fetching active layout from LilyGO...",
    loadSuccessToast: "Active profile loaded from LilyGO! 🎉",
    loadFailedAlert: "Could not load profile from LilyGO at {host}.",
  },
  fr: {
    langBtn: "🇬🇧 EN",
    btnModeSimple: "⚡ Simple",
    btnModeAdvanced: "🛠️ Avancé",
    httpsBanner: '🌐 <strong>Hébergé sur GitHub Pages (HTTPS)</strong> : Les navigateurs bloquent les requêtes Wi-Fi directes vers les appareils HTTP locaux. Utilisez <strong>« Exporter JSON »</strong> pour importer via <a href="http://tv-remote.local/setup" target="_blank">tv-remote.local/setup</a>, ou utilisez <strong>« Déployer via USB »</strong> !',
    studioTitle: "LiLyGO Remote Studio",
    studioSubtitle: "Concepteur universel d'interfaces et de macros DuckyScript",
    targetHostPlaceholder: "IP cible ou mDNS",
    btnConnect: "Connecter",
    btnLoadDongle: "📥 Charger depuis le LilyGO",
    btnWifiDeploy: "🚀 Enregistrer sur le LilyGO",
    btnPair: "🔒 Associer le dongle",
    btnImport: "📂 Importer JSON",
    btnCopy: "📋 Copier JSON",
    btnExport: "💾 Exporter JSON",
    btnUsbDeploy: "⚡ Déployer via USB",
    loadTemplate: "Modèles prédéfinis",
    optTplChoose: "-- Choisir un modèle prédéfini --",
    optGroupSaved: "Enregistrés sur le LilyGO",
    optGroupDefaults: "Modèles par défaut",
    optTplTv: "Télécommande Smart TV",
    optTplPc: "PC Windows & Centre Multimédia",
    optTplMac: "macOS Multimédia & Raccourcis",
    optTplAndroid: "Android / Google TV",
    optTplKodi: "Kodi / PC Home Cinéma",
    optTplSysadmin: "Boîte à outils SysAdmin / Ducky",
    optTplBlank: "Disposition vierge (Personnalisée)",
    profileSettings: "Paramètres du profil",
    profileId: "Identifiant du profil (alphanumérique)",
    profileName: "Nom du profil",
    profileDevice: "Appareil cible",
    optDevTv: "Smart TV",
    optDevPc: "PC Windows",
    optDevMac: "Apple Mac",
    optDevAndroid: "Android TV",
    optDevOther: "Appareil universel",
    pagesTitle: "Pages",
    btnAddPage: "+ Ajouter une page",
    pageCols: "Colonnes de grille pour cette page",
    optCols2: "2 colonnes",
    optCols3: "3 colonnes (Par défaut)",
    optCols4: "4 colonnes",
    optCols5: "5 colonnes",
    optCols6: "6 colonnes",
    btnAddButton: "+ Ajouter un bouton",
    btnAddSpacer: "+ Ajouter un espaceur",
    liveTestDongle: "⚡ Test en direct sur le dongle",
    simEncrypted: "🔒 Chiffré",
    buttonInspector: "Inspecteur de bouton",
    noSelectionMsg: "Sélectionnez un bouton ou un espaceur sur l'écran du téléphone pour modifier ses propriétés et macros.",
    actionType: "Type d'action",
    optActHid: "Touche HID unique (TV / PC)",
    optActMacro: "Macro DuckyScript (Script)",
    optActNone: "Espaceur vide",
    btnLabel: "Texte du bouton",
    btnIcon: "Icône / Symbole",
    optIconNone: "Aucun (Texte seulement)",
    optIconUp: "▲ Haut",
    optIconDown: "▼ Bas",
    optIconLeft: "◀ Gauche",
    optIconRight: "▶ Droite",
    optIconCheck: "✔ Valider / OK",
    optIconPower: "⏻ Alimentation",
    optIconVolUp: "🔊 Vol +",
    optIconVolDown: "🔉 Vol -",
    optIconMute: "🔇 Sourdine",
    optIconPlay: "▶ Lecture",
    optIconPause: "⏸ Pause",
    optIconPrev: "⏮ Piste préc.",
    optIconNext: "⏭ Piste suiv.",
    optIconHome: "🏠 Accueil",
    optIconBack: "⏎ Retour",
    optIconLock: "🔒 Verrouiller",
    optIconTerminal: "▶_ Terminal",
    optIconMonitor: "🖥 Bureau / Écran",
    optIconActivity: "📊 Gest. tâches",
    optIconVideo: "🎥 YouTube / Vidéo",
    optIconFilm: "🎬 Netflix / Film",
    optIconSearch: "🔍 Recherche",
    btnImportCustomIcon: "🖼️ Importer une image",
    customIconLoaded: "Image personnalisée chargée",
    btnSpan: "Largeur de grille (Colonnes)",
    optSpan1: "1 colonne (Standard)",
    optSpan2: "2 colonnes (Large)",
    optSpan3: "3 colonnes (Pleine largeur sur 3 col)",
    optSpan4: "4 colonnes (Pleine largeur sur 4 col)",
    colorPalette: "Palette de couleurs",
    hidKeyLabel: "Touche HID cible",
    tabBtnVisual: "Générateur visuel",
    tabBtnRaw: "DuckyScript brut",
    btnAddTypeText: "+ Taper du texte",
    btnAddCombo: "+ Combinaison de touches",
    btnAddDelay: "+ Délai (ms)",
    btnAddEnter: "+ Entrée",
    rawDuckyHelp: "Syntaxe DuckyScript supportée : STRING, DELAY, GUI, CTRL, ALT, SHIFT, ENTER, REPEAT.",
    btnTestMacro: "⚡ Tester la macro",
    btnMoveUp: "◀ Déplacer vers le haut",
    btnMoveDown: "Déplacer vers le bas ▶",
    btnDelete: "Supprimer",
    pairModalTitle: "Associer avec LilyGO T-Dongle-S3",
    pairModalDesc: "Pour gérer les dispositions et injecter des macros, cliquez sur <strong>« Demander l'association »</strong> ci-dessous, puis <strong>appuyez sur le bouton physique du dongle TV</strong> pour approuver.",
    btnRequestPairing: "Demander l'association",
    btnCancel: "Annuler",
    spacer: "Espaceur",
    page: "Page",
    rename: "Renommer",
    delete: "Supprimer",
    enterPageName: "Nom de la nouvelle page :",
    renamePagePrompt: "Renommer la page :",
    deletePageConfirm: 'Supprimer la page « {name} » ?',
    noActionsYet: "Aucune action configurée. Cliquez sur les boutons ci-dessous pour ajouter des étapes.",
    typeTextPrompt: "Texte à saisir automatiquement :",
    keyComboPrompt: "Combinaison de touches (ex. GUI r, CTRL SHIFT ESC, ALT F4) :",
    delayPrompt: "Délai en millisecondes (ex. 200) :",
    tplConfirm: 'Charger le modèle « {name} » ? Les modifications en cours seront écrasées.',
    profileImportSuccess: "Profil importé avec succès !",
    invalidProfileJson: "Structure JSON du profil non valide.",
    failParseJson: "Échec de lecture du fichier JSON : ",
    copiedToast: "JSON du profil copié dans le presse-papiers ! 📋",
    copyPrompt: "Copiez le JSON du profil ci-dessous :",
    webSerialNotSupported: "WebSerial n'est pas supporté par ce navigateur. Veuillez ouvrir cette page dans Google Chrome ou Microsoft Edge pour déployer directement via USB.",
    usbProfileSentToast: "🎉 Profil envoyé au LilyGO via USB !",
    usbProfileSentAlert: '🎉 Profil « {name} » envoyé avec succès au LilyGO par liaison série USB !',
    usbNotice: "Notification de déploiement USB : ",
    httpsMixedContentAlert: "⚠️ Avis de contenu mixte HTTPS :\n\nLes navigateurs bloquent les requêtes directes en Wi-Fi depuis un site HTTPS (comme GitHub Pages) vers des appareils HTTP locaux.\n\nPour déployer vers votre dongle :\n1. Cliquez sur « Exporter JSON » (ou « Copier JSON »).\n2. Ouvrez la page de configuration de votre dongle (http://tv-remote.local/setup).\n3. Sélectionnez le fichier dans « Upload Profile JSON » et cliquez sur Enregistrer.\n\nOu cliquez sur « ⚡ Déployer via USB » pour synchroniser directement via USB !",
    tokenNotFoundConfirm: "Jeton d'association introuvable. Vous devez d'abord associer ce studio avec le LilyGO. Ouvrir la fenêtre d'association maintenant ?",
    deploySuccessAlert: '🎉 Succès ! Le profil « {name} » a été déployé et activé sur le LilyGO !',
    deployFailedAlert: "Échec du déploiement : ",
    cannotConnectAlert: "Impossible de joindre le LilyGO à l'adresse {host}. Assurez-vous d'être connecté au même réseau Wi-Fi.",
    pairRequestSent: "Demande envoyée ! Appuyez sur le bouton du dongle LilyGO maintenant...",
    pairApproved: "Appareil approuvé et associé ! 🎉",
    pairTimedOut: "Délai d'association dépassé. Réessayez.",
    pairCannotContact: "Impossible de joindre le LilyGO à l'adresse ",
    connDotConnected: "Connecté au LilyGO",
    loadingFromDongle: "Récupération de la disposition active du LilyGO...",
    loadSuccessToast: "Profil actif chargé depuis le LilyGO ! 🎉",
    loadFailedAlert: "Impossible de charger le profil depuis le LilyGO à {host}.",
  }
};

let currentLang = 'en';
let currentMode = 'simple'; // 'simple' | 'advanced'

function t(key) {
  return (I18N[currentLang] && I18N[currentLang][key]) || (I18N['en'] && I18N['en'][key]) || key;
}

// Mode Switcher (Simple vs. Advanced)
function setMode(mode) {
  currentMode = mode === 'advanced' ? 'advanced' : 'simple';
  localStorage.setItem('lilygo_studio_mode', currentMode);

  const btnSimple = document.getElementById('btnModeSimple');
  const btnAdvanced = document.getElementById('btnModeAdvanced');

  if (currentMode === 'advanced') {
    document.body.classList.remove('mode-simple');
    document.body.classList.add('mode-advanced');
    if (btnSimple) btnSimple.classList.remove('active');
    if (btnAdvanced) btnAdvanced.classList.add('active');
  } else {
    document.body.classList.remove('mode-advanced');
    document.body.classList.add('mode-simple');
    if (btnAdvanced) btnAdvanced.classList.remove('active');
    if (btnSimple) btnSimple.classList.add('active');
  }
}

function setLanguage(lang) {
  if (!I18N[lang]) lang = 'en';
  currentLang = lang;
  localStorage.setItem('lilygo_studio_lang', lang);
  document.documentElement.lang = lang;

  const strings = I18N[lang];

  // Update text & HTML of elements with data-i18n
  document.querySelectorAll('[data-i18n]').forEach(el => {
    const key = el.getAttribute('data-i18n');
    if (strings[key] !== undefined) {
      if (strings[key].includes('<')) {
        el.innerHTML = strings[key];
      } else {
        el.textContent = strings[key];
      }
    }
  });

  // Update placeholders
  document.querySelectorAll('[data-i18n-placeholder]').forEach(el => {
    const key = el.getAttribute('data-i18n-placeholder');
    if (strings[key] !== undefined) {
      el.placeholder = strings[key];
    }
  });

  // Update optgroup labels
  document.querySelectorAll('[data-i18n-label]').forEach(el => {
    const key = el.getAttribute('data-i18n-label');
    if (strings[key] !== undefined) {
      el.label = strings[key];
    }
  });

  // Update language button label
  const btnLang = document.getElementById('btnLang');
  if (btnLang) {
    btnLang.textContent = strings.langBtn;
  }

  // Refresh dynamic parts of UI
  renderPagesList();
  renderSimGrid();
  renderInspector();
}

function toggleLanguage() {
  const nextLang = currentLang === 'en' ? 'fr' : 'en';
  setLanguage(nextLang);
}

// Current App State
let profile = JSON.parse(JSON.stringify(TEMPLATES.tv));
let currentPageIdx = 0;
let selectedBtnIdx = null;

function dismissHttpsBanner() {
  const banner = document.getElementById('httpsBanner');
  if (banner) banner.style.display = 'none';
  localStorage.setItem('lilygo_https_banner_dismissed', '1');
}

// Initialize on Load
window.addEventListener('DOMContentLoaded', () => {
  if (window.location.protocol === 'https:' && !localStorage.getItem('lilygo_https_banner_dismissed')) {
    const banner = document.getElementById('httpsBanner');
    if (banner) banner.style.display = 'flex';
  }

  // Language Auto-Detection & Initialization
  const pathIsFr = window.location.pathname.includes('/fr/') || window.location.pathname.endsWith('/fr');
  const forcedDefault = window.DEFAULT_STUDIO_LANG || (pathIsFr ? 'fr' : null);
  const savedLang = localStorage.getItem('lilygo_studio_lang');
  const browserIsFr = navigator.language && navigator.language.toLowerCase().startsWith('fr');
  const initialLang = forcedDefault || savedLang || (browserIsFr ? 'fr' : 'en');
  setLanguage(initialLang);

  // Mode Initialization (default: simple)
  const savedMode = localStorage.getItem('lilygo_studio_mode') || 'simple';
  setMode(savedMode);

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
  fetchSavedProfilesList();
});

function saveState() {
  localStorage.setItem('lilygo_studio_profile', JSON.stringify(profile));
}

// Render Complete App
function renderApp() {
  const pId = document.getElementById('profileId');
  const pName = document.getElementById('profileName');
  const pDev = document.getElementById('profileDevice');
  const sRoom = document.getElementById('simRoom');

  if (pId) pId.value = profile.id || '';
  if (pName) pName.value = profile.name || '';
  if (pDev) pDev.value = profile.deviceType || 'tv';
  if (sRoom) sRoom.innerText = profile.name || 'Remote';

  renderPagesList();
  renderSimTabs();
  renderSimGrid();
  renderInspector();
  saveState();
}

// Pages Management
function renderPagesList() {
  const list = document.getElementById('pagesList');
  if (!list) return;
  list.innerHTML = '';

  (profile.pages || []).forEach((p, idx) => {
    const item = document.createElement('div');
    item.className = 'page-item' + (idx === currentPageIdx ? ' active' : '');
    item.innerHTML = `
      <span>${escapeHtml(p.name || `${t('page')} ${idx + 1}`)}</span>
      <div style="display: flex; gap: 4px;">
        <span onclick="renamePage(${idx}, event)" title="${escapeHtml(t('rename'))}" style="cursor: pointer; opacity: 0.7;">✎</span>
        ${profile.pages.length > 1 ? `<span onclick="deletePage(${idx}, event)" title="${escapeHtml(t('delete'))}" style="cursor: pointer; opacity: 0.7; color: #ef4444;">✕</span>` : ''}
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
  const pageColsEl = document.getElementById('pageCols');
  if (pageColsEl && curPage) {
    pageColsEl.value = curPage.columns || profile.columns || 3;
  }
}

function addNewPage() {
  const name = prompt(t('enterPageName'), `${t('page')} ${profile.pages.length + 1}`);
  if (!name) return;
  profile.pages.push({
    id: 'page-' + Date.now(),
    name: name.trim(),
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
  const name = prompt(t('renamePagePrompt'), current);
  if (name && name.trim()) {
    profile.pages[idx].name = name.trim();
    renderApp();
  }
}

function deletePage(idx, e) {
  e.stopPropagation();
  if (profile.pages.length <= 1) return;
  if (!confirm(t('deletePageConfirm').replace('{name}', profile.pages[idx].name))) return;
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
  if (!tabs) return;
  tabs.innerHTML = '';
  if (profile.pages.length <= 1) {
    tabs.style.display = 'none';
    return;
  }
  tabs.style.display = 'flex';
  profile.pages.forEach((p, idx) => {
    const pill = document.createElement('div');
    pill.className = 'tab-pill' + (idx === currentPageIdx ? ' active' : '');
    pill.innerText = p.name || `${t('page')} ${idx + 1}`;
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
  if (!grid) return;
  const curPage = profile.pages[currentPageIdx] || profile.pages[0];
  const cols = curPage.columns || profile.columns || 3;
  grid.style.gridTemplateColumns = `repeat(${cols}, 1fr)`;
  grid.innerHTML = '';

  (curPage.buttons || []).forEach((b, idx) => {
    if (b.action === 'none') {
      const sp = document.createElement('div');
      sp.className = 'sim-spacer' + (idx === selectedBtnIdx ? ' selected' : '');
      if (b.span) sp.style.gridColumn = `span ${b.span}`;
      sp.innerText = t('spacer');
      sp.onclick = () => selectButton(idx);
      grid.appendChild(sp);
      return;
    }

    const el = document.createElement('div');
    el.className = 'sim-btn' + (idx === selectedBtnIdx ? ' selected' : '');
    if (b.span) el.style.gridColumn = `span ${b.span}`;
    if (b.color) el.style.background = b.color;

    let content = '';
    if (b.customIcon) {
      content += `<img src="${b.customIcon}" class="sim-btn-custom-icon" alt="" />`;
    } else if (b.icon) {
      content += `<span class="sim-btn-icon">${getIconSymbol(b.icon)}</span>`;
    }
    if (b.label) content += `<span class="sim-btn-label">${escapeHtml(b.label)}</span>`;
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
  if (!noSel || !form) return;
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

  // Custom icon preview
  const prevWrap = document.getElementById('customIconPreviewWrap');
  const prevImg = document.getElementById('customIconPreview');
  const clearBtn = document.getElementById('btnClearCustomIcon');
  if (b.customIcon) {
    if (prevWrap) prevWrap.style.display = 'flex';
    if (prevImg) prevImg.src = b.customIcon;
    if (clearBtn) clearBtn.style.display = 'inline-block';
  } else {
    if (prevWrap) prevWrap.style.display = 'none';
    if (prevImg) prevImg.src = '';
    if (clearBtn) clearBtn.style.display = 'none';
  }

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

// Custom Image Icon Uploader
function onCustomIconSelected(event) {
  const file = event.target.files[0];
  if (!file) return;

  const curPage = profile.pages[currentPageIdx];
  if (selectedBtnIdx === null || !curPage || !curPage.buttons[selectedBtnIdx]) return;
  const b = curPage.buttons[selectedBtnIdx];

  const reader = new FileReader();
  reader.onload = (e) => {
    const rawDataUrl = e.target.result;
    // Scale image down to max 64x64 for compact storage
    const img = new Image();
    img.onload = () => {
      const maxDim = 64;
      let width = img.width;
      let height = img.height;
      if (width > maxDim || height > maxDim) {
        if (width > height) {
          height = Math.round((height * maxDim) / width);
          width = maxDim;
        } else {
          width = Math.round((width * maxDim) / height);
          height = maxDim;
        }
      }
      const canvas = document.createElement('canvas');
      canvas.width = width;
      canvas.height = height;
      const ctx = canvas.getContext('2d');
      ctx.drawImage(img, 0, 0, width, height);
      b.customIcon = canvas.toDataURL('image/png');
      b.icon = ''; // Clear preset icon
      renderApp();
    };
    img.src = rawDataUrl;
  };
  reader.readAsDataURL(file);
}

function clearCustomIcon() {
  const curPage = profile.pages[currentPageIdx];
  if (selectedBtnIdx === null || !curPage || !curPage.buttons[selectedBtnIdx]) return;
  delete curPage.buttons[selectedBtnIdx].customIcon;
  renderApp();
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
  if (!container) return;
  container.innerHTML = '';
  const lines = script.split('\n').map(l => l.trim()).filter(l => l.length > 0 && !l.startsWith('REM'));

  if (lines.length === 0) {
    container.innerHTML = `<div style="color: var(--text-muted); font-size: 0.75rem; padding: 6px 0;">${escapeHtml(t('noActionsYet'))}</div>`;
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
      <button onclick="removeMacroStep(${idx})" style="padding: 2px 6px; font-size: 0.7rem; color: #ef4444; border: none; background: none; cursor: pointer;">✕</button>
    `;
    container.appendChild(item);
  });
}

function addMacroStep(type) {
  let line = '';
  if (type === 'string') {
    const text = prompt(t('typeTextPrompt'));
    if (text === null) return;
    line = 'STRING ' + text;
  } else if (type === 'combo') {
    const combo = prompt(t('keyComboPrompt'), 'GUI r');
    if (!combo) return;
    line = combo.trim().toUpperCase();
  } else if (type === 'delay') {
    const ms = prompt(t('delayPrompt'), '200');
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
  const val = sel.value;
  if (!val) return;

  if (val.startsWith('saved:')) {
    const pid = val.substring(6);
    loadSavedProfileFromDongle(pid);
    sel.value = '';
    return;
  }

  if (!TEMPLATES[val]) return;
  if (!confirm(t('tplConfirm').replace('{name}', sel.options[sel.selectedIndex].text))) return;

  profile = JSON.parse(JSON.stringify(TEMPLATES[val]));
  currentPageIdx = 0;
  selectedBtnIdx = null;
  renderApp();
  sel.value = '';
}

// Saved Profiles on LilyGO
async function fetchSavedProfilesList() {
  const host = getTargetHost();
  const grp = document.getElementById('optGroupSaved');
  if (!grp) return;

  try {
    const res = await fetch(`${host}/api/profiles`, { method: 'GET', mode: 'cors' });
    if (!res.ok) return;
    const list = await res.json();
    grp.innerHTML = '';
    if (Array.isArray(list) && list.length > 0) {
      list.forEach(p => {
        const opt = document.createElement('option');
        opt.value = `saved:${p.id}`;
        opt.textContent = `📺 ${p.name || p.id} ${p.active ? '(Active)' : ''}`;
        grp.appendChild(opt);
      });
    }
  } catch (e) {
    // Offline or HTTPS mixed content
  }
}

async function loadSavedProfileFromDongle(profileId) {
  const host = getTargetHost();
  showToast(t('loadingFromDongle'));
  try {
    const res = await fetch(`${host}/api/profiles/active`, { method: 'GET', mode: 'cors' });
    if (!res.ok) throw new Error(`HTTP error ${res.status}`);
    const data = await res.json();
    if (data && data.pages) {
      profile = data;
      currentPageIdx = 0;
      selectedBtnIdx = null;
      renderApp();
      showToast(t('loadSuccessToast'));
    }
  } catch (err) {
    alert(t('loadFailedAlert').replace('{host}', host) + '\n\n' + t('httpsMixedContentAlert'));
  }
}

// 1-Click "Load from LilyGO"
async function loadFromDongle() {
  const host = getTargetHost();
  showToast(t('loadingFromDongle'));

  try {
    const res = await fetch(`${host}/api/profiles/active`, { method: 'GET', mode: 'cors' });
    if (!res.ok) throw new Error(`HTTP error ${res.status}`);
    const data = await res.json();

    if (data && data.pages && Array.isArray(data.pages)) {
      profile = data;
      currentPageIdx = 0;
      selectedBtnIdx = null;
      renderApp();
      showToast(t('loadSuccessToast'));
      fetchSavedProfilesList();
    } else {
      alert(t('invalidProfileJson'));
    }
  } catch (err) {
    console.warn('Load from dongle failed:', err);
    alert(t('loadFailedAlert').replace('{host}', host) + '\n\n' + t('httpsMixedContentAlert'));
  }
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
        alert(t('profileImportSuccess'));
      } else {
        alert(t('invalidProfileJson'));
      }
    } catch (err) {
      alert(t('failParseJson') + err.message);
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
  if (!dot) return;
  try {
    const res = await fetch(`${host}/api/profiles`, { method: 'GET', mode: 'cors' });
    if (res.ok) {
      dot.className = 'status-dot connected';
      dot.title = t('connDotConnected');
    } else {
      dot.className = 'status-dot';
    }
  } catch (e) {
    dot.className = 'status-dot';
  }
}

// Toast notification
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
      showToast(t('copiedToast'));
    }).catch(err => {
      prompt(t('copyPrompt'), jsonStr);
    });
  } else {
    prompt(t('copyPrompt'), jsonStr);
  }
}

async function deployViaUsb() {
  if (!('serial' in navigator)) {
    alert(t('webSerialNotSupported'));
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
    
    showToast(t('usbProfileSentToast'));
    alert(t('usbProfileSentAlert').replace('{name}', profile.name));
  } catch (err) {
    if (err.name !== 'NotFoundError') {
      alert(t('usbNotice') + err.message);
    }
  }
}

async function deployToDongle() {
  const host = getTargetHost();

  if (window.location.protocol === 'https:' && host.startsWith('http://')) {
    alert(t('httpsMixedContentAlert'));
    return;
  }

  const token = localStorage.getItem('tv_remote_token') || '';

  if (!token) {
    if (confirm(t('tokenNotFoundConfirm'))) {
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
      alert(t('deploySuccessAlert').replace('{name}', profile.name));
    } else {
      alert(t('deployFailedAlert') + (data.error || 'Pairing token rejected. Re-pair device.'));
    }
  } catch (e) {
    alert(t('cannotConnectAlert').replace('{host}', host));
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
  statusEl.innerText = t('pairRequestSent');

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
          statusEl.innerText = t('pairApproved');
          checkConnection();
          setTimeout(closePairModal, 1500);
        } else if (data.status === 'expired') {
          clearInterval(pairPollTimer);
          statusEl.innerText = t('pairTimedOut');
        }
      } catch (err) {
        // continue polling
      }
    }, 1000);
  } catch (e) {
    statusEl.innerText = t('pairCannotContact') + host;
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
  if (typeof str !== 'string') return '';
  return str.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
}
