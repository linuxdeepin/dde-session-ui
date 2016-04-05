#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

class QGSettings;
class WallpaperList;
class WallpaperListView;
class AppearanceDaemonInterface;
class DeepinWM;
class Frame : public QFrame
{
    Q_OBJECT

public:
    Frame(QFrame *parent = 0);
    ~Frame();

protected:
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    WallpaperList *m_wallpaperList = NULL;
    AppearanceDaemonInterface * m_dbusAppearance = NULL;
    DeepinWM * m_dbusDeepinWM = NULL;
    QGSettings * m_gsettings = NULL;

    QString m_formerWallpaper;

    void initSize();
    void initListView();
    QStringList processListReply(QString &reply) const;
    void restoreWallpaper();
};

#endif // FRAME_H
