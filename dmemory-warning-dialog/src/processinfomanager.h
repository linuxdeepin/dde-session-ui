/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
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

#ifndef PROCESSINFOMANAGER_H
#define PROCESSINFOMANAGER_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <QPointer>

#include "org_deepin_dde_startmanager1.h"
#include "com_deepin_chromeextension_tabslimit.h"

QString genericAppName(const QString &desktop);

using StartManagerInter = org::deepin::dde::StartManager1;
using ChromeTabsInter = com::deepin::chromeextension::TabsLimit;

class ProcessInfo
{
public:
    int id;
    unsigned total_mem_bytes;
    QString cgroup_path;
    QString app_name;
    QString desktop;
    QStringList pid_list;
    QList<ProcessInfo> sub_procs;
};

class ProcessInfoModel;
class ProcessInfoManager : public QObject
{
    Q_OBJECT

    friend class ProcessInfoModel;

public:
    explicit ProcessInfoManager(QObject *parent = nullptr);

signals:
    void processInfoListChanged() const;

private slots:
    void scanChromeTabs();
    void scanProcessInfos();
    void startRefreshData();
    void stopRefreshData() { m_refreshTimer->stop(); }
    void mergeLists();

    void scanChromeTabsCB(QDBusPendingCallWatcher *watcher);

private:
    void appendCGroupPath(const QString &path, const QString &desktop);
//    void appendChromeTab(const ChromeTabInfo &tabInfo);

private:
    QPointer<QTimer> m_refreshTimer;
    QPointer<StartManagerInter> m_startManagerInter;
    QPointer<ChromeTabsInter> m_chromeTabsInter;
    QString m_sessionId;
    QList<ProcessInfo> tabsInfoList;
    QList<ProcessInfo> processInfoList;
};

#endif // PROCESSINFOMANAGER_H
