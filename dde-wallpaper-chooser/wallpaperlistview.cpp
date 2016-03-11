#include "wallpaperlistview.h"

#include <QDebug>
#include <QWheelEvent>
#include <QScrollBar>

WallpaperListView::WallpaperListView(QWidget * parent) :
    QListView(parent)
{
    setFlow(QListView::LeftToRight);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWrapping(false);
    setSelectionRectVisible(false);
    setEditTriggers(QListView::NoEditTriggers);
}

void WallpaperListView::wheelEvent(QWheelEvent * event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    if (numDegrees.x()) {
        scroll(numDegrees.x(), 0);
    } else if (numDegrees.y()){
        scroll(numDegrees.y(), 0);
    }

    event->accept();
}
