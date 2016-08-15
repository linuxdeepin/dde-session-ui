#include "frame.h"
#include <QDebug>
#include <DApplication>
#include <DLog>

DWIDGET_USE_NAMESPACE

DUTIL_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("dde-wallpaper-chooser");
    a.setApplicationVersion("1.0");

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    Frame w;
    w.show();
    w.grabKeyboard();

    return a.exec();
}
