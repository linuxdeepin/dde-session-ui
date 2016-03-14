#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

class WallpaperList;
class WallpaperListView;
class AppearanceDaemonInterface;
class Frame : public QFrame
{
    Q_OBJECT

public:
    Frame(QFrame *parent = 0);
    ~Frame();

protected:
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    WallpaperList *m_wallpaperList;
    AppearanceDaemonInterface * m_dbusAppearance;

    void initSize();
    void initListView();
    QStringList processListReply(QString &reply) const;
};

#endif // FRAME_H
