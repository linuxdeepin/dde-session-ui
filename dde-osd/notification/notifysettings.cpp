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

#include "notifysettings.h"
#include "constants.h"

#include <QGSettings>
#include <QTimer>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <DDesktopEntry>

DCORE_USE_NAMESPACE

const QString server = "com.deepin.dde.daemon.Launcher";
const QString path = "/com/deepin/dde/daemon/Launcher";

NotifySettings::NotifySettings(QObject *parent)
    : QObject(parent)
    , m_launcherInter(new LauncherInter(server, path, QDBusConnection::sessionBus(), this))
    , m_initTimer(new QTimer(this))
{
    registerLauncherItemInfoListMetaType();
    registerLauncherItemInfoMetaType();

    m_initTimer->setInterval(1000);
    if (QGSettings::isSchemaInstalled("com.deepin.dde.notification")) {
        m_settings = new QGSettings("com.deepin.dde.notification", "/com/deepin/dde/notification/", this);
        m_initTimer->start();
        connect(m_settings, &QGSettings::changed, this, &NotifySettings::settingChanged);
        connect(m_initTimer, &QTimer::timeout, this, &NotifySettings::initAllSettings);
    }
}

void NotifySettings::initAllSettings()
{
    if (m_settings == nullptr)
        return;
    QJsonObject obj = QJsonDocument::fromJson(m_settings->get("notifycations-settings").toString().toUtf8()).object();
    QStringList settingList = obj.keys();
    // 初始化系统设置
    if (!settingList.contains(SystemNotifySettingStr)) {
        QJsonObject systemObj;
        systemObj.insert(DoNotDisturbStr, DEFAULT_DO_NOT_DISTURB);
        systemObj.insert(TimeSlotStr, DEFAULT_TIME_SLOT);
        systemObj.insert(StartTimeStr, DefaultStartTime);
        systemObj.insert(EndTimeStr, DefaultEndTime);
        systemObj.insert(AppsInFullscreenStr, DEFAULT_APP_IN_FULLSCREEN);
        systemObj.insert(ConnectedProjectorStr, DEFAULT_CONNECTED_PROJECTOR);
        systemObj.insert(ScreenLockedStr, DEFAULT_SCREEN_LOCKED);
        systemObj.insert(ShowIconOnDockStr, DEFAULT_SHOW_ICON_ON_DOCK);
        obj.insert(SystemNotifySettingStr, systemObj);
    } else {
        QJsonObject systemObj = obj[SystemNotifySettingStr].toObject();
        if (!systemObj.contains(DoNotDisturbStr)) {
            systemObj.insert(DoNotDisturbStr, DEFAULT_DO_NOT_DISTURB);
        }
        if (!systemObj.contains(TimeSlotStr)) {
            systemObj.insert(TimeSlotStr, DEFAULT_TIME_SLOT);
        }
        if (!systemObj.contains(StartTimeStr)) {
            systemObj.insert(StartTimeStr, DefaultStartTime);
        }
        if (!systemObj.contains(EndTimeStr)) {
            systemObj.insert(EndTimeStr, DefaultEndTime);
        }
        if (!systemObj.contains(AppsInFullscreenStr)) {
            systemObj.insert(AppsInFullscreenStr, DEFAULT_APP_IN_FULLSCREEN);
        }
        if (!systemObj.contains(TimeSlotStr)) {
            systemObj.insert(TimeSlotStr, DEFAULT_TIME_SLOT);
        }
        if (!systemObj.contains(ConnectedProjectorStr)) {
            systemObj.insert(ConnectedProjectorStr, DEFAULT_CONNECTED_PROJECTOR);
        }
        if (!systemObj.contains(ScreenLockedStr)) {
            systemObj.insert(ScreenLockedStr, DEFAULT_SCREEN_LOCKED);
        }
        if (!systemObj.contains(ShowIconOnDockStr)) {
            systemObj.insert(ShowIconOnDockStr, DEFAULT_SHOW_ICON_ON_DOCK);
        }
        obj[SystemNotifySettingStr] = systemObj;
    }

    LauncherItemInfoList appList = m_launcherInter->GetAllItemInfos();
    if (appList.size() > 0) {
        m_initTimer->stop();
    }

    for (int i = 0; i < appList.size(); i++) {
        DDesktopEntry appDeskTopInfo(appList[i].Path);
        // 判断是否为白名单应用或者WINE应用，这两种类型的应用通知不做处理
        if (WhiteBoardAppList.contains(appList[i].ID) || appDeskTopInfo.rawValue("X-Created-By") == "Deepin WINE Team") {
            // 移除之前添加的WINE应用的通知设置
            obj.remove(appList[i].ID);
            continue;
        }

        if (!settingList.contains(appList[i].ID)) {
            QJsonObject appObj;
            appObj.insert(AppIconStr, appList[i].Icon);
            appObj.insert(AppNameStr, appList[i].Name);
            appObj.insert(AllowNotifyStr, DEFAULT_ALLOW_NOTIFY);
            appObj.insert(ShowInNotifyCenterStr, DEFAULT_ONLY_IN_NOTIFY);
            appObj.insert(LockShowNotifyStr, DEFAULT_LOCK_SHOW_NOTIFY);
            appObj.insert(ShowNotifyPreviewStr, DEFAULT_SHOW_NOTIFY_PREVIEW);
            appObj.insert(NotificationSoundStr, DEFAULT_NOTIFY_SOUND);
            obj.insert(appList[i].ID, appObj);
        } else {
            QJsonObject appObj;
            appObj = obj[appList[i].ID].toObject();
            appObj[AppNameStr] = appList[i].Name;
            obj[appList[i].ID] = appObj;
        }
    }

    m_settings->set("notifycations-settings", QString(QJsonDocument(obj).toJson()));
}

