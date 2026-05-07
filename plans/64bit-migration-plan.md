# 64-Bit Migration Master Plan

This plan organizes the 64-bit migration of Plants vs. Zombies into 4 distinct sessions. Each session has a dedicated prompt file to guide the AI's focus.

### Session Prompts
- **Session 1:** `session-1-prompt.md` - Toolchain & Build Scaffolding
- **Session 2:** `session-2-prompt.md` - The Great Pointer Purge (Fixing compilation errors)
- **Session 3:** `session-3-prompt.md` - Runtime & Serialization Debugging (Padding/Alignment)
- **Session 4:** `session-4-prompt.md` - Final Polish & Gameplay Verification

### Workflow
1. The user will provide the contents of a session prompt to start a session.
2. The AI will execute the tasks outlined in the prompt.
3. When the tasks for that session are complete, the AI will output exactly `Session Is Done`.
4. The user will manually initiate the next session using the next prompt.