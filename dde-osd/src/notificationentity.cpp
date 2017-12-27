/**
 * Copyright (C) 2014 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "notificationentity.h"

NotificationEntity::NotificationEntity(const QString &appName, const QString &id,
                                       const QString &appIcon, const QString &summary,
                                       const QString &body, const QStringList &actions,
                                       const QVariantMap hints, QObject *parent) :
    QObject(parent),
    m_appName(appName),
    m_id(id),
    m_appIcon(appIcon),
    m_summary(summary),
    m_body(body),
    m_actions(actions),
    m_hints(hints)
{
}

NotificationEntity::NotificationEntity(const NotificationEntity &notify) :
    NotificationEntity(notify.appName(), notify.id(), notify.appIcon(), notify.summary(),
                       notify.body(), notify.actions(), notify.hints())
{

}

NotificationEntity &NotificationEntity::operator=(const NotificationEntity &notify)
{
    NotificationEntity ent(notify);
    return ent;
}

QString NotificationEntity::appName() const
{
    return m_appName;
}

void NotificationEntity::setAppName(const QString &appName)
{
    m_appName = appName;
}

QString NotificationEntity::id() const
{
    return m_id;
}

void NotificationEntity::setId(const QString &id)
{
    m_id = id;
}
QString NotificationEntity::appIcon() const
{
    return m_appIcon;
}

void NotificationEntity::setAppIcon(const QString &appIcon)
{
    m_appIcon = appIcon;
}
QString NotificationEntity::summary() const
{
    return m_summary;
}

void NotificationEntity::setSummary(const QString &summary)
{
    m_summary = summary;
}
QString NotificationEntity::body() const
{
    return m_body;
}

void NotificationEntity::setBody(const QString &body)
{
    m_body = body;
}
QStringList NotificationEntity::actions() const
{
    return m_actions;
}

void NotificationEntity::setActions(const QStringList &actions)
{
    m_actions = actions;
}
QVariantMap NotificationEntity::hints() const
{
    return m_hints;
}

void NotificationEntity::setHints(const QVariantMap &hints)
{
    m_hints = hints;
}

QString NotificationEntity::ctime() const
{
    return m_id;
}







