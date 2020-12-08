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
#include <QtConcurrent>
#include <QStringList>

DCORE_USE_NAMESPACE

const QString schemaKey = "com.deepin.dde.notifications";
const QString schemaPath = "/com/deepin/dde/notifications/";
const QString appSchemaKey = "com.deepin.dde.notifications.applications";
const QString appSchemaPath = "/com/deepin/dde/notifications/applications/%1/";

NotifySettings::NotifySettings(QObject *parent)
    : QObject(parent)
    , m_initTimer(new QTimer(this))
    , m_launcherInter(new LauncherInter("com.deepin.dde.daemon.Launcher",
                                        "/com/deepin/dde/daemon/Launcher",
                                        QDBusConnection::sessionBus(), this))
{
    registerLauncherItemInfoListMetaType();
    registerLauncherItemInfoMetaType();

    if (!QGSettings::isSchemaInstalled("com.deepin.dde.notification")) {
        qDebug()<<"System configuration fetch failed!";
    }
    m_initTimer->start(1000);
    m_systemSetting = new QGSettings(schemaKey.toLocal8Bit(), schemaPath.toLocal8Bit(), this);

    connect(m_initTimer, &QTimer::timeout, this, &NotifySettings::initAllSettings);
    connect(m_launcherInter, &LauncherInter::ItemChanged, this, [ = ] (QString action, LauncherItemInfo info, qlonglong id) {
        Q_UNUSED(id)
        if (action == "deleted") {
            appRemoved(info.ID);
        } else if (action == "created") {
            appAdded(info);
        }
    });
}

void NotifySettings::initAllSettings()
{
    LauncherItemInfoList itemInfoList = m_launcherInter->GetAllItemInfos();
    if (!itemInfoList.isEmpty()) {
        m_initTimer->stop();
    }

    QStringList appList = m_systemSetting->get("app-list").toStringList();

    foreach(const LauncherItemInfo &item, itemInfoList) {
        DDesktopEntry desktopInfo(item.Path);
        if (IgnoreList.contains(item.ID) || desktopInfo.rawValue("X-Created-By") == "Deepin WINE Team") {
            continue;
        }

        if (appList.contains(item.ID)) {
            // 修改系统语言后需要更新翻译
            QGSettings itemSetting(appSchemaKey.toLocal8Bit(), appSchemaPath.arg(item.ID).toLocal8Bit(), this);
            itemSetting.set("app-name", item.Name);
            continue;
        }
        appList.append(item.ID);
        m_systemSetting->set("app-list", appList);
        appAdded(item);
    }
}

void NotifySettings::setAppSetting(const QString &id, const NotifySettings::AppConfigurationItem &item, const QVariant &var)
{
    QGSettings itemSetting(appSchemaKey.toLocal8Bit(), appSchemaPath.arg(id).toLocal8Bit(), this);
    switch (item) {
    case APPNAME:
        itemSetting.set("app-name", var);
        break;
    case APPICON:
        itemSetting.set("app-icon", var);
        break;
    case ENABELNOTIFICATION:
        itemSetting.set("enable-notification", var);
        break;
    case ENABELPREVIEW:
        itemSetting.set("enable-preview", var);
        break;
    case ENABELSOUND:
        itemSetting.set("enable-sound", var);
        break;
    case SHOWINNOTIFICATIONCENTER:
        itemSetting.set("show-in-notification-center", var);
        break;
    case LOCKSCREENSHOWNOTIFICATION:
        itemSetting.set("lockscreen-show-notification", var);
        break;
    }

    Q_EMIT appSettingChanged(id, item, var);
}

QVariant NotifySettings::getAppSetting(const QString &id, const NotifySettings::AppConfigurationItem &item)
{
    QGSettings itemSetting(appSchemaKey.toLocal8Bit(), appSchemaPath.arg(id).toLocal8Bit(), this);

    QVariant results;
    switch (item) {
    case APPNAME:
        results = itemSetting.get("app-name");
        break;
    case APPICON:
        results = itemSetting.get("app-icon");
        break;
    case ENABELNOTIFICATION:
        results = itemSetting.get("enable-notification");
        break;
    case ENABELPREVIEW:
        results = itemSetting.get("enable-preview");
        break;
    case ENABELSOUND:
        results = itemSetting.get("enable-sound");
        break;
    case SHOWINNOTIFICATIONCENTER:
        results = itemSetting.get("show-in-notification-center");
        break;
    case LOCKSCREENSHOWNOTIFICATION:
        results = itemSetting.get("lockscreen-show-notification");
        break;
    }
    return results;
}

