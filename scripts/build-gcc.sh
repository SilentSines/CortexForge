#!/bin/bash
set -e

CHIP="${1:?Usage: $0 <chip> [e.g. at32f403a]}"

PRESET="$CHIP-gcc"
BUILD_DIR=".build/$PRESET"

echo -e "\033[36m=== CortexForge Build (Linux / GCC) ===\033[0m"
echo "Chip:       $CHIP"
echo "Preset:     $PRESET"
echo "Build dir:  $BUILD_DIR"
echo ""

if [ -d "$BUILD_DIR" ]; then
    echo -e "\033[33mBuild directory exists, rebuilding...\033[0m"
else
    echo -e "\033[33mConfiguring...\033[0m"
    cmake --preset "$PRESET"
fi

echo -e "\033[33mBuilding...\033[0m"
cmake --build "$BUILD_DIR"

echo ""
echo -e "\033[32mBuild OK\033[0m"
echo "Output: $BUILD_DIR/output/"
