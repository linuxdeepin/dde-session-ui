#include <QApplication>
#include "app/sessionmanagement.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SessionManagement w;
    w.show();

    return a.exec();
}
