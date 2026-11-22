@echo off
cd /d "%~dp0"
g++ -std=c++11 -O2 -pthread backend\cpp\WebServer.cpp backend\cpp\Graph.cpp -o backend\cpp\sheshield_server.exe
if errorlevel 1 (
    echo Build failed. Check the compiler errors above.
    pause
    exit /b 1
)
backend\cpp\sheshield_server.exe
