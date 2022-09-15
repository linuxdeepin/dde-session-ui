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

#include "org_deepin_dde_daemon_launcherd1.h"
#include "types/launcheriteminfo.h"
#include "types/launcheriteminfolist.h"

#include <QObject>

class QGSettings;
class QTimer;

using LauncherInter = org::deepin::dde::daemon::Launcher1;

class AbstractNotifySetting : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        APPNAME,
        APPICON,
        ENABELNOTIFICATION,
        ENABELPREVIEW,
        ENABELSOUND,
        SHOWINNOTIFICATIONCENTER,
        LOCKSCREENSHOWNOTIFICATION,
        SHOWONTOP
    } AppConfigurationItem;

    typedef enum {
        DNDMODE,
        LOCKSCREENOPENDNDMODE,
        OPENBYTIMEINTERVAL,
        STARTTIME,
        ENDTIME,
        SHOWICON
    } SystemConfigurationItem;

    explicit AbstractNotifySetting(QObject *parent = nullptr)
        : QObject(parent)
    {}
    virtual void initAllSettings() = 0;
    virtual void setAppSetting(const QString &id, const AppConfigurationItem &item, const QVariant &var) = 0;
    virtual QVariant getAppSetting(const QString &id, const AppConfigurationItem &item) = 0;
    virtual void setSystemSetting(const SystemConfigurationItem &item, const QVariant &var) = 0;
    virtual QVariant getSystemSetting(const SystemConfigurationItem &item) = 0;
    virtual QStringList getAppLists() = 0;
    virtual void appAdded(const LauncherItemInfo &info) = 0;
    virtual void appRemoved(const QString &id) = 0;

    // 兼容之前版本的接口，后期移除
    virtual void setAppSetting_v1(QString settings) = 0;
    virtual QString getAppSettings_v1(const QString &id) = 0;
    virtual void setSystemSetting_v1(QString settings) = 0;
    virtual QString getSystemSetings_v1() = 0;
    virtual void setAllSetting_v1(QString settings) = 0;
    virtual QString getAllSetings_v1() = 0;

signals:
    void appAddedSignal(const QString &id);
    void appRemovedSignal(const QString &id);
    void appSettingChanged(const QString &id, const uint &item, QVariant var);
    void systemSettingChanged(const uint &item, QVariant var);
};

class NotifySettings : public AbstractNotifySetting
{
    Q_OBJECT
public:
    explicit NotifySettings(QObject *parent = nullptr);
    void initAllSettings() override;
    void setAppSetting(const QString &id, const AppConfigurationItem &item, const QVariant &var) override;
    QVariant getAppSetting(const QString &id, const AppConfigurationItem &item) override;
    void setSystemSetting(const SystemConfigurationItem &item, const QVariant &var) override;
    QVariant getSystemSetting(const SystemConfigurationItem &item) override;
    QStringList getAppLists() override;
    void appAdded(const LauncherItemInfo &info) override;
    void appRemoved(const QString &id) override;

    // 兼容之前版本的接口，后期移除
    void setAppSetting_v1(QString settings) override;
    QString getAppSettings_v1(const QString &id) override;
    void setSystemSetting_v1(QString settings) override;
    QString getSystemSetings_v1() override;
    void setAllSetting_v1(QString settings) override;
    QString getAllSetings_v1() override;

private:
    bool containsAppSettings(const QGSettings &settings, const QString &id);

    QTimer *m_initTimer;
    QGSettings *m_systemSetting;
    LauncherInter *m_launcherInter;
};

#endif // NOTIFYSETTINGS_H
