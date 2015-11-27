#include <QApplication>
#include <QtCore/QTranslator>
#include "app/loginframe.h"
#include <QLabel>

#include "logmanager.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

//    LogManager::instance()->debug_log_console_on();
    /* the logmanager is used to output debug log */
    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    const QString &backgroundUrl = ":/theme/background/default_background.jpg";
    LoginFrame  w(backgroundUrl);
    w.show();
    w.grabKeyboard();

    return a.exec();
}
