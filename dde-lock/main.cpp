#include "lockframe.h"
#include "logmanager.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    LogManager::instance()->debug_log_console_on();
    LockFrame lf;
    lf.show();

    lf.grabKeyboard();
    return app.exec();
}
