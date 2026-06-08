#include "HotkeyHelpDialog.h"
#include "../../LawnApp.h"
#include "../../Resources.h"
#include "../../Sexy.TodLib/TodCommon.h"
#include "../../SexyAppFramework/Font.h"
#include "../Hotkey.h"

HotkeyHelpDialog::HotkeyHelpDialog(LawnApp* theApp)
    : LawnDialog(theApp, Dialogs::DIALOG_HOTKEY_HELP, true,
                 _S("HOTKEYS"), _S(""), _S("CLOSE"),
                 Dialog::BUTTONS_FOOTER)
{
    mApp = theApp;
    CalcSize(220, 300);
    mApp->CenterDialog(this, mWidth, mHeight);
}

HotkeyHelpDialog::~HotkeyHelpDialog()
{
}

void HotkeyHelpDialog::Draw(Graphics* g)
{
    LawnDialog::Draw(g);

    Font* aFont = FONT_DWARVENTODCRAFT15;
    int aLineH = aFont->GetHeight() + 2;
    int aContentLeft = mContentInsets.mLeft + mBackgroundInsets.mLeft + 8;
    int aCol1X = aContentLeft;
    int aCol2X = aContentLeft + 110;

    int aY = mContentInsets.mTop + mBackgroundInsets.mTop + DIALOG_HEADER_OFFSET;
    if (mDialogHeader.size() > 0)
        aY += mHeaderFont->GetHeight() + mSpaceAfterHeader;
    aY += 6;

    TodDrawString(g, _S("Key"), aCol1X, aY, aFont, Color(220, 190, 80), DS_ALIGN_LEFT);
    TodDrawString(g, _S("Action"), aCol2X, aY, aFont, Color(220, 190, 80), DS_ALIGN_LEFT);
    aY += aLineH + 2;

    for (int i = 0; i < gHotkeyCount; i++)
    {
        const Hotkey& h = gHotkeys[i];
        if (HotkeyIsSeedAction(h.mAction))
            continue;

        SexyString aKeyStr;
        if (h.mMods & HOTKEY_MOD_CTRL)   aKeyStr += _S("Ctrl+");
        if (h.mMods & HOTKEY_MOD_SHIFT)  aKeyStr += _S("Shift+");
        if (h.mMods & HOTKEY_MOD_ALT)    aKeyStr += _S("Alt+");

        if (h.mKey == KEYCODE_SPACE)       aKeyStr += _S("Space");
        else if (h.mKey == KEYCODE_RETURN) aKeyStr += _S("Enter");
        else if (h.mKey == KEYCODE_ESCAPE) aKeyStr += _S("Esc");
        else if (h.mKey == KEYCODE_F1)     aKeyStr += _S("F1");
        else if (h.mKey == KEYCODE_F3)     aKeyStr += _S("F3");
        else if (h.mKey >= 'A' && h.mKey <= 'Z')
        {
            char buf[2] = { (char)h.mKey, 0 };
            aKeyStr += SexyString(buf);
        }
        else if (h.mKey >= '0' && h.mKey <= '9')
        {
            char buf[2] = { (char)h.mKey, 0 };
            aKeyStr += SexyString(buf);
        }

        TodDrawString(g, aKeyStr, aCol1X, aY, aFont, Color(255, 220, 100), DS_ALIGN_LEFT);
        TodDrawString(g, _S(h.mLabel), aCol2X, aY, aFont, Color(240, 240, 240), DS_ALIGN_LEFT);
        aY += aLineH;
    }

    aY += 4;
    TodDrawString(g, _S("1-9"), aCol1X, aY, aFont, Color(255, 220, 100), DS_ALIGN_LEFT);
    TodDrawString(g, _S("Select seed packet by slot"), aCol2X, aY, aFont, Color(240, 240, 240), DS_ALIGN_LEFT);
}

void HotkeyHelpDialog::AddedToManager(WidgetManager* theWidgetManager)
{
    LawnDialog::AddedToManager(theWidgetManager);
}

void HotkeyHelpDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
    LawnDialog::RemovedFromManager(theWidgetManager);
}

void HotkeyHelpDialog::ButtonDepress(int theId)
{
    LawnDialog::ButtonDepress(theId);
    if (theId == Dialog::ID_FOOTER)
    {
        mApp->KillDialog(mId);
    }
}
