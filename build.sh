#!/usr/bin/env bash
# build.sh — Cross-compile PvZ to Windows .exe using MinGW-w64 on Linux
set -e

ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT/build"
CROSS="x86_64-w64-mingw32"

# ---------------------------------------------------------------------------
# 1. Check prerequisites
# ---------------------------------------------------------------------------
for tool in "${CROSS}-g++" "${CROSS}-gcc" "${CROSS}-dlltool" "${CROSS}-windres" cmake make; do
    if ! command -v "$tool" &>/dev/null; then
        echo "ERROR: '$tool' not found."
        echo ""
        echo "Install with:"
        echo "  Arch:   sudo pacman -S mingw-w64-gcc cmake make"
        echo "  Ubuntu: sudo apt install gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 cmake make"
        echo "  Fedora: sudo dnf install mingw64-gcc mingw64-gcc-c++ cmake make"
        exit 1
    fi
done

# ---------------------------------------------------------------------------
# 2. Generate BASS import library for MinGW
#    bass.dll ships with an MSVC .lib — MinGW needs a .dll.a instead.
# ---------------------------------------------------------------------------
BASS_DLL="$ROOT/bass.dll"
BASS_DEF="$ROOT/bass.def"
BASS_IMPLIB="$ROOT/libbass.dll.a"

if [ ! -f "$BASS_IMPLIB" ]; then
    echo ">>> Generating MinGW import library for bass.dll ..."

    if [ ! -f "$BASS_DEF" ]; then
        echo ">>> Extracting exports from bass.dll ..."
        {
            echo "LIBRARY BASS"
            echo "EXPORTS"
            # Extract only the actual exported symbol names from the DLL export table.
            # The [Ordinal/Name Pointer] Table section lists lines like:
            #   [   0] +base[   1]  0000 BASS_Apply3D
            # We parse only those lines (4-field lines starting after the table header).
            "${CROSS}-objdump" -p "$BASS_DLL" \
                | awk '/\[Ordinal\/Name Pointer\] Table/,0 { if (/\] \+base\[/) print $NF }'
        } > "$BASS_DEF"
    fi

    "${CROSS}-dlltool" \
        --input-def "$BASS_DEF" \
        --dllname bass.dll \
        --output-lib "$BASS_IMPLIB" \
        --kill-at

    echo ">>> libbass.dll.a generated."
fi

# ---------------------------------------------------------------------------
# 3. Configure with CMake
# ---------------------------------------------------------------------------
BUILD_TYPE="${1:-Release}"   # pass "Debug" as first arg for debug build
echo ">>> Configuring (${BUILD_TYPE}) ..."

mkdir -p "$BUILD_DIR"
cmake -S "$ROOT" -B "$BUILD_DIR" \
    -DCMAKE_TOOLCHAIN_FILE="$ROOT/mingw-toolchain.cmake" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_VERBOSE_MAKEFILE=OFF

# ---------------------------------------------------------------------------
# 4. Build
# ---------------------------------------------------------------------------
echo ">>> Building ..."
cmake --build "$BUILD_DIR" -- -j"$(nproc)"

# ---------------------------------------------------------------------------
# 5. Copy required DLLs next to the exe
# ---------------------------------------------------------------------------
EXE="$BUILD_DIR/PlantsVsZombies.exe"
if [ -f "$EXE" ]; then
    echo ""
    echo ">>> Build successful: $EXE"
    echo ""

    # Copy MinGW runtime DLLs so the exe runs without MinGW installed
    MINGW_SYSROOT="/usr/${CROSS}"
    for dll in libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll; do
        src=$(find "$MINGW_SYSROOT" -name "$dll" 2>/dev/null | head -1)
        if [ -n "$src" ]; then
            cp -n "$src" "$BUILD_DIR/"
            echo "  Copied: $dll"
        fi
    done

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
    echo "To run under Wine:"
    echo "  wine '$EXE'"
    echo ""
    echo "NOTE: The game expects 'properties/resources.xml' and .pak files"
    echo "in the same directory as the exe (or one level up)."
fi
