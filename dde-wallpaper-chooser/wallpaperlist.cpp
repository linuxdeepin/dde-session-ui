#include "wallpaperlist.h"
#include "wallpaperitem.h"
#include "constants.h"
#include "dbus/appearancedaemon_interface.h"

#include <QDebug>
#include <QScrollBar>
#include <QWheelEvent>

WallpaperList::WallpaperList(QWidget * parent) :
    QListWidget(parent),
    m_dbusAppearance(new AppearanceDaemonInterface(AppearanceServ,
                                                   AppearancePath,
                                                   QDBusConnection::sessionBus(),
                                                   this))
{
    setViewMode(QListView::IconMode);
    setGridSize(QSize(ItemCellWidth, ItemCellHeight));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWrapping(false);
    setSelectionRectVisible(false);
    setEditTriggers(QListView::NoEditTriggers);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(this->horizontalScrollBar(), &QScrollBar::valueChanged, [this] {
        // hide the delete button.
        emit needCloseButton("", QPoint(0, 0));
    });
}

WallpaperList::~WallpaperList()
{

}

WallpaperItem * WallpaperList::addWallpaper(const QString &path)
{
    QListWidgetItem * item = new QListWidgetItem(this);
    item->setSizeHint(QSize(ItemWidth, ItemHeight));
    addItem(item);

    WallpaperItem * wallpaper = new WallpaperItem(this, path);
    wallpaper->setFixedSize(item->sizeHint());
    setItemWidget(item, wallpaper);

    connect(wallpaper, &WallpaperItem::pressed, this, &WallpaperList::wallpaperItemPressed);
    connect(wallpaper, &WallpaperItem::hoverIn, this, &WallpaperList::wallpaperItemHoverIn);
    connect(wallpaper, &WallpaperItem::hoverOut, this, &WallpaperList::wallpaperItemHoverOut);
    connect(wallpaper, &WallpaperItem::desktopButtonClicked, this, &WallpaperList::handleSetDesktop);
    connect(wallpaper, &WallpaperItem::desktopLockButtonClicked, this, &WallpaperList::handleSetDesktopLock);

    return wallpaper;
}

void WallpaperList::removeWallpaper(const QString &path)
{
    for (int i = 0; i < count(); i++) {
        QListWidgetItem * ii = this->item(i);
        WallpaperItem * wallpaper = qobject_cast<WallpaperItem*>(itemWidget(ii));

        if (wallpaper) {
            if (wallpaper->getPath() == path) {
                removeItemWidget(ii);
                takeItem(i);
            }
        }
    }
}

void WallpaperList::wheelEvent(QWheelEvent * event)
{
    QPoint numDegrees = event->angleDelta();

    if (numDegrees.x()) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - numDegrees.x());
    } else if (numDegrees.y()){
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - numDegrees.y());
    }

    event->accept();
}

void WallpaperList::wallpaperItemPressed()
{
    WallpaperItem * item = qobject_cast<WallpaperItem*>(sender());

    for (int i = 0; i < count(); i++) {
        QListWidgetItem * ii = this->item(i);
        WallpaperItem * wallpaper = qobject_cast<WallpaperItem*>(itemWidget(ii));

        if (wallpaper) {
            if (wallpaper == item) {
                wallpaper->slideUp();

                setWallpaper(wallpaper->getPath());
            } else {
                wallpaper->slideDown();
            }
        }
    }
}

void WallpaperList::wallpaperItemHoverIn()
{
    WallpaperItem * item = qobject_cast<WallpaperItem*>(sender());

    if (item->getDeletable()) {
        emit needCloseButton(item->getPath(), item->geometry().topRight());
    } else {
        emit needCloseButton("", QPoint(0, 0));
    }
}

void WallpaperList::wallpaperItemHoverOut()
{
//    emit needCloseButton("", QPoint(0, 0));
}

void WallpaperList::handleSetDesktop()
{
    WallpaperItem * item = qobject_cast<WallpaperItem*>(sender());
    emit wallpaperSet(item->getPath());

    qDebug() << "desktop item set, quit";
    qApp->quit();
}

void WallpaperList::handleSetDesktopLock()
{
    WallpaperItem * item = qobject_cast<WallpaperItem*>(sender());
    setLockScreen(item->getPath());
    emit wallpaperSet(item->getPath());

    qDebug() << "lock item set, quit";
    qApp->quit();
}

void WallpaperList::setWallpaper(QString realPath)
{
    m_dbusAppearance->Set("background", realPath);
}

void WallpaperList::setLockScreen(QString realPath)
{
    m_dbusAppearance->Set("greeterbackground", realPath);
}
