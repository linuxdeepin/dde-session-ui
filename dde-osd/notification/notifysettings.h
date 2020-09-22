/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei@uniontech.com>
 *
 * Maintainer: chenwei <chenwei@uniontech.com>
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

#ifndef NOTIFYSETTINGS_H
#define NOTIFYSETTINGS_H

#include <com_deepin_dde_daemon_launcherd.h>
#include "types/launcheriteminfo.h"
#include "types/launcheriteminfolist.h"

#include <QObject>

class QGSettings;
class QTimer;

using LauncherInter = com::deepin::dde::daemon::Launcher;

class NotifySettings : public QObject
{
    Q_OBJECT
public:
    explicit NotifySettings(QObject *parent = nullptr);

signals:
    void settingChanged(QString key);

public slots:

public:
    void initAllSettings();

    void setAppSetting(QString settings);
    QString getAppSetings(QString appName);

    void setSystemSetting(QString settings);
    QString getSystemSetings();

    void setAllSetting(QString settings);
    QString getAllSetings();

    void appAdded(LauncherItemInfo info);
    void appRemoved(QString appName);
private:
    void setSetings(QString settings);
    QString getSetings(QString key);

private:
    QGSettings *m_settings = nullptr;
    LauncherInter *m_launcherInter = nullptr;
    QTimer *m_initTimer;
};

#endif // NOTIFYSETTINGS_H
