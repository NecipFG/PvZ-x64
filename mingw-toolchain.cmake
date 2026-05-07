# MinGW-w64 Cross-Compilation Toolchain for 64-bit Windows
# Usage: cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Cross-compiler prefix — adjust if your distro uses a different prefix
set(CROSS_PREFIX "x86_64-w64-mingw32")

find_program(CMAKE_C_COMPILER   NAMES "${CROSS_PREFIX}-gcc"   REQUIRED)
find_program(CMAKE_CXX_COMPILER NAMES "${CROSS_PREFIX}-g++"   REQUIRED)
find_program(CMAKE_RC_COMPILER  NAMES "${CROSS_PREFIX}-windres")
find_program(DLLTOOL            NAMES "${CROSS_PREFIX}-dlltool")

set(CMAKE_FIND_ROOT_PATH "/usr/${CROSS_PREFIX}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
