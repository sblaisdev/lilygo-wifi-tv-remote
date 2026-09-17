# 📺 Télécommande TV Wi-Fi LiLyGO

<p align="center">
  <a href="README.md">🇬🇧 English</a> | <b>🇫🇷 Français</b>
</p>

Transformez votre téléphone intelligent en télécommande ultra-réactive et en clavier sans fil pour votre téléviseur connecté, boîtier de diffusion (Apple TV, Google TV, Fire TV, Roku) ou PC — propulsé par la clé USB miniature **LilyGO T-Dongle-S3**.

<p align="center">
  <img src="docs/remote_screenshot.png" alt="Interface de la télécommande TV intelligente" width="320">
</p>

---

## 🎨 Studio Compagnon de Conception (Designer)

Personnalisez la disposition de votre télécommande, réorganisez les boutons ou programmez des raccourcis et macros automatisés directement dans votre navigateur web :  
👉 **[Lancer le Studio en ligne (Designer)](https://sblaisdev.github.io/lilygo-wifi-tv-remote/fr/)**

---

## ✨ Fonctionnalités pour les utilisateurs au quotidien

- **📱 Contrôlez votre téléviseur depuis n'importe quel téléphone** : Ouvrez simplement une adresse web sur votre iPhone, Android, tablette ou ordinateur portable pour piloter votre TV. Aucune application à télécharger sur l'App Store, aucun compte et aucun jumelage Bluetooth compliqué.
- **⌨️ Clavier en direct fluide et rapide** : Fini la corvée de naviguer lettre par lettre sur le clavier virtuel de la télé avec les flèches ! Utilisez le vrai clavier de votre téléphone pour saisir instantanément vos recherches YouTube, mots de passe et adresses web sur votre téléviseur.
- **🧹 Bouton « Effacer » intelligent (Clear TV)** : Une touche dédiée efface en un instant le champ de recherche de votre téléviseur et la boîte de saisie de votre téléphone en même temps.
- **🔌 100 % Prêt à l'emploi (Zéro pilote)** : Branchez la clé dans n'importe quel port USB de votre téléviseur, projecteur, décodeur ou PC. Elle est immédiatement reconnue comme un clavier et un contrôleur multimédia USB standard.
- **🎨 Personnalisation visuelle facile** : Utilisez le [Studio Compagnon](https://sblaisdev.github.io/lilygo-wifi-tv-remote/fr/) pour créer vos propres télécommandes, ajouter des touches de raccourci (Netflix, YouTube) ou basculer entre mode TV et mode Ordinateur.
- **📲 S'installe comme une application mobile (PWA)** : Ajoutez la télécommande à l'écran d'accueil de votre téléphone en un clic. Elle s'ouvre en plein écran comme une vraie application avec retours haptiques (vibrations).
- **🌐 Fonctionne à la maison comme en déplacement** : Connectez-la au Wi-Fi de votre salon, ou laissez-la créer son propre réseau Wi-Fi autonome pour piloter un téléviseur à l'hôtel ou en voyage.
- **🔄 Mises à jour sans fil en 1 clic (OTA)** : Maintenez votre clé à jour d'un simple toucher depuis votre navigateur en Wi-Fi, sans câble ni outils de programmation.

---

## 📖 Guides d'utilisation

### ⚡ Première installation du firmware (Nouvelle clé)
- 🌐 **[Installateur Web de Firmware (1 clic)](https://sblaisdev.github.io/lilygo-wifi-tv-remote/install.html)** : Flashez votre clé LilyGO T-Dongle-S3 neuve ou réutilisée directement depuis Chrome ou Edge. Aucun logiciel, aucun pilote, aucun terminal !

### 🚀 Démarrage Rapide (Termes Simples)
- 🇫🇷 **[Guide de Démarrage Rapide](DEMARRAGE_RAPIDE.md)** : Configuration pas à pas et accès à la télécommande expliqués simplement.
- 🇬🇧 **[Getting Started Guide (English)](GETTING_STARTED.md)** : Step-by-step setup in plain language.

### 📚 Documentation Complète & Technique
- 🇫🇷 **[Guide Complet de l'utilisateur](USER_GUIDE_FR.md)** : Architecture matérielle, installation PWA, sécurité et cryptographie, réinitialisation, dépannage.
- 🇬🇧 **[Full User Guide (English)](USER_GUIDE_EN.md)** : Hardware architecture, security & crypto, factory reset, troubleshooting.

---

## 🛠️ Spécifications techniques et architecture de sécurité

1. **Barrière physique de proximité et jumelage sécurisé** : Le point d'accès initial (`TV-Remote-Setup`) génère un mot de passe aléatoire WPA2 de 8 caractères via le générateur matériel TRNG de l'ESP32-S3, affiché **exclusivement sur l'écran LCD 160×80** de la clé. Le **jumelage matériel (Méthode B)** requiert un appui sur le bouton physique de la clé avant qu'un appareil puisse transmettre des touches.
2. **Stockage d'identifiants chiffré (Zéro texte clair)** : Les identifiants Wi-Fi sont chiffrés en **AES-256 CTR** via le périphérique matériel **Hardware HMAC** de l'ESP32-S3 (`KEY0`–`KEY5`) avec réutilisation automatique du bloc de clé et validation stricte (aucun repli en texte clair).
3. **Chiffrement des frappes en transit** : Les frappes transmises via WebSocket/HTTP sont chiffrées côté client avec **WebCrypto AES-CTR** natif au navigateur avant diffusion sur le réseau local.
4. **Télécommande web à latence sub-milliseconde** : Communication temps réel par WebSockets avec croix directionnelle (D-Pad), commandes multimédias, volume, alimentation et flux de frappe en direct.
5. **Mode Station Wi-Fi ou Point d'accès autonome** : Connexion automatique au Wi-Fi domestique ou point d'accès privé dédié sans routeur externe.
6. **Application Web Progressive (PWA)** : Icône néon personnalisée, manifest web et fonctionnement plein écran sur iOS et Android.
7. **Réinitialisation d'usine sécurisée** : Maintien continu du bouton pendant 10 secondes avec confirmation en deux étapes à l'écran, effaçant la mémoire NVS tout en préservant les eFuses matériels.
8. **Studio universel et macros DuckyScript** : Application compagnon hébergée sur [GitHub Pages](https://sblaisdev.github.io/lilygo-wifi-tv-remote/fr/) (ou en local dans `docs/`) pour concevoir des grilles de boutons et déployer des macros via Wi-Fi, USB ou export JSON. Profils stockés dans LittleFS avec détection automatique de carte MicroSD.
9. **Mises à jour Cloud OTA et canal de testeurs** : Téléchargement et flashage direct depuis les versions GitHub Releases. Sélecteur de version avec possibilité de retour arrière (rollback), binaires générés par GitHub Actions, téléversement manuel hors-ligne et canal dédié **Test Device Mode** pour évaluer les versions préliminaires.
