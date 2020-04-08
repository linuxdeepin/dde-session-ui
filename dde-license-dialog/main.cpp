#include "mainwindow.h"
#include <DApplication>
#include <QTranslator>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFile>
#include <QDesktopWidget>
#include <QDebug>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
//    a.setTheme("light");

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    QCommandLineOption title(QStringList() << "t" << "title", "");
    QCommandLineOption content(QStringList() << "c" << "content", "");
    QCommandLineOption checkbox(QStringList() << "a" << "allow", "");
    QCommandLineOption encontent(QStringList() << "e" << "english content", "");

    title.setValueName("TitleName");
    content.setValueName("Content");
    checkbox.setValueName("Check");
    encontent.setValueName("EnContent");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(title);
    parser.addOption(content);
    parser.addOption(checkbox);
    parser.addOption(encontent);
    parser.process(a);

    MainWindow w;

    if (parser.isSet(title)) {
        w.setTitle(parser.value(title));
    }

    if (parser.isSet(content)) {
        w.setCnSource(parser.value(content));
    }

    if (parser.isSet(encontent)) {
        w.setEnSource(parser.value(encontent));
    }

    w.setAllowCheckBoxText(parser.value(checkbox));
    w.updateLocaleSource();

    w.moveToCenter();
    w.show();


    return a.exec();
}