void NotifySettings::setSetings(QString settings)
{
    if (m_settings == nullptr)
        return;
    QString allSettings = m_settings->get("notifycations-settings").toString();
    QJsonObject allSettingsObj = QJsonDocument::fromJson(allSettings.toUtf8()).object();
    QJsonObject currentObj = QJsonDocument::fromJson(settings.toUtf8()).object();

    QString key = currentObj.begin().key();
    QJsonObject currentSettingObj = currentObj[key].toObject();
    QStringList currentSettingList = currentSettingObj.keys();

    QJsonObject itemObj = allSettingsObj[key].toObject();
    if (currentSettingList.isEmpty())
        return;
    if (allSettingsObj.contains(key)) {
        for (int i = 0; i < currentSettingList.size(); i++) {
            if (itemObj.contains(currentSettingList[i])) {
                itemObj[currentSettingList[i]] = currentSettingObj[currentSettingList[i]];
            } else {
                itemObj.insert(currentSettingList[i], currentSettingObj[currentSettingList[i]]);
            }
        }
        allSettingsObj[key] = itemObj;
    } else {
        allSettingsObj.insert(key, currentSettingObj);
    }
    m_settings->set("notifycations-settings", QString(QJsonDocument(allSettingsObj).toJson()));
}

QString NotifySettings::getSetings(QString key)
{
    if (m_settings == nullptr)
        return QString();

    QString allSettings = m_settings->get("notifycations-settings").toString();
    QJsonObject allSettingsObj = QJsonDocument::fromJson(allSettings.toUtf8()).object();
    if (!allSettingsObj.isEmpty()) {
        QJsonObject appObj;
        appObj[key] = allSettingsObj[key].toObject();
        return QString(QJsonDocument(appObj).toJson());
    }

    return QString();
}

void NotifySettings::setAppSetting(QString settings)
{
    setSetings(settings);
}

QString NotifySettings::getAppSetings(QString appName)
{
    return getSetings(appName);
}

void NotifySettings::setSystemSetting(QString settings)
{
    setSetings(settings);
}

QString NotifySettings::getSystemSetings()
{
    return getSetings(SystemNotifySettingStr);
}

void NotifySettings::setAllSetting(QString settings)
{
    m_settings->set("notifycations-settings", settings);
}

QString NotifySettings::getAllSetings()
{
    return m_settings->get("notifycations-settings").toString();
}

void NotifySettings::setNotificationClosed(bool disable)
{
    m_settings->set("notification-closed",disable);
}

bool NotifySettings::getNotificationClosed()
{
    return m_settings->get("notification-closed").toBool();
}

void NotifySettings::appAdded(LauncherItemInfo info)
{
    if (m_settings == nullptr)
        return;

    DDesktopEntry appDeskTopInfo(info.Path);
    // 判断是否为白名单应用或者WINE应用，这两种类型的应用通知不做处理
    if (WhiteBoardAppList.contains(info.ID) || appDeskTopInfo.rawValue("X-Created-By") == "Deepin WINE Team")
        return;

    QJsonObject obj = QJsonDocument::fromJson(m_settings->get("notifycations-settings").toString().toUtf8()).object();
    QJsonObject appObj;
    appObj.insert(AppIconStr, info.Icon);
    appObj.insert(AppNameStr, info.Name);
    appObj.insert(AllowNotifyStr, DEFAULT_ALLOW_NOTIFY);
    appObj.insert(ShowInNotifyCenterStr, DEFAULT_ONLY_IN_NOTIFY);
    appObj.insert(LockShowNotifyStr, DEFAULT_LOCK_SHOW_NOTIFY);
    appObj.insert(ShowNotifyPreviewStr, DEFAULT_SHOW_NOTIFY_PREVIEW);
    appObj.insert(NotificationSoundStr, DEFAULT_NOTIFY_SOUND);
    obj.insert(info.ID, appObj);

    m_settings->set("notifycations-settings", QString(QJsonDocument(obj).toJson()));
}

void NotifySettings::appRemoved(QString appName)
{
    if (m_settings == nullptr)
        return;
    QJsonObject obj = QJsonDocument::fromJson(m_settings->get("notifycations-settings").toString().toUtf8()).object();
    obj.remove(appName);
    m_settings->set("notifycations-settings", QString(QJsonDocument(obj).toJson()));
}