void NotifySettings::setSystemSetting(const NotifySettings::SystemConfigurationItem &item, const QVariant &var)
{
    switch (item) {
    case DNDMODE:
        m_systemSetting->set("dndmode", var);
        break;
    case LOCKSCREENOPENDNDMODE:
        m_systemSetting->set("lockscreen-open-dndmode", var);
        break;
    case OPENBYTIMEINTERVAL:
        m_systemSetting->set("open-by-time-interval", var);
        break;
    case STARTTIME:
        m_systemSetting->set("start-time", var);
        break;
    case ENDTIME:
        m_systemSetting->set("end-time", var);
        break;
    case SHOWICON:
        m_systemSetting->set("show-icon", var);
        break;
    }

    Q_EMIT systemSettingChanged(item, var);
}

QVariant NotifySettings::getSystemSetting(const NotifySettings::SystemConfigurationItem &item)
{
    QVariant results;
    switch (item) {
    case DNDMODE:
        results = m_systemSetting->get("dndmode");
        break;
    case LOCKSCREENOPENDNDMODE:
        results = m_systemSetting->get("lockscreen-open-dndmode");
        break;
    case OPENBYTIMEINTERVAL:
        results = m_systemSetting->get("open-by-time-interval");
        break;
    case STARTTIME:
        results = m_systemSetting->get("start-time");
        break;
    case ENDTIME:
        results = m_systemSetting->get("end-time");
        break;
    case SHOWICON:
        results = m_systemSetting->get("show-icon");
        break;
    }
    return results;
}

QStringList NotifySettings::getAppLists()
{
    return m_systemSetting->get("app-list").toStringList();
}

void NotifySettings::appAdded(const LauncherItemInfo &info)
{
    QStringList appList = m_systemSetting->get("app-list").toStringList();
    if (!appList.contains(info.ID)) {
        appList.append(info.ID);
        m_systemSetting->set("app-list", appList);
    }

    QGSettings itemSetting(appSchemaKey.toLocal8Bit(), appSchemaPath.arg(info.ID).toLocal8Bit(), this);

    itemSetting.set("app-name", info.Name);
    itemSetting.set("app-icon", info.Icon);
    itemSetting.set("enable-notification", DEFAULT_ALLOW_NOTIFY);
    itemSetting.set("enable-preview", DEFAULT_SHOW_NOTIFY_PREVIEW);
    itemSetting.set("enable-sound", DEFAULT_NOTIFY_SOUND);
    itemSetting.set("show-in-notification-center", DEFAULT_ONLY_IN_NOTIFY);
    itemSetting.set("lockscreen-show-notification", DEFAULT_LOCK_SHOW_NOTIFY);

    Q_EMIT appAddedSignal(info.ID);
}

void NotifySettings::appRemoved(const QString &id)
{
    QStringList appList = m_systemSetting->get("app-list").toStringList();
    if (appList.contains(id)) {
        appList.removeOne(id);
        m_systemSetting->set("app-list", appList);
    }

    QGSettings itemSetting(appSchemaKey.toLocal8Bit(), appSchemaPath.arg(id).toLocal8Bit(), this);
    itemSetting.reset("app-name");
    itemSetting.reset("app-icon");
    itemSetting.reset("enable-notification");
    itemSetting.reset("enable-preview");
    itemSetting.reset("enable-sound");
    itemSetting.reset("show-in-notification-center");
    itemSetting.reset("lockscreen-show-notification");

    Q_EMIT appRemovedSignal(id);
}

void NotifySettings::setAppSetting(QString settings)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(settings.toLocal8Bit()).object();
    QString id = jsonObj.begin().key();
    jsonObj = jsonObj.begin().value().toObject();
    QGSettings itemSetting(appSchemaKey.toLocal8Bit(), appSchemaPath.arg(id).toLocal8Bit(), this);
    itemSetting.set("enable-notification", jsonObj[AllowNotifyStr].toBool());
    itemSetting.set("show-in-notification-center", jsonObj[ShowInNotifyCenterStr].toBool());
    itemSetting.set("lockscreen-show-notification", jsonObj[LockShowNotifyStr].toBool());
    itemSetting.set("enable-preview", jsonObj[ShowNotifyPreviewStr].toBool());
    itemSetting.set("enable-sound", jsonObj[NotificationSoundStr].toBool());
    itemSetting.set("app-icon", jsonObj[AppIconStr].toString());
    itemSetting.set("app-name", jsonObj[AppNameStr].toString());
}

QString NotifySettings::getAppSettings(const QString &id)
{
    QGSettings itemSetting(appSchemaKey.toLocal8Bit(), appSchemaPath.arg(id).toLocal8Bit(), this);
    QJsonObject jsonObj;
    jsonObj.insert(AllowNotifyStr, itemSetting.get("enable-notification").toJsonValue());
    jsonObj.insert(ShowInNotifyCenterStr, itemSetting.get("show-in-notification-center").toJsonValue());
    jsonObj.insert(LockShowNotifyStr, itemSetting.get("lockscreen-show-notification").toJsonValue());
    jsonObj.insert(ShowNotifyPreviewStr, itemSetting.get("enable-preview").toJsonValue());
    jsonObj.insert(NotificationSoundStr, itemSetting.get("enable-sound").toJsonValue());
    jsonObj.insert(AppIconStr, itemSetting.get("app-icon").toJsonValue());
    jsonObj.insert(AppNameStr, itemSetting.get("app-name").toJsonValue());
    QJsonObject appSetingObj;
    appSetingObj[id] = jsonObj;
    return QString(QJsonDocument(appSetingObj).toJson());
}

