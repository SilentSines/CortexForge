@echo off
if "%1"=="" (
    echo Usage: build-armclang.bat ^<chip^> [e.g. at32f403a]
    exit /b 1
)

set CHIP=%1
set PRESET=%CHIP%-armclang
set BUILD_DIR=.build\%PRESET%

echo === CortexForge Build (Windows / ARMCLANG) ===
echo Chip:       %CHIP%
echo Preset:     %PRESET%
echo Build dir:  %BUILD_DIR%
echo.

if exist %BUILD_DIR% (
    echo Build directory exists, rebuilding...
) else (
    echo Configuring...
    cmake --preset %PRESET%
    if errorlevel 1 (
        echo Configure failed!
        exit /b 1
    )
)

echo Building...
cmake --build %BUILD_DIR%
if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo.
echo Build OK
echo Output: %BUILD_DIR%\output\
