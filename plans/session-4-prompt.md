# Session 4: Final Polish & Gameplay Verification

**Objective:** Address any remaining subtle 64-bit bugs, UI glitches, and verify core gameplay functionality.

**Tasks:**
1. **Address Truncation Warnings:**
   - Fix remaining compiler warnings where 64-bit values (`size_t`, `ptrdiff_t`) are truncated to 32-bit integers, ensuring no data loss occurs in critical game logic.
2. **Verify Subsystems:**
   - Check particle effects (`Sexy.TodLib/TodParticle.cpp`) and memory managers to ensure they behave correctly with 64-bit addresses.
   - Verify audio playback via BASS is functioning correctly.
3. **Playtesting & UI Checks:**
   - Play a test level to ensure UI elements, rendering coordinates, and input handling (mouse clicks) map correctly to the screen in 64-bit.
   - Check the Zen Garden and Mini-games for any edge-case crashes.

**Completion Condition:**
When the game is fully playable and stable in 64-bit mode, print exactly:
`Session Is Done`