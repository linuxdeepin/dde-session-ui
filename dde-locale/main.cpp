#include "localeformat.h"
#include <DApplication>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setApplicationName("区域设置");
    a.setApplicationVersion("1.0");
    a.setTheme("light");

    LocaleFormat w;
    w.show();

    return a.exec();
}
