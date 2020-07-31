/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
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

#include "suspenddialog.h"

#include <DApplication>

#include <QScreen>
#include <QDebug>
#include <QAbstractButton>
#include <com_deepin_daemon_display.h>
#include <com_deepin_daemon_display_monitor.h>

using MonitorInter = com::deepin::daemon::display::Monitor;

SuspendDialog::SuspendDialog(QRect screenGeometry)
    : DDialog(tr("External monitor detected, suspend?"), tr("%1s").arg(15)),
      m_screenGeometry(screenGeometry),
      m_timerTick(0)
{
    const auto ratio = devicePixelRatioF();
    QPixmap icon = QIcon::fromTheme("computer").pixmap(QSize(48, 48) * ratio);
    icon.setDevicePixelRatio(ratio);

    setIconPixmap(icon);

    QStringList buttons;
    buttons << tr("Cancel") << tr("Suspend");
    addButtons(buttons);
    getButton(0)->setFocus();

    m_timer.setInterval(1000);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        m_timerTick++;
        if (m_timerTick >= 15) {
            done(1);
        } else {
            setMessage(tr("%1s").arg(15 - m_timerTick));
        }
    });

    m_timer.start();
}

void SuspendDialog::showEvent(QShowEvent *event)
{
    DDialog::showEvent(event);

    const auto ratio = devicePixelRatioF();
    move(m_screenGeometry.center() / ratio - rect().center());
}

Manager::Manager()
{
    setupDialogs();
}

void Manager::setupDialogs()
{
    com::deepin::daemon::Display *displayInter = new com::deepin::daemon::Display("com.deepin.daemon.Display", "/com/deepin/daemon/Display", QDBusConnection::sessionBus(), this);
    QList<QDBusObjectPath> screenList = displayInter->monitors();
    for (int i = 0; i < screenList.length(); i++) {
        MonitorInter *monitor = new MonitorInter("com.deepin.daemon.Display", screenList[i].path(), QDBusConnection::sessionBus());
        if (monitor->name().toLower().contains("edp"))
            continue;

        const QRect geom = QRect(monitor->x(), monitor->y(), monitor->width(), monitor->height());
        SuspendDialog *dialog = new SuspendDialog(geom);
        dialog->show();

        connect(dialog, &SuspendDialog::finished, this, &Manager::finished);
    }
}
