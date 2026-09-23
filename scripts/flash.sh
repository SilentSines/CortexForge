#!/bin/bash
set -e

if [ -z "$1" ]; then
    echo "Usage: flash.sh <platform> [toolchain]"
    echo ""
    echo "Examples:"
    echo "  flash.sh at32f403a gcc"
    exit 1
fi

PLATFORM="$1"
TOOLCHAIN="${2:-gcc}"
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT/.build/$PLATFORM-$TOOLCHAIN"

echo "Building and flashing $PLATFORM ($TOOLCHAIN)..."

if [ ! -d "$BUILD_DIR" ]; then
    echo "Configuring..."
    cmake --preset "$PLATFORM-$TOOLCHAIN"
fi

cmake --build "$BUILD_DIR" --target flash
