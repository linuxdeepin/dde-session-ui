#ifndef WALLPAPERLISTVIEW_H
#define WALLPAPERLISTVIEW_H

#include <QListView>

class WallpaperListView : public QListView
{
public:
    WallpaperListView(QWidget * parent =  0);

protected:
    void wheelEvent(QWheelEvent *);
};

#endif // WALLPAPERLISTVIEW_H
