#!/usr/bin/env bash
# build_linux.sh — Build PvZ-x64 natively for Linux
set -e

ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT/build-linux"

echo ">>> Configuring Native Linux Build (Release) ..."
mkdir -p "$BUILD_DIR"
cmake -S "$ROOT" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release

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
echo ">>> Native Linux Release Build successful: $BUILD_DIR/PvZ-Linux"
echo "  This build has cheats DISABLED and DRM enabled."
echo "  Use build_linux_debug.sh for a debug build with cheats."
echo "To run:"
echo "  cd '$BUILD_DIR' && env LD_LIBRARY_PATH=\".\" ./PvZ-Linux"
echo ""

