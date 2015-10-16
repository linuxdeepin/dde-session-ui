#include <QApplication>
#include "app/sessionlogin.h"

#include "logmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogManager::instance()->debug_log_console_on();
    SessionLogin w;
    w.show();


    return a.exec();
}
