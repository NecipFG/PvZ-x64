# PvZ-x64 — AGENTS.md

## Project
Reconstructed 64-bit port of Plants vs. Zombies. C++17, CMake.

## Build

| Target | Command |
|--------|---------|
| Linux (release) | `bash build_linux.sh` |
| Windows (cross from Linux) | `bash build.sh` or `bash build.sh Debug` |
| Manual cmake (Linux) | `cmake -B build-linux -DCMAKE_BUILD_TYPE=Release && cmake --build build-linux -j` |
| Manual cmake (Windows cross) | `cmake -B build -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake -DCMAKE_BUILD_TYPE=Release && cmake --build build -j` |

Build scripts auto-copy `libbass.so` / `bass.dll` + SDL2.dll + MinGW runtime DLLs + assets from `Debug/` dir.

## Run
- **Linux:** `env LD_LIBRARY_PATH="." ./PvZ-Linux` (optional: `PVZ_FPS=144 PVZ_VSYNC=1`)
- **Wine:** `bash run.sh` (forces Mesa drivers via `__GLX_VENDOR_LIBRARY_NAME=mesa`)

## Architecture
- `main.cpp` → `LawnApp` (top-level app) → `Board` (game board) + subsystems
- `Lawn/` — game logic (Plant, Zombie, Projectile, Board, Challenge, ZenGarden, etc.)
- `SexyAppFramework/` — PopCap framework; SDL2 on Linux, Direct3D on Windows
- `Sexy.TodLib/` — animation/particles/audio library (Reanimator, TodParticle, EffectSystem, etc.)
- `compat/` — cross-platform shims; **must be first in include path** (CMake does this)
- `compat/pvz_compat.h` is force-included via `-include` in CMake (Windows MinGW only)
- `ImageLib/` — zlib, libpng, libjpeg, JasPer JPEG2000 (all compiled from source)
- `PakLib/` — `.pak` file reader
- `ConstEnums.h` (1322 lines) — game enums; read in sections
- `Resources.h` (1345 lines) — resource ID definitions

## Porting gotchas
- **`ulong` = `unsigned int`** on Linux (LP64: `unsigned long` is 8 bytes, breaks ARGB pixel arrays). Already handled via `compat`.
- **Color UB:** Signed left-shift of 32-bit color components (e.g. `mAlpha << 24`) must cast to `uint32_t` first.
- **SWTri padding:** `SWHelper::XYZStruct::mDiffuse` changed from `long` to `uint32_t` for consistent struct layout across LP64/LLP64.
- **Frame rate:** Logic ticks at fixed 125 Hz. Render FPS defaults to 120 (`PVZ_FPS` env var). VSync off by default on Linux.
- **No game assets in repo.** Need original `properties/`, `images/`, `sounds/`, etc. from a legitimate PvZ copy.

## Testing
No test framework exists. Verification is manual: build + run.

## Key files
- `CMakeLists.txt` — single build file, conditionally targets Windows (`PvZ-x64.exe`) or Linux (`PvZ-Linux`)
- `mingw-toolchain.cmake` — cross-compile toolchain file
- `LawnApp.h` — central class with all game state pointers, documented member offsets
