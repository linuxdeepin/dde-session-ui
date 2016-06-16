#include "dialog.h"

Dialog::Dialog() : DDialog()
{
    setTitle(tr("Updates downloaded, update now?"));
    setIcon(QIcon::fromTheme("system_updated", QIcon::fromTheme("system-software-update")).pixmap(48));

    addButton(tr("Remind Later"));
    addButton(tr("Update Now"));
    addButton(tr("Not Now"));
}
