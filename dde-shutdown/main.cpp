#include <QApplication>
#include <QtCore/QTranslator>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>

#include "logmanager.h"
#include "app/shutdownframe.h"
#include "dbus/dbuscontrolcenter.h"
#include "backgroundlabel.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    LogManager::instance()->debug_log_console_on();
    // hide dde-control-center
    DBusControlCenter *DCCInter = new DBusControlCenter;
    if (DCCInter->isValid())
        DCCInter->HideImmediately();
    DCCInter->deleteLater();

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);

    QCommandLineOption hide(QStringList() << "H" << "Hide", "hide buttons", "btns");
    QCommandLineOption disable(QStringList() << "D" << "Disable", "disable buttons", "btns");
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(hide);
    parser.addOption(disable);
    parser.process(app);

    qDebug() << parser.values(hide);
    qDebug() << parser.values(disable);

    BackgroundLabel* shutdown_background = new BackgroundLabel(true);
    ShutdownFrame w(shutdown_background);
//    w.hideBtns(parser.values(hide));
//    w.disableBtns(parser.values(disable));
    w.show();
    return app.exec();
}
