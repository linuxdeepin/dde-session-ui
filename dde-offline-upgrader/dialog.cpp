#include "dialog.h"

Dialog::Dialog() : DDialog()
{
    setTitle(tr("Updates downloaded, update now?"));
    setIcon(QIcon::fromTheme("system_updated", QIcon::fromTheme("system-software-update")).pixmap(48));

    addButton(tr("Remind Later"));
    addButton(tr("Update Now"));
    addButton(tr("Not Now"));

    new Adaptor(this);
}

Adaptor::Adaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

void Adaptor::RaiseWindow()
{
    QWidget * pp = qobject_cast<QWidget*>(parent());
    pp->activateWindow();
    pp->raise();
}
