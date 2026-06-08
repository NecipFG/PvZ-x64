#ifndef __HOTKEY_H__
#define __HOTKEY_H__

#include "../SexyAppFramework/KeyCodes.h"
#include <cstdint>

enum class HotkeyAction
{
    Pause,
    FastForward,
    ToggleAutoCollectSun,
    Restart,
    Menu,
    ToggleShovel,
    SelectSeed1,
    SelectSeed2,
    SelectSeed3,
    SelectSeed4,
    SelectSeed5,
    SelectSeed6,
    SelectSeed7,
    SelectSeed8,
    SelectSeed9,
    ToggleMute,
    ShowHelp,
    ToggleFps
};

struct Hotkey
{
    Sexy::KeyCode  mKey;
    uint8_t        mMods;
    HotkeyAction   mAction;
    const char*    mLabel;
};

constexpr uint8_t HOTKEY_MOD_NONE  = 0;
constexpr uint8_t HOTKEY_MOD_SHIFT = 1 << 0;
constexpr uint8_t HOTKEY_MOD_CTRL  = 1 << 1;
constexpr uint8_t HOTKEY_MOD_ALT   = 1 << 2;

extern const Hotkey gHotkeys[];
extern const int gHotkeyCount;

uint8_t HotkeyGetMods();
bool HotkeyMatches(const Hotkey& h, Sexy::KeyCode key, uint8_t mods);
bool HotkeyIsSeedAction(HotkeyAction action);

#endif
