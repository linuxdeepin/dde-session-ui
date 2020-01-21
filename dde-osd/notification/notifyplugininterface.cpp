#include "notifyplugininterface.h"

ComDeepinDdeOsd_plugin::ComDeepinDdeOsd_plugin(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

ComDeepinDdeOsd_plugin::~ComDeepinDdeOsd_plugin()
{
}
