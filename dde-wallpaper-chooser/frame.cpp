#include "frame.h"
#include "constants.h"
#include "wallpaperlist.h"
#include "dbus/appearancedaemon_interface.h"
#include "dbus/deepin_wm.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QGSettings>

Frame::Frame(QFrame *parent)
    : QFrame(parent),
      m_wallpaperList(new WallpaperList(this)),
      m_dbusAppearance(new AppearanceDaemonInterface(AppearanceServ,
                                                     AppearancePath,
                                                     QDBusConnection::sessionBus(),
                                                     this)),
      m_dbusDeepinWM(new DeepinWM(DeepinWMServ, DeepinWMPath, QDBusConnection::sessionBus(), this)),
      m_gsettings(new QGSettings(WallpaperSchemaId, WallpaperPath, this))
{
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    initSize();
    initListView();

    m_formerWallpaper = m_gsettings->get(WallpaperKey).toString();
}

Frame::~Frame()
{
    restoreWallpaper();
    m_dbusDeepinWM->CancelHideWindows();
}

void Frame::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);

    painter.fillRect(rect(), QColor::fromRgbF(0, 0, 0, 0.6));

    painter.end();
}

void Frame::showEvent(QShowEvent * event)
{
    m_dbusDeepinWM->RequestHideWindows();

    activateWindow();

    QFrame::showEvent(event);
}

void Frame::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Escape) {
        qApp->quit();
    }

    QFrame::keyPressEvent(event);
}

void Frame::initSize()
{
    QDesktopWidget * desktop = QApplication::desktop();
    QRect primaryRect = desktop->screen(desktop->primaryScreen())->rect();

    setFixedSize(primaryRect.width(), FrameHeight);
    move(primaryRect.x(), primaryRect.y() + primaryRect.height() - FrameHeight);
    m_wallpaperList->setFixedSize(primaryRect.width(), ListHeight);
    m_wallpaperList->move(0, (FrameHeight - ListHeight) / 2);
}

void Frame::initListView()
{
    QDBusPendingReply<QString> reply = m_dbusAppearance->List("background");
    reply.waitForFinished();

    QString value = reply.value();
    QStringList strings = processListReply(value);

    foreach (QString path, strings) {
        m_wallpaperList->addWallpaper(path);
    }

    m_wallpaperList->setStyleSheet("QListWidget { background: transparent }");

    connect(m_wallpaperList, &WallpaperList::wallpaperSet, [this] (QString path) {
        m_formerWallpaper = path;
    });
}

QStringList Frame::processListReply(QString &reply) const
{
    QStringList result;

    QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8());
    if (doc.isArray()) {
        QJsonArray arr = doc.array();
        foreach (QJsonValue val, arr) {
            QJsonObject obj = val.toObject();
            result.append(obj["Id"].toString());
        }
    }

    return result;
}

void Frame::restoreWallpaper()
{
    m_dbusAppearance->Set("background", m_formerWallpaper);
}
