#include "mainwindow.h"
#include "hotzone.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *mainWindow = new MainWindow(0);
    mainWindow->show();
    mainWindow->grabKeyboard();
    return a.exec();
}
