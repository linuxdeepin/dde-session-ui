#include <QApplication>
#include "app/loginmanager.h"

#include "logmanager.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    LogManager::instance()->debug_log_console_on();
    /* the logmanager is used to output debug log */

    LoginManager  w;
    w.show();

    return a.exec();
}
