#include <QCoreApplication>

#include <QDebug>
#include <QDBusConnection>
#include <QDBusError>

#include "dmanproxy.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    DManProxy dmp;

    auto conn = QDBusConnection::sessionBus();

    if (!conn.registerService(SERVICE_NAME)) {
        qDebug() << "registerService failed:" << conn.lastError();
        return EXIT_FAILURE;
    }
    if (!conn.registerObject(SERVICE_PATH, &dmp, QDBusConnection::ExportAllSlots)) {
        qDebug() << "registerObject failed:" << conn.lastError();
        return EXIT_FAILURE;
    }

    return app.exec();
}
