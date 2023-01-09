// SPDX-FileCopyrightText: 2014 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "notifications_dbus_adaptor.h"
#include "bubblemanager.h"

#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

DCORE_USE_NAMESPACE

/*
 * Implementation of adaptor class NotificationsDBusAdaptor
 */

NotificationsDBusAdaptor::NotificationsDBusAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

NotificationsDBusAdaptor::~NotificationsDBusAdaptor()
{
    // destructor
}

void NotificationsDBusAdaptor::CloseNotification(uint in0)
{
    // handle method call org.freedesktop.Notifications.CloseNotification
    QMetaObject::invokeMethod(parent(), "CloseNotification", Q_ARG(uint, in0));
}

QStringList NotificationsDBusAdaptor::GetCapabilities()
{
    // handle method call org.freedesktop.Notifications.GetCapabilities
    QStringList out0;
    QMetaObject::invokeMethod(parent(), "GetCapabilities", Q_RETURN_ARG(QStringList, out0));
    return out0;
}

QString NotificationsDBusAdaptor::GetServerInformation(QString &out1, QString &out2, QString &out3)
{
    // handle method call org.freedesktop.Notifications.GetServerInformation
    return static_cast<BubbleManager*>(parent())->GetServerInformation(out1, out2, out3);
}

uint NotificationsDBusAdaptor::Notify(const QString &in0, uint in1, const QString &in2, const QString &in3, const QString &in4, const QStringList &in5, const QVariantMap &in6, int in7)
{
    // handle method call org.freedesktop.Notifications.Notify
    uint out0;
    QMetaObject::invokeMethod(parent(), "Notify", Q_RETURN_ARG(uint, out0), Q_ARG(QString, in0), Q_ARG(uint, in1), Q_ARG(QString, in2), Q_ARG(QString, in3), Q_ARG(QString, in4), Q_ARG(QStringList, in5), Q_ARG(QVariantMap, in6), Q_ARG(int, in7));
    return out0;
}


/*
 * Implementation of adaptor class DDENotifyDBus
 */

