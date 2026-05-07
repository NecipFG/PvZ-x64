# Plants vs. Zombies – Decompilation Codebase

## Project Overview

This is a decompiled/reconstructed source for **Plants vs. Zombies (PC)**, based on PopCap's original ~2009 Windows build using their proprietary **SexyAppBase v1.33 framework**.

- **Language:** C++ (C++14 compatible), with C files for image codecs and compression
- **Target:** Win32 (x86 32-bit) Windows executable
- **Original build system:** Visual Studio 2019/2022 (`SexyAppFramework/SexyAppBase.vcxproj`)
- **Cross-compile build system:** CMake + MinGW-w64 (`CMakeLists.txt`)

---

## Directory Structure

```
Updated Base/
├── main.cpp                  # Entry point (WinMain)
├── LawnApp.cpp / LawnApp.h   # Main application class
├── Resources.cpp / .h        # Resource declarations
├── GameConstants.h           # Game constants
├── ConstEnums.h              # Enumerations
├── Lawn/                     # Game logic
│   ├── Board.cpp             # Main game board
│   ├── Plant.cpp             # Plant logic
│   ├── Zombie.cpp            # Zombie logic
│   ├── Projectile.cpp        # Projectile logic
│   ├── System/               # Subsystems (Music, SaveGame, ProfileMgr, etc.)
│   └── Widget/               # UI screens (TitleScreen, GameSelector, etc.)
├── SexyAppFramework/         # PopCap game framework (DirectX, audio, UI)
│   ├── SexyAppBase.vcxproj   # Original VS project file (all sources defined here)
│   ├── SexyApp.cpp/h         # Framework core
│   ├── D3D8Helper.cpp        # Direct3D 8 rendering
│   ├── DDInterface.cpp       # DirectDraw fallback
│   ├── BassLoader.cpp        # BASS audio loader
│   └── ogg/                  # Ogg Vorbis decoder (Tremor)
├── Sexy.TodLib/              # TOD animation/particle/foley library
├── ImageLib/                 # Image codecs (PNG, JPEG, JPEG2000, zlib)
│   ├── jpeg/                 # libjpeg
│   ├── jpeg2000/             # JasPer JPEG2000
│   ├── png/                  # libpng
│   └── zlib/                 # zlib
├── PakLib/                   # .pak resource file loader
├── dx8sdk/                   # DirectX 8 SDK headers and MSVC import libs
│   ├── include/              # d3d8.h, ddraw.h, dsound.h, dinput8.h, etc.
│   └── lib/                  # MSVC .lib files (not used by MinGW)
├── Debug/src/                # Standalone compression/encryption utilities
│                             # (NOT part of main game build)
├── PopcapDocs/               # SDK documentation (.chm, .pdf)
├── bass.dll                  # BASS audio DLL (must ship with exe)
├── PlantsVsZombies.sln       # Visual Studio solution
└── CMakeLists.txt            # Cross-compilation build (MinGW-w64)
```

---

## Building on Linux (Cross-Compile → Windows .exe)

### Prerequisites

```bash
# Arch Linux
sudo pacman -S mingw-w64-gcc cmake make

# Ubuntu/Debian
sudo apt install gcc-mingw-w64-i686 g++-mingw-w64-i686 cmake make

# Fedora
sudo dnf install mingw32-gcc mingw32-gcc-c++ cmake make
```

Verify you have: `i686-w64-mingw32-g++`

### Build Steps

```bash
cd "/home/nfg/İndirilenler/Updated Base"
mkdir -p build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

Or use the convenience script:
```bash
bash build.sh
```

Output: `build/PlantsVsZombies.exe`

### Running the exe

The exe requires these DLLs alongside it:
- `bass.dll` (BASS audio — included in project root)
- `d3d8.dll` (DirectX 8 — from Windows system or DXVK)
- Standard MinGW runtime DLLs (bundled by `build.sh`)

Run under Wine or copy to Windows:
```bash
wine build/PlantsVsZombies.exe
```

---

## Key Architectural Notes

### Graphics
- **Primary renderer:** Direct3D 8 (`D3D8Helper.cpp`, `D3DInterface.cpp`)
- **Fallback renderer:** DirectDraw 7 (`DDInterface.cpp`, `D3DTester.cpp`)
- Conflict: `d3d.h` and `d3d8.h` define overlapping types — they are compiled in separate translation units

### Audio
- **Engine:** BASS audio library (`bass.dll`, `BassLoader.cpp`)
- **Formats:** Ogg Vorbis (Tremor decoder in `SexyAppFramework/ogg/`)
- FMod loader also present but not active

### Resources
- Game assets loaded from `.pak` files via `PakLib/PakInterface.cpp`
- Resource XML at `properties/resources.xml`
- Path resolution: checks current dir, then `../`

### String System
- `_USE_WIDE_STRING` / `WIN_32` defines control wide vs. multibyte strings
- `SexyString` is `std::string` or `std::wstring` depending on build config
- Default for this codebase: **multibyte** (`CharacterSet = MultiByte` in vcxproj)

### Debug/src/ Directory
These are **standalone compression/encryption research utilities** (QuickBMS tools). They are **not compiled into the game**. The `Debug/src/compression/jcalg1_d.lib` is a prebuilt static lib.

---

## Preprocessor Defines

| Define | Purpose |
|--------|---------|
| `WIN32` | Windows platform |
| `_WINDOWS` | Windows subsystem (WinMain) |
| `_LIB` | Library mode for SexyAppBase |
| `_VISUALC_` | MSVC-specific code paths |
| `_JPEGLIB_` | Enable libjpeg in ImageLib |
| `_CRT_SECURE_NO_WARNINGS` | Suppress MSVC deprecation warnings (debug only) |
| `_DEBUG` / `NDEBUG` | Debug/Release mode |

---

## Known Compilation Challenges

1. **SEH (Structured Exception Handling):** `SEHCatcher.cpp` uses MSVC-specific SEH (`__try`/`__except`). MinGW supports SEH on 32-bit via Dwarf2 exceptions — this file may need `#ifdef _MSC_VER` guards.

