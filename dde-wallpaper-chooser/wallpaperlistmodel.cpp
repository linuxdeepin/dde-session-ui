#include "wallpaperlistmodel.h"
#include "dbus/appearancedaemon_interface.h"

#include <QPixmap>
#include <QVariant>
#include <QUrl>
#include <QJsonDocument>

static const int ItemWidth = 100;
static const int ItemHeight = 48;

static const QString AppearanceServ = "com.deepin.daemon.Appearance";
static const QString AppearancePath = "/com/deepin/daemon/Appearance";

WallpaperListModel::WallpaperListModel(QObject * parent) :
    QStringListModel(parent),
    m_dbusAppearance(new AppearanceDaemonInterface(AppearanceServ,
                                                   AppearancePath,
                                                   QDBusConnection::sessionBus(),
                                                   this))
{
    QDBusPendingReply<QString> reply = m_dbusAppearance->List("background");
    reply.waitForFinished();

    QString value = reply.value();
    setStringList(processListReply(value));
}

QVariant WallpaperListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole) {
        QString path = stringList().at(index.row());
        QUrl url = QUrl::fromPercentEncoding(path.toUtf8());

        return QPixmap(url.toLocalFile()).scaled(ItemWidth, ItemHeight, Qt::KeepAspectRatio);
    }

    return QVariant();
}

QStringList WallpaperListModel::processListReply(QString &reply) const
{
    QStringList result;

    QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8());
    if (doc.isArray()) {
        QJsonArray arr = doc.array();
        foreach (QJsonValue val, arr) {
            QJsonObject obj = val.toObject();
            result.append(obj["Id"].toString());
        }
    }

    return result;
}
