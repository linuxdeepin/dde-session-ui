#ifndef WALLPAPERITEM_H
#define WALLPAPERITEM_H

#include <QFrame>
#include <QFutureWatcher>
#include <QPropertyAnimation>

class QLabel;
class Button;
class AppearanceDaemonInterface;
class WallpaperItem : public QFrame
{
    Q_OBJECT
public:
    WallpaperItem(QFrame *parent = 0, const QString &path = "");
    ~WallpaperItem();

    bool eventFilter(QObject *, QEvent *);

    void slideUp();
    void slideDown();

    QString getPath();
    void setPath(const QString &path);

    bool getDeletable() const;
    void setDeletable(bool deletable);

signals:
    void pressed();
    void hoverIn();
    void hoverOut();
    void desktopButtonClicked();
    void lockButtonClicked();

public slots:
    void thumbnailFinished();

private:
    QString m_path;
    bool m_deletable;

    QFrame * m_wrapper = NULL;
    QLabel * m_picture = NULL;
    Button * m_desktopButton = NULL;
    Button * m_lockButton = NULL;
    QPropertyAnimation * m_upAnim = NULL;
    QPropertyAnimation * m_downAnim = NULL;

    QFutureWatcher<QPixmap> * m_thumbnailerWatcher = NULL;

    void initUI();
    void initAnimation();
    void initPixmap();
    QPixmap thumbnailImage();
};

#endif // WALLPAPERITEM_H
