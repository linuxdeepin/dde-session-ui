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

#ifndef SHUTDOWNFRAME
#define SHUTDOWNFRAME

#include <QFrame>

#include <com_deepin_wm.h>
#include <com_deepin_daemon_appearance.h>
#include "fullscreenbackground.h"
#include "view/contentwidget.h"

using Appearance = com::deepin::daemon::Appearance;

class ShutdownFrontDBus;
class ShutdownFrame: public FullscreenBackground
{
    Q_OBJECT
public:
    ShutdownFrame(QWidget* parent = 0);
    ~ShutdownFrame();

public slots:
    void powerAction(const Actions action);
    void setConfirm(const bool confrim);

private:
    com::deepin::wm *m_wmInter;
    Appearance *m_dbusAppearance = NULL;
    void initBackground();
    ContentWidget *m_shutdownFrame;
};

class ShutdownFrontDBus : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.shutdownFront")

public:
    ShutdownFrontDBus(ShutdownFrame* parent);
    ~ShutdownFrontDBus();

    Q_SLOT void Ping();
    Q_SLOT void Shutdown();
    Q_SLOT void Restart();
    Q_SLOT void Logout();
    Q_SLOT void Suspend();
    Q_SLOT void SwitchUser();
    Q_SLOT void Show();

private:
    ShutdownFrame* m_parent;
};
#endif // SHUTDOWNFRAME

