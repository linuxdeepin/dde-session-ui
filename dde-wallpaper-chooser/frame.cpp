#include "frame.h"
#include "constants.h"
#include "wallpaperlist.h"
#include "wallpaperitem.h"
#include "dbus/appearancedaemon_interface.h"
#include "dbus/deepin_wm.h"
#include "dbus/dbusxmousearea.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QScrollBar>

Frame::Frame(QFrame *parent)
    : QFrame(parent),
      m_wallpaperList(new WallpaperList(this)),
      m_closeButton(new DImageButton(":/images/close_normal.png",
                                 ":/images/close_hover.png",
                                 ":/images/close_press.png", this)),
      m_dbusAppearance(new AppearanceDaemonInterface(AppearanceServ,
                                                     AppearancePath,
                                                     QDBusConnection::sessionBus(),
                                                     this)),
      m_dbusDeepinWM(new DeepinWM(DeepinWMServ,
                                  DeepinWMPath,
                                  QDBusConnection::sessionBus(),
                                  this)),
      m_dbusMouseArea(new DBusXMouseArea(this))
{
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    initSize();
    initListView();

    connect(m_dbusMouseArea, &DBusXMouseArea::ButtonPress, [this](int button, int x, int y, const QString &id){
        if (id != m_mouseAreaKey) return;

        if (button == 4) {
            m_wallpaperList->prevPage();
        } else if (button == 5) {
            m_wallpaperList->nextPage();
        } else {
            qDebug() << "button pressed on blank area, quit.";

            if (!rect().contains(x - this->x(), y - this->y())) {
                qApp->quit();
            }
        }
    });

    m_closeButton->hide();
    connect(m_wallpaperList, &WallpaperList::needCloseButton, this, &Frame::handleNeedCloseButton);
}

Frame::~Frame()
{
    m_dbusDeepinWM->CancelHideWindows();
    m_dbusMouseArea->UnregisterArea(m_mouseAreaKey);
}

void Frame::handleNeedCloseButton(QString path, QPoint pos)
{
    if (!path.isEmpty()) {
        m_closeButton->move(pos.x() - 10, pos.y() + 10);
        m_closeButton->show();
        m_closeButton->disconnect();

        connect(m_closeButton, &DImageButton::clicked, [this, path] {
            m_closeButton->hide();
            m_dbusAppearance->Delete("background", path);
            m_wallpaperList->removeWallpaper(path);
        });
    } else {
        m_closeButton->hide();
    }
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
    m_mouseAreaKey = m_dbusMouseArea->RegisterFullScreen();

    activateWindow();

    QFrame::showEvent(event);
}

void Frame::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "escape key pressed, quit.";
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
        WallpaperItem * item = m_wallpaperList->addWallpaper(path);
        item->setDeletable(m_deletableInfo.value(path));
    }

    m_wallpaperList->setStyleSheet("QListWidget { background: transparent }");
}

QStringList Frame::processListReply(const QString &reply)
{
    QStringList result;

    QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8());
    if (doc.isArray()) {
        QJsonArray arr = doc.array();
        foreach (QJsonValue val, arr) {
            QJsonObject obj = val.toObject();
            QString id = obj["Id"].toString();
            result.append(id);
            m_deletableInfo[id] = obj["Deletable"].toBool();
        }
    }

    return result;
}
