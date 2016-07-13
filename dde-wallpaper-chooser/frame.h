#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <dimagebutton.h>

DWIDGET_USE_NAMESPACE

class QGSettings;
class WallpaperList;
class WallpaperListView;
class AppearanceDaemonInterface;
class DeepinWM;
class DBusXMouseArea;
class Frame : public QFrame
{
    Q_OBJECT

public:
    Frame(QFrame *parent = 0);
    ~Frame();

public slots:
    void handleNeedCloseButton(QString path, QPoint pos);

protected:
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    WallpaperList *m_wallpaperList = NULL;
    DImageButton * m_closeButton = NULL;

    AppearanceDaemonInterface * m_dbusAppearance = NULL;
    DeepinWM * m_dbusDeepinWM = NULL;
    QGSettings * m_gsettings = NULL;
    DBusXMouseArea * m_dbusMouseArea = NULL;

    QString m_formerWallpaper;
    QString m_mouseAreaKey;
    QMap<QString, bool> m_deletableInfo;

    void initSize();
    void initListView();
    QStringList processListReply(const QString &reply);
    void restoreWallpaper();
};

#endif // FRAME_H
