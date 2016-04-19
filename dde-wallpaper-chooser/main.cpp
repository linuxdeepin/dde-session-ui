#include "frame.h"
#include <QDebug>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("dde-wallpaper-chooser");
    a.setApplicationVersion("1.0");

    Frame w;
    w.show();
    w.grabKeyboard();

    return a.exec();
}
