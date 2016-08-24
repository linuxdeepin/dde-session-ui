#include "frame.h"
#include <QDebug>
#include <DApplication>
#include <QTranslator>
#include <DLog>

DWIDGET_USE_NAMESPACE

DUTIL_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("dde-wallpaper-chooser");
    a.setApplicationVersion("1.0");

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    Frame w;
    w.show();
    w.grabKeyboard();

    return a.exec();
}
