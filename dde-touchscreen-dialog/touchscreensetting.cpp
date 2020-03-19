/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author: quezhiyong <quezhiyong@uniontech.com>
 *         wangwei <wangwei@uniontech.com>
 *
 * Maintainer: quezhiyong <quezhiyong@uniontech.com>
 *             wangwei <wangwei@uniontech.com>
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

#include "touchscreensetting.h"

#include "recognizedialog.h"

#include <com_deepin_daemon_display_monitor.h>

#include <QGuiApplication>
#include <QScreen>
#include <QTimer>

DCORE_USE_NAMESPACE

const int Y_OFFSET = 200;

TouchscreenSetting::TouchscreenSetting(const QString &touchscreen, QWidget *parent)
    : DDialog(parent)
    , m_displayInter(
              new Display("com.deepin.daemon.Display", "/com/deepin/daemon/Display", QDBusConnection::sessionBus()))
    , m_recognize(new RecognizeDialog(m_displayInter, this))
    , m_touchscreenSerial(touchscreen)
    , m_listCombo(new DComboBox)
{
    setTitle(tr("Select your touch screen"));
    setIcon(QIcon::fromTheme("dcc_touchscreen"));
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    addContent(m_listCombo);

    addButton(tr("Cancel"));
    addButton(tr("Confirm"), false, ButtonRecommend);

    connect(m_displayInter, &Display::MonitorsChanged, this, &TouchscreenSetting::onMonitorChanged);
    connect(this, &DDialog::buttonClicked, this, &TouchscreenSetting::onButtonClicked);

    onMonitorChanged();

    // 移动位置避免被屏幕检测覆盖
    QTimer::singleShot(0, this, [this]() {
        auto primaryRect = QGuiApplication::primaryScreen()->geometry();
        move(primaryRect.center().x() - width() / 2, Y_OFFSET);
    });

    m_recognize->show();
}

void TouchscreenSetting::onMonitorChanged()
{
    m_listCombo->clear();

    auto monitorPathList = m_displayInter->monitors();
    for (const QDBusObjectPath &m : monitorPathList) {
        display::Monitor monitor("com.deepin.daemon.Display", m.path(), QDBusConnection::sessionBus());

        m_listCombo->addItem(monitor.name());
    }
}

void TouchscreenSetting::onButtonClicked(int index, const QString &)
{
    if (index != 1) {
        close();
        return;
    }

    qDebug() << "confirm clicked";

    QString monitor = m_listCombo->currentText();
    Q_EMIT requestAssociateTouch(m_touchscreenSerial, monitor);

    close();
}
