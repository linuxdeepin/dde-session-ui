/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
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

#include "container.h"

#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QGSettings>
#include <DPlatformWindowHandle>
#include <com_deepin_daemon_display.h>
#include <com_deepin_daemon_display_monitor.h>

using DisplayInter = com::deepin::daemon::Display;
using MonitorInter = com::deepin::daemon::display::Monitor;

DGUI_USE_NAMESPACE

Container::Container(QWidget *parent)
    : DBlurEffectWidget(parent)
    , m_wmHelper(DWindowManagerHelper::instance())
    , m_quitTimer(new QTimer(this))
    , m_supportComposite(m_wmHelper->hasComposite())
{
    setWindowFlags(Qt::ToolTip | Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus);
    setAttribute(Qt::WA_TranslucentBackground);

    m_quitTimer->setSingleShot(true);
    m_quitTimer->setInterval(60 * 1000);

    m_layout = new QHBoxLayout;
    m_layout->setSpacing(0);
    m_layout->setMargin(0);
    setLayout(m_layout);

    const int radius = getWindowRadius();

    DPlatformWindowHandle handle(this);
    handle.setBorderColor(QColor(0, 0, 0, 0.04 * 255));
    handle.setShadowColor(Qt::transparent);
    handle.setTranslucentBackground(true);
    handle.setWindowRadius(radius);

    setBlurRectXRadius(radius);
    setBlurRectYRadius(radius);
    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setMaskColor(DBlurEffectWidget::AutoColor);

    connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged,
            this, &Container::windowManagerChanged);

    connect(m_quitTimer, &QTimer::timeout, this, &Container::onDelayQuit);
}

void Container::setContent(QWidget *content)
{
    m_layout->addWidget(content);
}

void Container::moveToCenter()
{
    QDesktopWidget *desktop = QApplication::desktop();
    const int primary = desktop->primaryScreen();

    QRect displayRect;
    DisplayInter *displayInter = new DisplayInter("com.deepin.daemon.Display", "/com/deepin/daemon/Display", QDBusConnection::sessionBus(), this);
    QList<QDBusObjectPath> screenList = displayInter->monitors();
    for (auto screen : screenList) {
        MonitorInter *monitor = new MonitorInter("com.deepin.daemon.Display", screen.path(), QDBusConnection::sessionBus());
        if (monitor->enabled()) {
            qDebug() << " screen display : " << screen.path();
            displayRect = QRect(monitor->x(), monitor->y(), monitor->width(), monitor->height());
            break;
        }
    }

    if (!displayRect.isValid() || displayRect.isEmpty()) {
        displayRect = desktop->screenGeometry(primary);
    }

    move(QPoint(displayRect.center().x(), displayRect.bottom() - 180) - QPoint(rect().center().x(), rect().bottom()));
}

void Container::showEvent(QShowEvent *event)
{
    DBlurEffectWidget::showEvent(event);

    m_quitTimer->stop();
}

void Container::hideEvent(QHideEvent *event)
{
    DBlurEffectWidget::hideEvent(event);

    m_quitTimer->start();
}

void Container::windowManagerChanged()
{
    m_supportComposite = m_wmHelper->hasComposite();

    updateWindowRadius();
}

void Container::updateWindowRadius()
{
    const int value = getWindowRadius();

    DPlatformWindowHandle handle(this);
    handle.setWindowRadius(value);

    setBlurRectXRadius(value);
    setBlurRectYRadius(value);
}

int Container::getWindowRadius()
{
    return m_supportComposite ? 10 : 0;
}

void Container::onDelayQuit()
{
    const QGSettings gsettings("com.deepin.dde.osd", "/com/deepin/dde/osd/");
    if (gsettings.keys().contains("autoExit") && gsettings.get("auto-exit").toBool()) {
        if (isVisible())
            return m_quitTimer->start();
        qWarning() << "Killer Timeout, now quiiting...";
        qApp->quit();
    }
}
