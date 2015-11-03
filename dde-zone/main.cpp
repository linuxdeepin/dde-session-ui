#include "mainwindow.h"
#include "hotzone.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDesktopWidget* desktopItem = a.desktop();
    MainWindow* mainWindow = new MainWindow(desktopItem->width(),desktopItem->height(),0);
    mainWindow->show();
    mainWindow->grabKeyboard();
    return a.exec();
}
