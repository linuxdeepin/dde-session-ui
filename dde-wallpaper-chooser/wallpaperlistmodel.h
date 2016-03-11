#ifndef WALLPAPERLISTMODEL_H
#define WALLPAPERLISTMODEL_H

#include <QStringListModel>

class WallpaperListModel : public QStringListModel
{
public:
    WallpaperListModel(QObject * parent = 0);

private:
    virtual QVariant data(const QModelIndex &index, int role) const;
};

#endif // WALLPAPERLISTMODEL_H
