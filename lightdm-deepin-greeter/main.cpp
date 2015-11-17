#include <QApplication>
#include <QtCore/QTranslator>
#include "app/loginmanager.h"

#include "logmanager.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    LogManager::instance()->debug_log_console_on();
    /* the logmanager is used to output debug log */
    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    LoginManager  w;
    w.show();
    w.grabKeyboard();
    return a.exec();
}
