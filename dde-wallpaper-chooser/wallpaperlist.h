#ifndef WALLPAPERLIST_H
#define WALLPAPERLIST_H

#include <QListWidget>

class WallpaperItem;
class AppearanceDaemonInterface;
class WallpaperList : public QListWidget
{
public:
    explicit WallpaperList(QWidget * parent = 0);

    void addWallpaper(const QString &path);

protected:
    void wheelEvent(QWheelEvent *);

private:
    AppearanceDaemonInterface * m_dbusAppearance;

    void setWallpaper(QString realPath);
    void setLockScreen(QString realPath);

private slots:
    void wallpaperItemPressed();
    void handleSetDesktop();
    void handleSetDesktopLock();
};

#endif // WALLPAPERLIST_H
