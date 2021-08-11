/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QCursor>
#include <QDebug>
#include <QWindow>
#include <QTimer>
#include "window.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
    , m_image(new QLabel(this))
    , m_text(new QLabel(tr("Low battery, please plug in"), this))
    , m_bSleepLock(false)
{
    m_image->setAccessibleName("LowPowerImage");
    m_text->setAccessibleName("LowPowerText");
    // set window flags as dde-lock, so we can easily cover it.
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    const qreal ratio = devicePixelRatioF();
    const QString iconPath = qt_findAtNxFile(":/images/lowpower.png", ratio);
    QPixmap pix(iconPath);
    pix.setDevicePixelRatio(ratio);
    m_image->setPixmap(pix);
    m_text->setStyleSheet("QLabel{ color:#ff8000; font-size:15px }");

    setupSize();
    setStyleSheet("Window { background: black }");

    QDBusConnection::sessionBus().connect("com.deepin.dde.lockFront",
                                              "/com/deepin/dde/lockFront",
                                              "com.deepin.dde.lockFront",
                                              "Visible",
                                              this, SLOT(HideShowToRaise(bool)));
}

Window::~Window()
{

}

bool Window::SleepLock()
{
    return m_bSleepLock;
}

void Window::setupSize()
{
    QDesktopWidget * desktop = QApplication::desktop();
    int screenCount = desktop->screenCount();

    int totalWidth = 0;
    int totalHeight = 0;
    for (int i = 0; i < screenCount; i++) {
        QWidget * screen = desktop->screen(i);
        totalWidth = qMax(totalWidth, screen->x() + screen->width());
        totalHeight = qMax(totalHeight, screen->y() + screen->height());
    }

    setFixedSize(totalWidth, totalHeight);

    QPoint cursorPos = QCursor::pos();
    int screenNum = desktop->screenNumber(cursorPos);
    QWidget * screen = desktop->screen(screenNum);

    const qreal ratio = screen->devicePixelRatioF();
    m_image->setFixedSize(m_image->pixmap()->size() / ratio);
    m_image->move(screen->geometry().center() - m_image->rect().center());

    m_text->setFixedSize(screen->geometry().width(), 30);
    m_text->setAlignment(Qt::AlignHCenter);
    m_text->move(0, m_image->y() + m_image->pixmap()->height());

    QDBusInterface inter2Power("com.deepin.daemon.Power",
                               "/com/deepin/daemon/Power",
                               "com.deepin.daemon.Power",
                               QDBusConnection::sessionBus(), this);
    m_bSleepLock =  inter2Power.property("SleepLock").toBool();
}

void Window::HideShowToRaise(bool visible)
{
    if (visible) {
        QTimer::singleShot(200, this, [ = ]  {
            hide();
            this->raise();
            show();
        });
    }
}

LowPowerAdaptor::LowPowerAdaptor(Window * parent) :
    QDBusAbstractAdaptor(parent)
{

}

LowPowerAdaptor::~LowPowerAdaptor()
{

}

void LowPowerAdaptor::Raise()
{
    Window * w = qobject_cast<Window*>(parent());
    if (w) {
        w->raise();
        w->activateWindow();
        w->grabMouse();
        if (!w->SleepLock()) {
           w->grabKeyboard();
        }
    }
}

void LowPowerAdaptor::Quit()
{
    qApp->quit();
}
