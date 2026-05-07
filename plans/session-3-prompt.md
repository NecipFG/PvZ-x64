# Session 3: Runtime & Serialization Debugging

**Objective:** Ensure the compiled 64-bit executable runs without crashing and maintains compatibility with existing 32-bit data files (save games, `.compiled` resources).

**Tasks:**
1. **Audit Data Serialization:**
   - Review `Lawn/System/SaveGame.cpp`, `Sexy.TodLib/Definition.cpp`, and any other file reading/writing binary data.
   - Identify structs written directly to disk using `sizeof(Struct)`. 
2. **Fix Struct Padding/Alignment:**
   - Because pointers are now 8 bytes, the size and padding of structs containing pointers or `size_t` have changed. Ensure explicit padding or fixed-width types (`int32_t`) are used so the 64-bit game can read 32-bit save files and `.compiled` files.
3. **Initial Startup Debugging:**
   - Run the 64-bit executable (using Wine64 or natively on Windows).
   - Debug and fix any immediate segmentation faults or access violations during the initialization phase (e.g., Resource Manager loading, app initialization).

**Completion Condition:**
When the game launches successfully to the main menu without crashing and can load basic assets/saves, print exactly:
`Session Is Done`