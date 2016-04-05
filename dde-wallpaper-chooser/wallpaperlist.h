#ifndef WALLPAPERLIST_H
#define WALLPAPERLIST_H

#include <QListWidget>

class WallpaperItem;
class WallpaperList : public QListWidget
{
public:
    explicit WallpaperList(QWidget * parent = 0);

    void addWallpaper(const QString &path);

protected:
    void wheelEvent(QWheelEvent *);

private slots:
    void wallpaperItemPressed(WallpaperItem *);
};

#endif // WALLPAPERLIST_H
