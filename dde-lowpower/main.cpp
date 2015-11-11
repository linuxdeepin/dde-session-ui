#include "window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Window w;
    w.show();
    w.grabMouse();
    w.grabKeyboard();

    return a.exec();
}
