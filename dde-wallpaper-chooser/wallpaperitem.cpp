#include "wallpaperitem.h"
#include "constants.h"
#include "thumbnailmanager.h"
#include "button.h"

#include <QEvent>
#include <QPixmap>
#include <QLabel>
#include <QUrl>
#include <QDebug>
#include <QVBoxLayout>
#include <QtConcurrent>

WallpaperItem::WallpaperItem(QFrame *parent, const QString &path) :
    QFrame(parent),
    m_path(path),
    m_thumbnailerWatcher(new QFutureWatcher<QPixmap>(this))
{
    initUI();
    initPixmap();
    initAnimation();

    connect(m_thumbnailerWatcher, &QFutureWatcher<QPixmap>::finished, this, &WallpaperItem::thumbnailFinished);
}

WallpaperItem::~WallpaperItem()
{

}

void WallpaperItem::initUI()
{
    setAttribute(Qt::WA_TranslucentBackground);

    m_wrapper = new QFrame(this);
    m_wrapper->setFixedSize(ItemWidth, ItemHeight * 2);
    m_wrapper->setAttribute(Qt::WA_TranslucentBackground);

    m_picture = new QLabel(m_wrapper);
    m_picture->setFixedSize(ItemWidth, ItemHeight);
    m_picture->setAlignment(Qt::AlignCenter);
    m_picture->installEventFilter(this);

    QFrame * buttonArea = new QFrame(m_wrapper);
    buttonArea->setFixedSize(m_picture->size());
    buttonArea->move(0, ItemHeight);

    QVBoxLayout * buttonLayout = new QVBoxLayout(buttonArea);
    buttonLayout->setSpacing(0);
    buttonLayout->setMargin(0);

    m_desktopButton = new Button;
    m_desktopButton->setText(tr("Only desktop"));
    m_desktopButton->setAttract(false);
    m_lockButton = new Button;
    m_lockButton->setText(tr("Only lock screen"));
    m_lockButton->setAttract(false);

    buttonLayout->addWidget(m_desktopButton);
    buttonLayout->addWidget(m_lockButton);

    connect(m_desktopButton, &Button::clicked, this, &WallpaperItem::desktopButtonClicked);
    connect(m_lockButton, &Button::clicked, this, &WallpaperItem::lockButtonClicked);
}

void WallpaperItem::initAnimation()
{
    m_upAnim = new QPropertyAnimation(m_wrapper, "pos");
    m_upAnim->setDuration(300);
    m_upAnim->setStartValue(QPoint(0, 0));
    m_upAnim->setEndValue(QPoint(0, -ItemHeight));

    m_downAnim = new QPropertyAnimation(m_wrapper, "pos");
    m_downAnim->setDuration(300);
    m_downAnim->setStartValue(QPoint(0, -ItemHeight));
    m_downAnim->setEndValue(QPoint(0, 0));
}

void WallpaperItem::initPixmap()
{
    ThumbnailManager * tnm = ThumbnailManager::instance();

    QPixmap pix;
    if (!tnm->find(QUrl::toPercentEncoding(m_path), &pix)) {
        QFuture<QPixmap> f = QtConcurrent::run(this, &WallpaperItem::thumbnailImage);
        m_thumbnailerWatcher->setFuture(f);
    } else {
        m_picture->setPixmap(pix);
    }
}

QPixmap WallpaperItem::thumbnailImage()
{
    QUrl url = QUrl::fromPercentEncoding(m_path.toUtf8());
    QString realPath = url.toLocalFile();

    ThumbnailManager * tnm = ThumbnailManager::instance();
    QPixmap pix = QPixmap(realPath).scaled(QSize(ItemWidth, ItemHeight), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    tnm->replace(QUrl::toPercentEncoding(m_path), pix);

    return pix;
}

bool WallpaperItem::eventFilter(QObject * obj, QEvent * event)
{
    if (obj == m_picture && event->type() == QEvent::MouseButtonPress) {
        emit pressed();

        return true;
    } else if (obj == m_picture && event->type() == QEvent::Enter) {
        emit hoverIn();

        return true;
    } else if (obj == m_picture && event->type() == QEvent::Leave) {
        emit hoverOut();

        return true;
    }

    return false;
}

void WallpaperItem::slideUp()
{
    if (m_upAnim->endValue().toPoint() != m_wrapper->pos()) {
        m_upAnim->start();
    }
}

void WallpaperItem::slideDown()
{
    if (m_downAnim->endValue().toPoint() != m_wrapper->pos()) {
        m_downAnim->start();
    }
}

QString WallpaperItem::getPath()
{
    QUrl url = QUrl::fromPercentEncoding(m_path.toUtf8());
    return url.toLocalFile();
}

void WallpaperItem::thumbnailFinished()
{
    QFuture<QPixmap> f = m_thumbnailerWatcher->future();
    m_picture->setPixmap(f.result());
}

bool WallpaperItem::getDeletable() const
{
    return m_deletable;
}

void WallpaperItem::setDeletable(bool deletable)
{
    m_deletable = deletable;
}

void WallpaperItem::setPath(const QString &path)
{
    m_path = path;
}
