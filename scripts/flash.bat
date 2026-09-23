@echo off
setlocal

if "%~1"=="" (
    echo Usage: flash.bat ^<platform^> [toolchain]
    echo.
    echo Examples:
    echo   flash.bat at32f403a armclang
    echo   flash.bat at32f403a gcc
    exit /b 1
)

set "PLATFORM=%~1"
set "TOOLCHAIN=%~2"

if "%TOOLCHAIN%"=="" set "TOOLCHAIN=armclang"

set "ROOT=%~dp0.."
set "BUILD_DIR=%ROOT%\.build\%PLATFORM%-%TOOLCHAIN%"

echo Building and flashing %PLATFORM% (%TOOLCHAIN%)...

if not exist "%BUILD_DIR%" (
    echo Configuring...
    cmake --preset "%PLATFORM%-%TOOLCHAIN%"
)

cmake --build "%BUILD_DIR%" --target flash
