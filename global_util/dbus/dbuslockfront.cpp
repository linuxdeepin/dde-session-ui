// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dbuslockfront.h"

/*
 * Implementation of interface class DBusLauncherFrame
 */

DBusLockFront::DBusLockFront(QObject *parent)
    : QDBusAbstractInterface("com.deepin.dde.lockFront", "/com/deepin/dde/lockFront", staticInterfaceName(), QDBusConnection::sessionBus(), parent)
{
    QDBusConnection::sessionBus().connect(this->service(), this->path(), "org.freedesktop.DBus.Properties",  "PropertiesChanged","sa{sv}as", this, SLOT(__propertyChanged__(QDBusMessage)));
}

DBusLockFront::~DBusLockFront()
{
    QDBusConnection::sessionBus().disconnect(service(), path(), "org.freedesktop.DBus.Properties",  "PropertiesChanged",  "sa{sv}as", this, SLOT(propertyChanged(QDBusMessage)));
}

