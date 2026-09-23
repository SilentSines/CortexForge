#!/bin/bash
set -e

ROOT="$(cd "$(dirname "$0")/.." && pwd)"

echo "Cleaning build directories..."
rm -rf "$ROOT/.build" && echo "  Removed .build/" || echo "  .build/ not found"

echo "Cleaning VS Code generated configs..."
rm -f "$ROOT/.vscode/launch.json" && echo "  Removed launch.json" || echo "  launch.json not found"
rm -f "$ROOT/.vscode/tasks.json" && echo "  Removed tasks.json" || echo "  tasks.json not found"
rm -f "$ROOT/.vscode/settings.json" && echo "  Removed settings.json" || echo "  settings.json not found"

echo "Done. Run 'cmake --preset at32f403a-gcc' to regenerate."
