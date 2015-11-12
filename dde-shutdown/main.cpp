#include <QApplication>
#include <QtCore/QTranslator>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>

#include "app/shutdownmanager.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);

    QCommandLineOption hide(QStringList() << "H" << "Hide", "hide buttons", "btns");
    QCommandLineOption disable(QStringList() << "D" << "Disable", "disable buttons", "btns");
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(hide);
    parser.addOption(disable);
    parser.process(app);

    qDebug() << parser.values(hide);
    qDebug() << parser.values(disable);

    ShutdownManager w;
    w.hideBtns(parser.values(hide));
    w.disableBtns(parser.values(disable));
    w.show();

    QTimer::singleShot(1000, &w, SLOT(shutDownFramegrabKeyboard()));

    return app.exec();
}
