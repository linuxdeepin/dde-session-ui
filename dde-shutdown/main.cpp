#include <QApplication>
#include <QtCore/QTranslator>
#include "app/shutdownmanager.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);
    ShutdownManager w;
    w.show();
    return app.exec();
}
