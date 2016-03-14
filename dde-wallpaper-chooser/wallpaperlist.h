#ifndef WALLPAPERLIST_H
#define WALLPAPERLIST_H

#include <QListWidget>

class WallpaperList : public QListWidget
{
public:
    explicit WallpaperList(QWidget * parent = 0);

    void addWallpaper(const QString &path);

protected:
    void wheelEvent(QWheelEvent *);
};

#endif // WALLPAPERLIST_H
