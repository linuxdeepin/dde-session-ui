#include "touchscreensetting.h"

#include <DApplication>

#include <QCommandLineParser>
#include <QDebug>
#include <QTranslator>

#include <com_deepin_daemon_display.h>

DWIDGET_USE_NAMESPACE
using namespace com::deepin::daemon;

int main(int argc, char *argv[])
{
    DApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setQuitOnLastWindowClosed(true);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" +
                    QLocale::system().name());
    app.installTranslator(&translator);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("touchscreen",
                                 "the UUID of touchscreen.",
                                 "[touchscreen]");
    parser.process(app);

    const QStringList &posArguments = parser.positionalArguments();

    if (posArguments.isEmpty()) {
        qDebug() << "empty touchscreen UUID";
        return -1;
    }

    Display display("com.deepin.daemon.Display", "/com/deepin/daemon/Display", QDBusConnection::sessionBus());
    // 只有一块屏幕时不显示触摸选择提示框
    if (display.monitors().size() < 2) {
        return -1;
    }

    TouchscreenSetting s(posArguments.first());
    s.show();
    s.moveToCenter();
    return app.exec();
}
