#include "lockframe.h"
#include "logmanager.h"
#include "dbus/dbuscontrolcenter.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);

    // hide dde-control-center
    DBusControlCenter *DCCInter = new DBusControlCenter;
    if (DCCInter->isValid())
        DCCInter->HideImmediately();
    DCCInter->deleteLater();

    LogManager::instance()->debug_log_console_on();
    LockFrame lf;
    lf.show();

    lf.grabKeyboard();
    return app.exec();
}
