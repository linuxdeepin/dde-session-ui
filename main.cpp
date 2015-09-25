#include <QApplication>
#include "sessionmanager.h"
#include "leftcornerwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SessionManager w;
    w.show();

    return a.exec();
}
