# Session 2: The Great Pointer Purge

**Objective:** Fix all compilation errors related to 32-bit assumptions, specifically pointer-to-integer casts and inline assembly, to achieve a successful 64-bit compile.

**Tasks:**
1. **Fix Pointer Arithmetic:** 
   - In `Sexy.TodLib/Definition.cpp` (and anywhere else), fix instances where pointers are cast to `int` for arithmetic (e.g., `(void*)((int)theDefinition + aField->mFieldOffset)`). Change these to use `(intptr_t)` or `(char*)`.
2. **Fix Pointer Indexing:**
   - In `Sexy.TodLib/ReanimAtlas.cpp`, fix hackish logic where `Image*` pointers are compared or cast to integers (e.g., `if ((int)theImage > 1000)`). Use `(intptr_t)`.
3. **Upgrade Windows API Calls:**
   - In `SexyAppFramework/SexyAppBase.cpp`, update `SetWindowLong` calls that store pointers to use the 64-bit safe `SetWindowLongPtr` with `GWLP_USERDATA` and `(LONG_PTR)`.
4. **Replace Inline Assembly:**
   - In `SexyAppFramework/PerfTimer.cpp`, replace the GCC 32-bit inline assembly `__asm__ volatile ("rdtsc" : "=A" (cur));` with the cross-platform `__rdtsc()` intrinsic from `<x86intrin.h>`.
5. **Iterative Fixing:**
   - Continue resolving compiler errors (truncation from `size_t` to `int`, generic `long` vs `int` mismatches) until the codebase successfully compiles into an x86_64 executable.

**Completion Condition:**
When the codebase compiles successfully without critical 64-bit errors, print exactly:
`Session Is Done`