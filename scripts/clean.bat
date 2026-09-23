@echo off
setlocal

set "ROOT=%~dp0.."

echo Cleaning build directories...
if exist "%ROOT%\.build" (
    rmdir /s /q "%ROOT%\.build"
    echo   Removed .build/
) else (
    echo   .build/ not found
)

echo Cleaning VS Code generated configs...
del /q "%ROOT%\.vscode\launch.json" 2>nul && echo   Removed launch.json || echo   launch.json not found
del /q "%ROOT%\.vscode\tasks.json" 2>nul && echo   Removed tasks.json || echo   tasks.json not found
del /q "%ROOT%\.vscode\settings.json" 2>nul && echo   Removed settings.json || echo   settings.json not found
del /q "%ROOT%\.vscode\c_cpp_properties.json" 2>nul && echo   Removed c_cpp_properties.json || echo   c_cpp_properties.json not found

echo Done. Run "cmake --preset at32f403a-armclang" or "cmake --preset at32f403a-gcc" to regenerate.
