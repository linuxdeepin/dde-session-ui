#include "frame.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Frame w;
    w.show();

    return a.exec();
}
