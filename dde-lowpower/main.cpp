#include "window.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDebug>

#define DBUS_SERV "com.deepin.dde.lowpower"
#define DBUS_PATH "/com/deepin/dde/lowpower"
#define DBUS_IFCE "com.deepin.dde.LowPower"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineOption raiseOption("raise", "raise the main window.");
    QCommandLineOption quitOption("quit", "quit all dde-lowpower instances.");

    // command line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(raiseOption);
    parser.addOption(quitOption);
    parser.process(a);

    Window w;
    LowPowerAdaptor adaptor(&w);
    QDBusInterface iface(DBUS_SERV,
                         DBUS_PATH,
                         DBUS_IFCE,
                         QDBusConnection::sessionBus());

    bool result = QDBusConnection::sessionBus().registerService(DBUS_SERV);
    if (result) {
        if (parser.isSet(quitOption)) {
            iface.call("Quit");
        } else {
            w.show();
            w.grabMouse();
            w.grabKeyboard();

            QDBusConnection::sessionBus().registerObject(DBUS_PATH, &w);
        }
    } else {
        if (parser.isSet(raiseOption)) {
            iface.call("Raise");
        }
    }

    return a.exec();
}
