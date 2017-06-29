#include <DApplication>

#include "welcome.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);

    Welcome w;
    w.show();

    return app.exec();
}
