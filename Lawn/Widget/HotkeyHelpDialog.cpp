#include "HotkeyHelpDialog.h"
#include "../../LawnApp.h"
#include "../../Resources.h"
#include "../../Sexy.TodLib/TodCommon.h"
#include "../../SexyAppFramework/Font.h"
#include "../Hotkey.h"

// Format a hotkey binding for display, e.g. "Ctrl+R", "Space", "F1".
static SexyString HotkeyKeyName(const Hotkey& h)
{
    SexyString aKeyStr;
    if (h.mMods & HOTKEY_MOD_CTRL)  aKeyStr += _S("Ctrl+");
    if (h.mMods & HOTKEY_MOD_SHIFT) aKeyStr += _S("Shift+");
    if (h.mMods & HOTKEY_MOD_ALT)   aKeyStr += _S("Alt+");

    switch (h.mKey)
    {
    case KEYCODE_SPACE:  aKeyStr += _S("Space"); break;
    case KEYCODE_RETURN: aKeyStr += _S("Enter"); break;
    case KEYCODE_ESCAPE: aKeyStr += _S("Esc");   break;
    case KEYCODE_F1:     aKeyStr += _S("F1");    break;
    case KEYCODE_F3:     aKeyStr += _S("F3");    break;
    default:
        if ((h.mKey >= 'A' && h.mKey <= 'Z') || (h.mKey >= '0' && h.mKey <= '9'))
        {
            char aBuf[2] = { (char)h.mKey, 0 };
            aKeyStr += SexyString(aBuf);
        }
        break;
    }
    return aKeyStr;
}

// Build the display rows from the global hotkey table, merging extra
// bindings for the same action into one row (e.g. "Space / Enter").
void HotkeyHelpDialog::BuildRows()
{
    mRows.clear();

    for (int i = 0; i < gHotkeyCount; i++)
    {
        const Hotkey& h = gHotkeys[i];
        if (HotkeyIsSeedAction(h.mAction))
            continue;

        SexyString aKeyStr = HotkeyKeyName(h);
        SexyString anActionStr = _S(h.mLabel);

        bool aMerged = false;
        for (Row& aRow : mRows)
        {
            if (aRow.mAction == anActionStr)
            {
                aRow.mKey += _S(" / ") + aKeyStr;
                aMerged = true;
                break;
            }
        }

        if (!aMerged)
            mRows.push_back(Row{ aKeyStr, anActionStr });
    }

    mRows.push_back(Row{ _S("1 - 9"), _S("Select seed packet by slot") });
}

HotkeyHelpDialog::HotkeyHelpDialog(LawnApp* theApp)
    : LawnDialog(theApp, Dialogs::DIALOG_HOTKEY_HELP, true,
                 _S("HOTKEYS"), _S(""), _S("CLOSE"),
                 Dialog::BUTTONS_FOOTER),
      mCol1Offset(0),
      mCol2Offset(0),
      mContentW(0),
      mRowHeight(0)
{
    mApp = theApp;
    BuildRows();

    Font* aFont = FONT_DWARVENTODCRAFT15;
    mRowHeight = aFont->GetHeight() + 6;

    // Size the columns from the widest entries (including the column headers)
    int aKeyW = aFont->StringWidth(_S("KEY"));
    int anActionW = aFont->StringWidth(_S("ACTION"));
    for (const Row& aRow : mRows)
    {
        int aRowKeyW = aFont->StringWidth(aRow.mKey);
        int aRowActionW = aFont->StringWidth(aRow.mAction);
        if (aRowKeyW > aKeyW) aKeyW = aRowKeyW;
        if (aRowActionW > anActionW) anActionW = aRowActionW;
    }

    const int aColGap = 30;
    mCol1Offset = 0;
    mCol2Offset = aKeyW + aColGap;
    mContentW = aKeyW + aColGap + anActionW;

    const int aHeaderRowH = mRowHeight + 6;  // column header + divider gap
    int aContentH = aHeaderRowH + (int)mRows.size() * mRowHeight + 8;

    CalcSize(mContentW + 16, aContentH);
    mApp->CenterDialog(this, mWidth, mHeight);
}

HotkeyHelpDialog::~HotkeyHelpDialog()
{
}

void HotkeyHelpDialog::Draw(Graphics* g)
{
    LawnDialog::Draw(g);

    Font* aFont = FONT_DWARVENTODCRAFT15;
    int aLeft = mContentInsets.mLeft + mBackgroundInsets.mLeft + 8;
    int aCol1X = aLeft + mCol1Offset;
    int aCol2X = aLeft + mCol2Offset;

    int aY = mContentInsets.mTop + mBackgroundInsets.mTop + DIALOG_HEADER_OFFSET;
    if (mDialogHeader.size() > 0)
        aY += -mHeaderFont->GetAscentPadding() + mHeaderFont->GetHeight() + mSpaceAfterHeader;

    // Column headers
    TodDrawString(g, _S("KEY"), aCol1X, aY, aFont, Color(255, 200, 60), DS_ALIGN_LEFT);
    TodDrawString(g, _S("ACTION"), aCol2X, aY, aFont, Color(255, 200, 60), DS_ALIGN_LEFT);
    aY += aFont->GetHeight() + 6;

    // Thin divider under the column headers
    g->SetColor(Color(255, 220, 120, 90));
    g->FillRect(aLeft, aY, mContentW, 2);
    aY += 8;

    for (const Row& aRow : mRows)
    {
        TodDrawString(g, aRow.mKey, aCol1X, aY, aFont, Color(255, 235, 130), DS_ALIGN_LEFT);
        TodDrawString(g, aRow.mAction, aCol2X, aY, aFont, Color(245, 245, 235), DS_ALIGN_LEFT);
        aY += mRowHeight;
    }
}

void HotkeyHelpDialog::KeyDown(KeyCode theKey)
{
    // Close on Esc / Enter / Space, or F1 to toggle the reference off
    if (theKey == KEYCODE_ESCAPE || theKey == KEYCODE_RETURN ||
        theKey == KEYCODE_SPACE || theKey == KEYCODE_F1)
    {
        mApp->KillDialog(mId);
    }
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
