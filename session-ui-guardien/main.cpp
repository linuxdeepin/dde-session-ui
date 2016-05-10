#include <QApplication>
#include <QProcess>

#include "guardien.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Guardien guardien;
    guardien.start();

    return a.exec();
}
