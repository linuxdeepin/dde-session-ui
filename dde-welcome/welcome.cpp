#include "welcome.h"

#include <QPainter>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>

Welcome::Welcome(QWidget *parent)
    : QWidget(parent),

      m_displayInter(new DisplayInter("com.deepin.daemon.Display", "/com/deepin/daemon/Display", QDBusConnection::sessionBus(), this)),

      m_sizeAdjustTimer(new QTimer(this))
{
    m_displayInter->setSync(false);

    m_sizeAdjustTimer->setInterval(100);
    m_sizeAdjustTimer->setSingleShot(true);

    setWindowFlags(Qt::X11BypassWindowManagerHint);
    qApp->setOverrideCursor(Qt::BlankCursor);

    connect(m_displayInter, &DisplayInter::ScreenHeightChanged, m_sizeAdjustTimer, static_cast<void (QTimer::*)()>(&QTimer::start));
    connect(m_sizeAdjustTimer, &QTimer::timeout, this, &Welcome::onScreenRectChanged, Qt::QueuedConnection);

    QTimer::singleShot(1, this, &Welcome::onScreenRectChanged);
}

Welcome::~Welcome()
{
    qApp->restoreOverrideCursor();
}

void Welcome::dbus_show()
{
    show();
}

void Welcome::dbus_exit()
{
    qApp->quit();
}

void Welcome::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef  QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    default:;
    }

    QWidget::keyPressEvent(e);
}

void Welcome::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);

    painter.fillRect(rect(), Qt::black);
}

void Welcome::onScreenRectChanged()
{
    const int w = m_displayInter->screenWidth();
    const int h = m_displayInter->screenHeight();

    setFixedWidth(w);
    setFixedHeight(h);
}
