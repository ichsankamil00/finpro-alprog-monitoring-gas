@echo off
cd /d "%~dp0"

if not exist monitoring.exe (
    call build.bat
)

start "SERVER MONITORING" cmd /k "cd /d %~dp0 && monitoring.exe"
timeout /t 2 >nul
start "CLIENT SENSOR" cmd /k "cd /d %~dp0 && monitoring.exe"