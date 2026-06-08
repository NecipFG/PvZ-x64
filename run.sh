#!/usr/bin/env bash
# run.sh — Run PvZ-x64 under Wine

ROOT="$(cd "$(dirname "$0")" && pwd)"
EXE="$ROOT/build/PvZ-x64.exe"

if [ ! -f "$EXE" ]; then
    echo "ERROR: Game executable not found at $EXE"
    echo "Please build the game first using: bash build.sh"
    exit 1
fi

# Force Mesa (Intel/AMD) drivers to avoid broken 64-bit Nvidia driver fallback under Wine
export __GLX_VENDOR_LIBRARY_NAME=mesa
export __EGL_VENDOR_LIBRARY_FILENAMES=/usr/share/glvnd/egl_vendor.d/50_mesa.json
export DRI_PRIME=0

cd "$ROOT/build"
echo ">>> Launching PvZ-x64..."
wine PvZ-x64.exe "$@"
