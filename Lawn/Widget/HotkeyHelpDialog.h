#ifndef __HOTKEYHELPDIALOG_H__
#define __HOTKEYHELPDIALOG_H__

#include "LawnDialog.h"
#include <vector>

class HotkeyHelpDialog : public LawnDialog
{
public:
    struct Row
    {
        SexyString mKey;
        SexyString mAction;
    };

    std::vector<Row> mRows;
    int              mCol1Offset;   // key column x, relative to content left
    int              mCol2Offset;   // action column x, relative to content left
    int              mContentW;     // width of the two-column content block
    int              mRowHeight;

public:
    HotkeyHelpDialog(LawnApp* theApp);
    virtual ~HotkeyHelpDialog();

    virtual void    Draw(Graphics* g);
    virtual void    KeyDown(KeyCode theKey);
    virtual void    AddedToManager(WidgetManager* theWidgetManager);
    virtual void    RemovedFromManager(WidgetManager* theWidgetManager);
    virtual void    ButtonDepress(int theId);

    void            BuildRows();
};

#endif
