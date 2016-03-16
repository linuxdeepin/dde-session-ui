#include "frame.h"
#include "constants.h"
#include "wallpaperlist.h"
#include "dbus/appearancedaemon_interface.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>

static const QString AppearanceServ = "com.deepin.daemon.Appearance";
static const QString AppearancePath = "/com/deepin/daemon/Appearance";

Frame::Frame(QFrame *parent)
    : QFrame(parent),
      m_wallpaperList(new WallpaperList(this)),
      m_dbusAppearance(new AppearanceDaemonInterface(AppearanceServ,
                                                     AppearancePath,
                                                     QDBusConnection::sessionBus(),
                                                     this))
{
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    initSize();
    initListView();
}

Frame::~Frame()
{

}

void Frame::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);

    painter.fillRect(rect(), QColor::fromRgbF(0, 0, 0, 0.8));

    painter.end();
}

void Frame::showEvent(QShowEvent * event)
{
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
