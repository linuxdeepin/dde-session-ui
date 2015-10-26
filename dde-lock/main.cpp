#include "lockframe.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    LockFrame lf;
    lf.show();

    return app.exec();
}
