#include "wallpaperitem.h"
#include "constants.h"
#include "thumbnailmanager.h"

#include <QPixmap>
#include <QLabel>
#include <QUrl>
#include <QDebug>

WallpaperItem::WallpaperItem(QFrame *parent, const QString &path) :
    QFrame(parent),
    m_path(path)
{
    QUrl url = QUrl::fromPercentEncoding(path.toUtf8());
    QString realPath = url.toLocalFile();

    ThumbnailManager * tnm = ThumbnailManager::instance();

    QPixmap pix;
    if (!tnm->find(QUrl::toPercentEncoding(path), &pix)) {
        pix = QPixmap(realPath).scaled(QSize(ItemWidth, ItemHeight), Qt::KeepAspectRatio);
        tnm->replace(QUrl::toPercentEncoding(path), pix);
    }

    m_picture = new QLabel(this);
    m_picture->setFixedSize(QSize(ItemWidth, ItemHeight));
    m_picture->setScaledContents(true);
    m_picture->setPixmap(pix);
}
