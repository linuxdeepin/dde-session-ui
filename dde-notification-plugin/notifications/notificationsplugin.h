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