void NotifySettings::setSystemSetting(QString settings)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(settings.toUtf8()).object();
    jsonObj = jsonObj.begin().value().toObject();
    if (jsonObj.contains(DoNotDisturbStr)) {
        m_systemSetting->set("dndmode", jsonObj[DoNotDisturbStr].toBool());
        Q_EMIT systemSettingChanged(DNDMODE, jsonObj[DoNotDisturbStr].toBool());
    }
    if (jsonObj.contains(ScreenLockedStr)) {
        m_systemSetting->set("lockscreen-open-dndmode", jsonObj[ScreenLockedStr].toBool());
    }
    if (jsonObj.contains(TimeSlotStr)) {
        m_systemSetting->set("open-by-time-interval", jsonObj[TimeSlotStr].toBool());
    }
    if (jsonObj.contains(StartTimeStr)) {
        m_systemSetting->set("start-time", jsonObj[StartTimeStr].toString());
    }
    if (jsonObj.contains(EndTimeStr)) {
        m_systemSetting->set("end-time", jsonObj[EndTimeStr].toString());
    }
    if (jsonObj.contains(ShowIconOnDockStr)) {
        m_systemSetting->set("show-icon", jsonObj[ShowIconOnDockStr].toBool());
        Q_EMIT systemSettingChanged(SHOWICON, jsonObj[ShowIconOnDockStr].toBool());
    }
}

QString NotifySettings::getSystemSetings()
{
    QJsonObject jsonObj;
    jsonObj.insert(DoNotDisturbStr, m_systemSetting->get("dndmode").toJsonValue());
    jsonObj.insert(ScreenLockedStr, m_systemSetting->get("lockscreen-open-dndmode").toJsonValue());
    jsonObj.insert(TimeSlotStr, m_systemSetting->get("open-by-time-interval").toJsonValue());
    jsonObj.insert(StartTimeStr, m_systemSetting->get("start-time").toJsonValue());
    jsonObj.insert(EndTimeStr, m_systemSetting->get("end-time").toJsonValue());
    jsonObj.insert(ShowIconOnDockStr, m_systemSetting->get("show-icon").toJsonValue());
    QJsonObject SystemSetingObj;
    SystemSetingObj[SystemNotifySettingStr] = jsonObj;
    return QString(QJsonDocument(SystemSetingObj).toJson());
}

void NotifySettings::setAllSetting(QString settings)
{
    // 未被使用的接口废弃
    Q_UNUSED(settings)
}

QString NotifySettings::getAllSetings()
{
    QStringList appList = m_systemSetting->get("app-list").toStringList();

    QJsonObject jsonObj;
    foreach (const auto &id, appList) {
        QGSettings itemSetting(appSchemaKey.toLocal8Bit(), appSchemaPath.arg(id).toLocal8Bit(), this);
        QJsonObject itemObj;
        itemObj.insert(AllowNotifyStr, itemSetting.get("enable-notification").toJsonValue());
        itemObj.insert(ShowInNotifyCenterStr, itemSetting.get("show-in-notification-center").toJsonValue());
        itemObj.insert(LockShowNotifyStr, itemSetting.get("lockscreen-show-notification").toJsonValue());
        itemObj.insert(ShowNotifyPreviewStr, itemSetting.get("enable-preview").toJsonValue());
        itemObj.insert(NotificationSoundStr, itemSetting.get("enable-sound").toJsonValue());
        itemObj.insert(AppIconStr, itemSetting.get("app-icon").toJsonValue());
        itemObj.insert(AppNameStr, itemSetting.get("app-name").toJsonValue());
        jsonObj[id] = itemObj;
    }

    QJsonObject Obj;
    Obj.insert(DoNotDisturbStr, m_systemSetting->get("dndmode").toJsonValue());
    Obj.insert(ScreenLockedStr, m_systemSetting->get("lockscreen-open-dndmode").toJsonValue());
    Obj.insert(TimeSlotStr, m_systemSetting->get("open-by-time-interval").toJsonValue());
    Obj.insert(StartTimeStr, m_systemSetting->get("start-time").toJsonValue());
    Obj.insert(EndTimeStr, m_systemSetting->get("end-time").toJsonValue());
    Obj.insert(ShowIconOnDockStr, m_systemSetting->get("show-icon").toJsonValue());
    jsonObj[SystemNotifySettingStr] = Obj;
    return QString(QJsonDocument(jsonObj).toJson());
}


