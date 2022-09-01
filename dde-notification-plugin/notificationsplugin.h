// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NOTIFITIONPLUGIN_H
#define NOTIFITIONPLUGIN_H

#include "notificationswidget.h"

#include <QDBusInterface>
#include <QLabel>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QJsonDocument>

#include <pluginsiteminterface.h>
#include <com_deepin_dde_notification.h>

#define DOCK_DEFAULT_POS    9
#define KEY_DNDMODE         0
#define KEY_SHOWICON        5

using NotifyInter = com::deepin::dde::Notification;

class QGSettings;
class NotificationsPlugin : public QObject, PluginsItemInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginsItemInterface)
    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginsItemInterface" FILE "notifications.json")

public:
    explicit NotificationsPlugin(QObject *parent = nullptr);

    const QString pluginName() const override;
    const QString pluginDisplayName() const override;
    void init(PluginProxyInterface *proxyInter) override;
    void pluginStateSwitched() override;
    bool pluginIsAllowDisable() override { return true; }
    bool pluginIsDisable() override;
    QWidget *itemWidget(const QString &itemKey) override;
    QWidget *itemTipsWidget(const QString &itemKey) override;
    const QString itemCommand(const QString &itemKey) override;
    void displayModeChanged(const Dock::DisplayMode displayMode) override;
    int itemSortKey(const QString &itemKey) override;
    void setSortKey(const QString &itemKey, const int order) override;
    void pluginSettingsChanged() override;
    const QString itemContextMenu(const QString &itemKey) override;
    void invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked) override;

private slots:
    void changeTheme();

private:
    void loadPlugin();
    bool checkSwap();
    void refreshPluginItemsVisible();
    void initPluginState();
    void updateDockIcon(uint item, const QDBusVariant &var);

private:
    bool m_pluginLoaded;
    NotifyInter *m_notifyInter;
    //Judge whether it is in not Disturb mode
    bool m_disturb = false;
    bool m_isShowIcon = true;

    NotificationsWidget *m_itemWidget = nullptr;
    QLabel *m_tipsLabel;
    QGSettings *m_settings;
};

#endif // NOTIFITIONPLUGIN_H
