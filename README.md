# Plants vs. Zombies - Reconstructed Source

This repository contains the reconstructed source code for Plants vs. Zombies (PC), based on the original SexyAppFramework.

## Project Information

- **Platform:** Windows (Win32)
- **Framework:** SexyAppFramework v1.33
- **Language:** C++14

## Documentation

- [CLAUDE.md](CLAUDE.md) - Project overview and instructions
- [GEMINI.md](GEMINI.md) - Decompilation details and build instructions
- [QWEN.md](QWEN.md) - Additional notes

## Building

Please refer to [GEMINI.md](GEMINI.md) for detailed build instructions using CMake and MinGW-w64.

### Build Quick Start (Linux Cross-Compile)

```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
make
```

## Credits

Original game by PopCap Games. Reconstructed source by the community.
