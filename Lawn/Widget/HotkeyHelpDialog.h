#ifndef __HOTKEYHELPDIALOG_H__
#define __HOTKEYHELPDIALOG_H__

#include "LawnDialog.h"

class HotkeyHelpDialog : public LawnDialog
{
public:
    HotkeyHelpDialog(LawnApp* theApp);
    virtual ~HotkeyHelpDialog();

    virtual void    Draw(Graphics* g);
    virtual void    AddedToManager(WidgetManager* theWidgetManager);
    virtual void    RemovedFromManager(WidgetManager* theWidgetManager);
    virtual void    ButtonDepress(int theId);
};

#endif
