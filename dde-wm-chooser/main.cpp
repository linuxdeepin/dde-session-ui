#include "wmframe.h"
#include <DApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QTranslator>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setApplicationName("deepin-wm-chooser");

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    QCommandLineOption config(QStringList() << "c" << "config", "");
    config.setValueName("ConfigPath");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(config);
    parser.process(a);

    WMFrame w;
    if (parser.isSet(config)) {
        w.setConfigPath(parser.value(config));
        w.show();
    } else {
        return 0;
    }

    return a.exec();
}
