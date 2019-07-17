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

#include <QDebug>

#include "shutdownframe.h"
#include "sessionbasemodel.h"
#include "dbusshutdownagent.h"

const QString WallpaperKey = "pictureUri";

ShutdownFrame::ShutdownFrame(SessionBaseModel * const model, QWidget *parent)
    : FullscreenBackground(parent)
    , m_model(model)
{
    m_shutdownFrame = new ContentWidget(this);
    m_shutdownFrame->setModel(model);
    setContent(m_shutdownFrame);
    m_shutdownFrame->hide();

    connect(m_shutdownFrame, &ContentWidget::requestBackground,
            this, static_cast<void (ShutdownFrame::*)(const QString &)>(&ShutdownFrame::updateBackground));


    connect(model, &SessionBaseModel::visibleChanged, this, [=] (bool visible) {
        if (visible) {
            // refresh hibernate and sleep function.
            emit model->onStatusChanged(SessionBaseModel::PowerMode);
        }
    });
}

bool ShutdownFrame::powerAction(const Actions action)
{
    return m_shutdownFrame->powerAction(action);
}

void ShutdownFrame::setConfirm(const bool confrim)
{
    m_shutdownFrame->setConfirm(confrim);
}

void ShutdownFrame::showEvent(QShowEvent *event)
{
    Q_EMIT requestEnableHotzone(false);

    m_model->setIsShow(true);

    return FullscreenBackground::showEvent(event);
}

void ShutdownFrame::hideEvent(QHideEvent *event)
{
    Q_EMIT requestEnableHotzone(true);

    m_model->setIsShow(false);

    m_shutdownFrame->recoveryLayout();

    return FullscreenBackground::hideEvent(event);
}

ShutdownFrame::~ShutdownFrame()
{
}

ShutdownFrontDBus::ShutdownFrontDBus(DBusShutdownAgent *parent):
    QDBusAbstractAdaptor(parent),
    m_parent(parent)
{
}

ShutdownFrontDBus::~ShutdownFrontDBus()
{
}

void ShutdownFrontDBus::Ping()
{
}

void ShutdownFrontDBus::Shutdown()
{
    m_parent->Shutdown();
}

void ShutdownFrontDBus::Restart()
{
    m_parent->Restart();
}

void ShutdownFrontDBus::Logout()
{
    m_parent->Logout();
}

void ShutdownFrontDBus::Suspend()
{
    m_parent->Suspend();
}

void ShutdownFrontDBus::Hibernate()
{
    m_parent->Hibernate();
}

void ShutdownFrontDBus::SwitchUser()
{
    m_parent->SwitchUser();
}

void ShutdownFrontDBus::Show()
{
    m_parent->Show();
}
