#ifndef WALLPAPERITEM_H
#define WALLPAPERITEM_H

#include <QFrame>
#include <QFutureWatcher>

class QLabel;
class Button;
class AppearanceDaemonInterface;
class WallpaperItem : public QFrame
{
    Q_OBJECT
public:
    WallpaperItem(QFrame *parent = 0, const QString &path = "");

    bool eventFilter(QObject *, QEvent *);

signals:
    void slideUp();
    void slideDown();

public slots:
    void thumbnailFinished();

private:
    QString m_path;
    QFrame * m_wrapper = NULL;
    QLabel * m_picture = NULL;
    Button * m_desktopButton = NULL;
    Button * m_desktopLockButton = NULL;

    QFutureWatcher<QPixmap> * m_thumbnailerWatcher = NULL;

    AppearanceDaemonInterface * m_dbusAppearance;

    void initUI();
    void initAnimation();
    void initPixmap();
    QPixmap thumbnailImage();

    void setWallpaper();
    void setLockScreen();
};

#endif // WALLPAPERITEM_H
