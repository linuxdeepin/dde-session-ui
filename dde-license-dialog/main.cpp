#include "mainwindow.h"
#include <DApplication>
#include <QTranslator>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFile>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setTheme("light");

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    QCommandLineOption title(QStringList() << "t" << "title", "");
    QCommandLineOption content(QStringList() << "c" << "content", "");
    QCommandLineOption checkbox(QStringList() << "a" << "allow", "");

    title.setValueName("TitleName");
    content.setValueName("Content");
    checkbox.setValueName("Check");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(title);
    parser.addOption(content);
    parser.addOption(checkbox);
    parser.process(a);

    MainWindow w;
    w.show();

    if (parser.isSet(title)) {
        w.setTitle(parser.value(title));
    }

    if (parser.isSet(content)) {
        const QString& path = parser.value(content);
        QFile file(path);
        if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
            w.setSource(file.readAll());
        }
    }

    w.setAllowCheckBoxText(parser.value(checkbox));

    return a.exec();
}
