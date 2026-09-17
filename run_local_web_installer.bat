@echo off
echo ============================================================
echo   LilyGO T-Dongle-S3 - Local Web Firmware Installer
echo ============================================================
echo.
echo Starting local HTTP server for testing Web Serial...
echo Opening http://localhost:8000/install.html in your default browser...
echo.

start http://localhost:8000/install.html

python -m http.server 8000 --directory docs 2>nul
if %errorlevel% neq 0 (
    echo Python not found in system PATH, trying PlatformIO Python...
    "%USERPROFILE%\.platformio\penv\Scripts\python.exe" -m http.server 8000 --directory docs
)

pause
