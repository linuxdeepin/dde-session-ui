#ifndef WALLPAPERITEM_H
#define WALLPAPERITEM_H

#include <QFrame>

class QLabel;
class WallpaperItem : public QFrame
{
    Q_OBJECT
public:
    WallpaperItem(QFrame *parent = 0, const QString &path = "");

private:
    QString m_path;
    QLabel * m_picture = NULL;
};

#endif // WALLPAPERITEM_H
