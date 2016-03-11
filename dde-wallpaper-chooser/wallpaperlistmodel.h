#ifndef WALLPAPERLISTMODEL_H
#define WALLPAPERLISTMODEL_H

#include <QStringListModel>

class AppearanceDaemonInterface;
class WallpaperListModel : public QStringListModel
{
public:
    WallpaperListModel(QObject * parent = 0);

protected:
    virtual QVariant data(const QModelIndex &index, int role) const;

private:
    AppearanceDaemonInterface * m_dbusAppearance;

    QStringList processListReply(QString &reply) const;
};

#endif // WALLPAPERLISTMODEL_H
