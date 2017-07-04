#include <DApplication>
#include <DLog>

#include "welcome.h"
#include "welcomeservice.h"

DWIDGET_USE_NAMESPACE
DUTIL_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    Welcome w;
    WelcomeService serviceAdaptor(&w);
    Q_UNUSED(serviceAdaptor);

    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.registerService("com.deepin.dde.Welcome") ||
        !connection.registerObject("/com/deepin/dde/Welcome", &w))
    {
        qDebug() << "dbus service already registered!";

        return -1;
    }

    return app.exec();
}
