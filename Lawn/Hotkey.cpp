#include "Hotkey.h"
#include "../SexyAppFramework/WidgetManager.h"
#include "../LawnApp.h"

using namespace Sexy;

const Hotkey gHotkeys[] = {
    { KEYCODE_SPACE,  HOTKEY_MOD_NONE,  HotkeyAction::Pause,         "Pause / Resume" },
    { KEYCODE_RETURN, HOTKEY_MOD_NONE,  HotkeyAction::Pause,         "Pause / Resume" },
    { 'F',            HOTKEY_MOD_NONE,  HotkeyAction::FastForward,   "Toggle Fast-Forward (2x)" },
    { 'R',            HOTKEY_MOD_CTRL,  HotkeyAction::Restart,       "Restart Level (with confirm)" },
    { KEYCODE_ESCAPE, HOTKEY_MOD_NONE,  HotkeyAction::Menu,          "Open in-game menu" },
    { 'Q',            HOTKEY_MOD_NONE,  HotkeyAction::ToggleShovel,  "Pick up / cancel shovel" },
    { 'M',            HOTKEY_MOD_NONE,  HotkeyAction::ToggleMute,    "Toggle sound (music + SFX)" },
    { KEYCODE_F1,     HOTKEY_MOD_NONE,  HotkeyAction::ShowHelp,      "Show this help dialog" },
    { KEYCODE_F3,     HOTKEY_MOD_NONE,  HotkeyAction::ToggleFps,     "Toggle FPS overlay" },
    { '1',            HOTKEY_MOD_NONE,  HotkeyAction::SelectSeed1,   "Select seed slot 1" },
    { '2',            HOTKEY_MOD_NONE,  HotkeyAction::SelectSeed2,   "Select seed slot 2" },
    { '3',            HOTKEY_MOD_NONE,  HotkeyAction::SelectSeed3,   "Select seed slot 3" },
    { '4',            HOTKEY_MOD_NONE,  HotkeyAction::SelectSeed4,   "Select seed slot 4" },
    { '5',            HOTKEY_MOD_NONE,  HotkeyAction::SelectSeed5,   "Select seed slot 5" },
    { '6',            HOTKEY_MOD_NONE,  HotkeyAction::SelectSeed6,   "Select seed slot 6" },
    { '7',            HOTKEY_MOD_NONE,  HotkeyAction::SelectSeed7,   "Select seed slot 7" },
    { '8',            HOTKEY_MOD_NONE,  HotkeyAction::SelectSeed8,   "Select seed slot 8" },
    { '9',            HOTKEY_MOD_NONE,  HotkeyAction::SelectSeed9,   "Select seed slot 9" },
    { 'J',            HOTKEY_MOD_NONE,  HotkeyAction::ToggleAutoCollectSun, "Toggle Auto-Collect Sun" },
};

const int gHotkeyCount = sizeof(gHotkeys) / sizeof(gHotkeys[0]);

uint8_t HotkeyGetMods()
{
    uint8_t mods = HOTKEY_MOD_NONE;
    WidgetManager* wm = gLawnApp->mWidgetManager;
    if (wm->mKeyDown[KEYCODE_SHIFT])   mods |= HOTKEY_MOD_SHIFT;
    if (wm->mKeyDown[KEYCODE_CONTROL]) mods |= HOTKEY_MOD_CTRL;
    if (wm->mKeyDown[KEYCODE_MENU])    mods |= HOTKEY_MOD_ALT;
    return mods;
}

bool HotkeyMatches(const Hotkey& h, KeyCode key, uint8_t mods)
{
    if (h.mKey != key) return false;
    if (h.mMods == HOTKEY_MOD_NONE)
        return mods == HOTKEY_MOD_NONE;
    return (mods & h.mMods) == h.mMods;
}

bool HotkeyIsSeedAction(HotkeyAction action)
{
    return action >= HotkeyAction::SelectSeed1 &&
           action <= HotkeyAction::SelectSeed9;
}
