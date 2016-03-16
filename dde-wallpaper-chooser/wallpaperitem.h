#ifndef WALLPAPERITEM_H
#define WALLPAPERITEM_H

#include <QFrame>

class QLabel;
class Button;
class WallpaperItem : public QFrame
{
    Q_OBJECT
public:
    WallpaperItem(QFrame *parent = 0, const QString &path = "");

    bool eventFilter(QObject *, QEvent *);

signals:
    void slideUp();
    void slideDown();

private:
    QString m_path;
    QFrame * m_wrapper = NULL;
    QLabel * m_picture = NULL;
    Button * m_desktopButton = NULL;
    Button * m_desktopLockButton = NULL;

    void initUI();
    void initAnimation();
    void initPixmap();
};

#endif // WALLPAPERITEM_H
