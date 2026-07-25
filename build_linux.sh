#!/usr/bin/env bash
# build_linux.sh — Build PvZ-x64 natively for Linux
set -e

ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT/build-linux"

BUILD_TYPE="${1:-Release}"
echo ">>> Configuring Native Linux Build (${BUILD_TYPE}) ..."
mkdir -p "$BUILD_DIR"
cmake -S "$ROOT" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

echo ">>> Building ..."
cmake --build "$BUILD_DIR" -- -j"$(nproc)"

# Copy libbass.so next to the binary if it exists
if [ -f "$ROOT/libbass.so" ]; then
    cp -u "$ROOT/libbass.so" "$BUILD_DIR/"
    echo "  Copied: libbass.so to build-linux/"
fi

# Copy assets from Debug folder if they exist
if [ -d "$ROOT/Debug" ]; then
    echo ">>> Copying assets from Debug folder ..."
    for dir in properties images sounds reanim particles data compiled; do
        if [ -d "$ROOT/Debug/$dir" ]; then
            cp -ru "$ROOT/Debug/$dir" "$BUILD_DIR/"
        fi
    done
fi


echo ""
echo ">>> Native Linux ${BUILD_TYPE} Build successful: $BUILD_DIR/PvZ-Linux"
if [ "$BUILD_TYPE" = "Debug" ]; then
    echo "  This build has cheats ENABLED."
else
    echo "  This build has cheats DISABLED and DRM enabled."
    echo "  Run 'bash build_linux.sh Debug' for a debug build with cheats."
fi
echo "To run:"
echo "  cd '$BUILD_DIR' && env LD_LIBRARY_PATH=\".\" ./PvZ-Linux"
echo ""
