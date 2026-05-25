@echo off
cd /d "%~dp0"

echo Compiling project...
g++ -std=c++11 main.cpp client.cpp server.cpp -o monitoring.exe -lws2_32

if %errorlevel% neq 0 (
    echo.
    echo Compile failed.
    pause
    exit /b
)

echo.
echo Compile success. monitoring.exe created.
pause