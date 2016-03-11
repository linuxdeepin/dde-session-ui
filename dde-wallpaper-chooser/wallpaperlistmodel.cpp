#include "wallpaperlistmodel.h"

#include <QPixmap>

static const int ItemWidth = 100;
static const int ItemHeight = 48;

WallpaperListModel::WallpaperListModel(QObject * parent) :
    QStringListModel(parent)
{

}

QVariant WallpaperListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole) {
        QString path = stringList().at(index.row());
        return QPixmap(path).scaled(ItemWidth, ItemHeight, Qt::KeepAspectRatio);
    }

    return QVariant();
}
