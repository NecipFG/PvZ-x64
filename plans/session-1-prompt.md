# Session 1: Toolchain & Build Scaffolding

**Objective:** Set up the 64-bit (x86_64) toolchain and build scaffolding for Plants vs. Zombies. The goal of this session is to modify the build system and dependencies so that the compiler attempts a 64-bit build and generates a list of compilation errors for us to fix.

**Tasks:**
1. **Update CMakeLists.txt:** 
   - Remove the `WIN32` sanity check at the top of the file.
   - Remove the `-m32` flags from `target_compile_options` and `target_link_options`.
   - Ensure the build is configured to use the `x86_64-w64-mingw32-g++` cross-compiler instead of `i686`.
2. **Update BASS Audio:**
   - The current `bass.dll` is 32-bit. We need to obtain or specify a 64-bit `bass.dll`.
   - Update `build.sh` or manual instructions to generate a 64-bit `libbass.dll.a` using `x86_64-w64-mingw32-dlltool`.
3. **DirectX 8 Wrapper Strategy:**
   - Since native 64-bit d3d8.dll is not viable, document or implement the inclusion of a 64-bit `d3d8to9` wrapper, or adjust linkage to allow the build to proceed.
4. **Initial Compilation:**
   - Run the CMake configuration and trigger a build. 
   - Capture the initial wave of 64-bit compiler errors (pointer casts, assembly issues, etc.).

**Completion Condition:**
When the build system is successfully configured for x86_64 and we have a concrete list of compilation errors to tackle, print exactly:
`Session Is Done`