#include "mainwindow.h"
#include "hotzone.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    MainWindow *mainWindow = new MainWindow(0);
    mainWindow->show();
    mainWindow->grabKeyboard();
    return a.exec();
}
