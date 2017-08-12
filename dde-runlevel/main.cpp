#include "runlevel.h"
#include <DApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("深度运行级别修改工具");
    a.setApplicationVersion("1.0");
    a.setTheme("light");

    QStringList arguments = QApplication::arguments();

    if (arguments.count() < 2)
        return 0;

    RunLevel w;
    w.setFile(arguments.at(1));
    w.show();

    return a.exec();
}
