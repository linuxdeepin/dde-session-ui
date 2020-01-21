#ifndef CLIPBOARDLOADERINTERFACE_H
#define CLIPBOARDLOADERINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.deepin.dde.ClipboardLoader
 */
class ComDeepinDdeOsd_plugin: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.dde.notify_plugin"; }

public:
    ComDeepinDdeOsd_plugin(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~ComDeepinDdeOsd_plugin();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> notifyComing()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("notifyComing"), argumentList);
    }
};

namespace com {
  namespace deepin {
    namespace dde {
      typedef ::ComDeepinDdeOsd_plugin NotifyPluginInterface;
    }
  }
}
#endif