2. **`freopen_s`:** Used in `main.cpp` debug block — MinGW provides this, no issue.

3. **MSVC-only intrinsics:** Some files may use `__declspec`, `__forceinline`, `__assume` — mostly harmless with MinGW.

4. **dx8sdk .lib files:** MSVC format — not usable by MinGW. CMake uses MinGW's built-in DirectX import libs instead.

5. **BASS import lib:** `bass.dll` ships without a MinGW `.dll.a`. The build script auto-generates it via `dlltool`.

6. **`j2k-codec.lib`:** Prebuilt MSVC static lib for JPEG2000. The CMake build uses the open-source JasPer C sources from `ImageLib/jpeg2000/` instead.

## Knowledge Graph (RAG)

A graphify knowledge graph lives at `graphify-out/graph.json` (6,340 nodes, 12,958 edges, 98 communities). Use it before reading raw files — it is faster and more complete than grepping.

### When to query

| Situation | Command |
|-----------|---------|
| "Where is X implemented / what calls X?" | `/graphify query "X"` |
| "How does A connect to B?" | `/graphify path "A" "B"` |
| "Explain what this component does" | `/graphify explain "ComponentName"` |
| Before modifying a file | Query its key functions first to find hidden callers |
| After modifying code files | Run the rebuild command below to keep graph current |

### Key god nodes (highest connectivity — touch carefully)

| Node | Edges | Where |
|------|-------|-------|
| `PlayZombieReanim()` | 42 | `Zombie.cpp` |
| `ControlSite` | 38 | `SexyAppFramework/` (Flash/ActiveX bridge) |
| `UpdateAbilities()` | 32 | `Lawn/` |
| `ExtractResourcesByName()` | 30 | `SexyAppFramework/ResourceManager.cpp` |
| `DrawGameObjects()` | 30 | `Board.cpp` |
| `UpdateActions()` | 28 | `Lawn/` |

### Community map (98 total — major ones)

| Community | Size | What it is |
|-----------|------|------------|
| Game Board Logic | 269 | `Board.cpp` — main game loop, rendering, coins, graves |
| SexyApp UI Framework | 413 | Buttons, dialogs, widgets, popup system |
| Zombie AI & Animation | 228 | `Zombie.cpp` — all zombie behavior |
| Plant AI & Animation | 106 | `Plant.cpp` — all plant behavior |
| Challenge Game Modes | 159 | Mini-games, Bejeweled mode, boss logic |
| App Framework Core | 199 | `SexyAppBase` — window, input, app lifecycle |
| Main App & Reanimation | 185 | `LawnApp.cpp` — top-level game init |
| Direct3D 8 Renderer | 64 | `D3DInterface.cpp` — primary renderer |
| BASS Audio Engine | 96 | `BassLoader.cpp` + `BassMusicInterface.cpp` |
| Resource Manager | 52 | `ResourceManager.cpp` — asset loading |
| Particle Attachment System | 183 | `Attachment.cpp` — particle/reanim effects |
| Zen Garden Mode | 84 | `ZenGarden.cpp` — zen garden logic |

### How to query

```bash
# From project root — use the graphify venv
PYTHON=/home/nfg/.local/share/pipx/venvs/graphifyy/bin/python

# Broad: what is X connected to?
$PYTHON -m graphify query "Board" graphify-out/graph.json

# Trace: how does A reach B?
$PYTHON -m graphify path "Plant" "Board" graphify-out/graph.json

# Deep-dive a node
$PYTHON -m graphify explain "DrawGameObjects" graphify-out/graph.json
```

Or use the `/graphify query "..."` slash command directly in Claude Code.

### Keep graph current after code changes

```bash
/home/nfg/.local/share/pipx/venvs/graphifyy/bin/python -c "
from graphify.watch import _rebuild_code
from pathlib import Path
_rebuild_code(Path('.'))
"
```

### Known dead / inert nodes

- `ImageLib/j2k-codec/` — superseded by JasPer; nothing in CMake build uses it
- `CMake BASS Import Library` — build artifact, auto-generated by `dlltool`
- `CMake pvz_compat.h Force-Include Shim` — injected via `-include` flag, no source imports it directly
