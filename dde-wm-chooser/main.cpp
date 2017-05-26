#include "wmframe.h"
#include <DApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setApplicationName("deepin-wm-chooser");
    a.loadTranslator();

    QCommandLineOption config(QStringList() << "c" << "config", "");
    config.setValueName("ConfigPath");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(config);
    parser.process(a);

    WMFrame w;
    w.show();
    if (parser.isSet(config)) {
        w.setConfigPath(parser.value(config));
        w.show();
    } else {
        return 0;
    }

    return a.exec();
}
