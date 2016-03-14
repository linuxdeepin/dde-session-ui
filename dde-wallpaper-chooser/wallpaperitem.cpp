#include "wallpaperitem.h"
#include "constants.h"

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
    QPixmap pix = QPixmap(realPath).scaled(size(), Qt::KeepAspectRatio);

    m_picture = new QLabel(this);
    m_picture->setFixedSize(QSize(ItemWidth, ItemHeight));
    m_picture->setScaledContents(true);
    m_picture->setPixmap(pix);

    qDebug() << m_picture->size();
}