DDENotifyDBus::DDENotifyDBus(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

DDENotifyDBus::~DDENotifyDBus()
{
    // destructor
}

QString DDENotifyDBus::allSetting() const
{
    // get the value of property allSetting
    return qvariant_cast< QString >(parent()->property("allSetting"));
}

void DDENotifyDBus::setAllSetting(const QString &value)
{
    // set the value of property allSetting
    parent()->setProperty("allSetting", QVariant::fromValue(value));
}

QString DDENotifyDBus::systemSetting() const
{
    // get the value of property systemSetting
    return qvariant_cast< QString >(parent()->property("systemSetting"));
}

void DDENotifyDBus::setSystemSetting(const QString &value)
{
    // set the value of property systemSetting
    parent()->setProperty("systemSetting", QVariant::fromValue(value));
}

void DDENotifyDBus::ClearRecords()
{
    // handle method call com.deepin.dde.Notification.ClearRecords
    QMetaObject::invokeMethod(parent(), "ClearRecords");
}

void DDENotifyDBus::CloseNotification(uint in0)
{
    // handle method call com.deepin.dde.Notification.CloseNotification
    QMetaObject::invokeMethod(parent(), "CloseNotification", Q_ARG(uint, in0));
}

QString DDENotifyDBus::GetAllRecords()
{
    // handle method call com.deepin.dde.Notification.GetAllRecords
    QString out0;
    QMetaObject::invokeMethod(parent(), "GetAllRecords", Q_RETURN_ARG(QString, out0));
    return out0;
}

QDBusVariant DDENotifyDBus::GetAppInfo(const QString &in0, uint in1)
{
    // handle method call com.deepin.dde.Notification.GetAppInfo
    QDBusVariant out0;
    QMetaObject::invokeMethod(parent(), "GetAppInfo", Q_RETURN_ARG(QDBusVariant, out0), Q_ARG(QString, in0), Q_ARG(uint, in1));
    return out0;
}

QStringList DDENotifyDBus::GetAppList()
{
    // handle method call com.deepin.dde.Notification.GetAppList
    QStringList out0;
    QMetaObject::invokeMethod(parent(), "GetAppList", Q_RETURN_ARG(QStringList, out0));
    return out0;
}

QStringList DDENotifyDBus::GetCapabilities()
{
    // handle method call com.deepin.dde.Notification.GetCapbilities
    QStringList out0;
    QMetaObject::invokeMethod(parent(), "GetCapabilities", Q_RETURN_ARG(QStringList, out0));
    return out0;
}

QString DDENotifyDBus::GetRecordById(const QString &in0)
{
    // handle method call com.deepin.dde.Notification.GetRecordById
    QString out0;
    QMetaObject::invokeMethod(parent(), "GetRecordById", Q_RETURN_ARG(QString, out0), Q_ARG(QString, in0));
    return out0;
}

QString DDENotifyDBus::GetRecordsFromId(int in0, const QString &in1)
{
    // handle method call com.deepin.dde.Notification.GetRecordsFromId
    QString out0;
    QMetaObject::invokeMethod(parent(), "GetRecordsFromId", Q_RETURN_ARG(QString, out0), Q_ARG(int, in0), Q_ARG(QString, in1));
    return out0;
}

QString DDENotifyDBus::GetServerInformation(QString &out1, QString &out2, QString &out3)
{
    // handle method call com.deepin.dde.Notification.GetServerInformation
    return static_cast<BubbleManager *>(parent())->GetServerInformation(out1, out2, out3);
}

QDBusVariant DDENotifyDBus::GetSystemInfo(uint in0)
{
    // handle method call com.deepin.dde.Notification.GetSystemInfo
    QDBusVariant out0;
    QMetaObject::invokeMethod(parent(), "GetSystemInfo", Q_RETURN_ARG(QDBusVariant, out0), Q_ARG(uint, in0));
    return out0;
}

uint DDENotifyDBus::Notify(const QString &in0, uint in1, const QString &in2, const QString &in3, const QString &in4, const QStringList &in5, const QVariantMap &in6, int in7)
{
    // handle method call com.deepin.dde.Notification.Notify
    uint out0;
    QMetaObject::invokeMethod(parent(), "Notify", Q_RETURN_ARG(uint, out0), Q_ARG(QString, in0), Q_ARG(uint, in1), Q_ARG(QString, in2), Q_ARG(QString, in3), Q_ARG(QString, in4), Q_ARG(QStringList, in5), Q_ARG(QVariantMap, in6), Q_ARG(int, in7));
    return out0;
}

void DDENotifyDBus::RemoveRecord(const QString &in0)
{
    // handle method call com.deepin.dde.Notification.RemoveRecord
    QMetaObject::invokeMethod(parent(), "RemoveRecord", Q_ARG(QString, in0));
}

void DDENotifyDBus::SetAppInfo(const QString &in0, uint in1, const QDBusVariant &in2)
{
    // handle method call com.deepin.dde.Notification.SetAppInfo
    QMetaObject::invokeMethod(parent(), "SetAppInfo", Q_ARG(QString, in0), Q_ARG(uint, in1), Q_ARG(QDBusVariant, in2));
}

void DDENotifyDBus::SetSystemInfo(uint in0, const QDBusVariant &in1)
{
    // handle method call com.deepin.dde.Notification.SetSystemInfo
    QMetaObject::invokeMethod(parent(), "SetSystemInfo", Q_ARG(uint, in0), Q_ARG(QDBusVariant, in1));
}

void DDENotifyDBus::Toggle()
{
    // handle method call com.deepin.dde.Notification.Toggle
    QMetaObject::invokeMethod(parent(), "Toggle");
}

void DDENotifyDBus::Show()
{
    // handle method call com.deepin.dde.Notification.Show
    QMetaObject::invokeMethod(parent(), "Show");
}

void DDENotifyDBus::Hide()
{
    // handle method call com.deepin.dde.Notification.Hide
    QMetaObject::invokeMethod(parent(), "Hide");
}

QString DDENotifyDBus::getAppSetting(const QString &in0)
{
    // handle method call com.deepin.dde.Notification.getAppSetting
    QString out0;
    QMetaObject::invokeMethod(parent(), "getAppSetting", Q_RETURN_ARG(QString, out0), Q_ARG(QString, in0));
    return out0;
}

uint DDENotifyDBus::recordCount()
{
    // handle method call com.deepin.dde.Notification.recordCount
    uint out0;
    QMetaObject::invokeMethod(parent(), "recordCount", Q_RETURN_ARG(uint, out0));
    return out0;
}

void DDENotifyDBus::setAppSetting(const QString &in0)
{
    // handle method call com.deepin.dde.Notification.setAppSetting
    QMetaObject::invokeMethod(parent(), "setAppSetting", Q_ARG(QString, in0));
}

