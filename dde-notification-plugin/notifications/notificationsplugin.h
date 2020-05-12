/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *
 * Maintainer: sbw <sbw@sbw.so>
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

#ifndef NOTIFITIONPLUGIN_H
#define NOTIFITIONPLUGIN_H

#include "pluginsiteminterface.h"
#include "notificationswidget.h"

#include <QDBusInterface>
#include <QLabel>

#include <QList>
#include <QMap>
#include <QVariant>
#include <QJsonDocument>

#include <com_deepin_dde_notification.h>

using NotifyInter = com::deepin::dde::Notification;

class NotificationsPlugin : public QObject, PluginsItemInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginsItemInterface)
    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginsItemInterface" FILE "notifications.json")

public:
    explicit NotificationsPlugin(QObject *parent = 0);

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
    int itemSortKey(const QString &itemKey) Q_DECL_OVERRIDE;
    void setSortKey(const QString &itemKey, const int order) Q_DECL_OVERRIDE;
    void pluginSettingsChanged() override;
    const QString itemContextMenu(const QString &itemKey) override;
    void invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked) override;

//    QDBusInterface *getNotifyInterface();

private slots:
    void changeTheme();

private:
    void loadPlugin();
    bool checkSwap();
    void refreshPluginItemsVisible();
    void getDndModel();

private:
    bool m_pluginLoaded;
//    QDBusInterface *m_interface = nullptr;
    NotifyInter *m_notifyInter;
    //Judge whether it is in not Disturb mode
    bool m_disturb = false;

    NotificationsWidget *m_itemWidget = nullptr;
    QLabel *m_tipsLabel;
};

#endif // NOTIFITIONPLUGIN_H
