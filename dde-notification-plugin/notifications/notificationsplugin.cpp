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

#include "notificationsplugin.h"

#include <QDBusConnectionInterface>
#include <DGuiApplicationHelper>
#include <DApplication>
#include <DDBusSender>
#include <QIcon>
#include <QGSettings>

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

#define PLUGIN_STATE_KEY    "enable"

const QString server = "com.deepin.dde.Notification";
const QString path = "/com/deepin/dde/Notification";

NotificationsPlugin::NotificationsPlugin(QObject *parent)
    : QObject(parent)
    , m_pluginLoaded(false)
    , m_notifyInter(new NotifyInter(server, path, QDBusConnection::sessionBus(), this))
    , m_tipsLabel(new QLabel)
{
    m_tipsLabel->setVisible(false);
    m_tipsLabel->setObjectName("notifications");
    m_tipsLabel->setAccessibleName("TipsLabel");

    if (QGSettings::isSchemaInstalled("com.deepin.dde.notifications")) {
        m_settings = new QGSettings("com.deepin.dde.notifications", "/com/deepin/dde/notifications/", this);
    }
    changeTheme();

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &NotificationsPlugin::changeTheme);
    connect(m_notifyInter, &NotifyInter::SystemInfoChanged, this, &NotificationsPlugin::updateDockIcon);
}

void NotificationsPlugin::changeTheme()
{
    QPalette pa = m_tipsLabel->palette();
    pa.setBrush(QPalette::WindowText, pa.brightText());
    m_tipsLabel->setPalette(pa);
}

const QString NotificationsPlugin::pluginName() const
{
    return "notifications";
}

const QString NotificationsPlugin::pluginDisplayName() const
{
    return tr("Notification Center");
}

QWidget *NotificationsPlugin::itemWidget(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    return m_itemWidget;
}

QWidget *NotificationsPlugin::itemTipsWidget(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    if (!m_notifyInter->isValid())
        return nullptr;

    uint recordCount = m_notifyInter->recordCount();

    if (recordCount)
        m_tipsLabel->setText(QString(tr("%1 Notifications")).arg(recordCount));
    else
        m_tipsLabel->setText(tr("No messages"));

    return m_tipsLabel;
}

void NotificationsPlugin::init(PluginProxyInterface *proxyInter)
{
    // 第一次是为了正确加载翻译，第二次是为了避免影响dock的accessible
    QString applicationName = qApp->applicationName();
    qApp->setApplicationName("dde-session-ui");
    qApp->loadTranslator();
    qApp->setApplicationName(applicationName);

    m_proxyInter = proxyInter;

    if (!pluginIsDisable()) {
        loadPlugin();
    }
}

void NotificationsPlugin::pluginStateSwitched()
{
    bool pluginState = !m_proxyInter->getValue(this, PLUGIN_STATE_KEY, true).toBool();
    m_proxyInter->saveValue(this, PLUGIN_STATE_KEY, pluginState);
    m_notifyInter->SetSystemInfo(5, QDBusVariant(pluginState));
    refreshPluginItemsVisible();
}

bool NotificationsPlugin::pluginIsDisable()
{
    return !m_proxyInter->getValue(this, PLUGIN_STATE_KEY, true).toBool();
}

const QString NotificationsPlugin::itemCommand(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    m_notifyInter->Toggle();
    return "";
}

void NotificationsPlugin::displayModeChanged(const Dock::DisplayMode displayMode)
{
    Q_UNUSED(displayMode);

    if (!pluginIsDisable()) {
        m_itemWidget->update();
    }
}

int NotificationsPlugin::itemSortKey(const QString &itemKey)
{
    Dock::DisplayMode mode = displayMode();
    const QString key = QString("pos_%1_%2").arg(itemKey).arg(mode);
    return m_proxyInter->getValue(this, key, DOCK_DEFAULT_POS).toInt();
}

void NotificationsPlugin::setSortKey(const QString &itemKey, const int order)
{
    const QString key = QString("pos_%1_%2").arg(itemKey).arg(displayMode());
    m_proxyInter->saveValue(this, key, order);
}

void NotificationsPlugin::pluginSettingsChanged()
{
    refreshPluginItemsVisible();
}

void NotificationsPlugin::loadPlugin()
{
    if (m_pluginLoaded)
        return;

    initPluginState();

    m_pluginLoaded = true;

    m_itemWidget = new NotificationsWidget;
    m_itemWidget->setAccessibleName("ItemWidget");
    if (m_isShowIcon) {
        m_proxyInter->itemAdded(this, pluginName());
        m_itemWidget->setDisturb(m_disturb);
    }

    displayModeChanged(displayMode());
}

bool NotificationsPlugin::checkSwap()
{
    QFile file("/proc/swaps");
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        const QString &body = file.readAll();
        file.close();
        QRegularExpression re("\\spartition\\s");
        QRegularExpressionMatch match = re.match(body);
        return match.hasMatch();
    }

    return false;
}

void NotificationsPlugin::refreshPluginItemsVisible()
{
    if (pluginIsDisable()) {
        m_proxyInter->itemRemoved(this, pluginName());
    } else {
        if (!m_pluginLoaded) {
            loadPlugin();
            return;
        }
        m_proxyInter->itemAdded(this, pluginName());
    }
}

void NotificationsPlugin::initPluginState()
{
    if (m_settings == nullptr)
        return;
    m_isShowIcon = m_proxyInter->getValue(this, PLUGIN_STATE_KEY, true).toBool();
    m_settings->set("show-icon", m_isShowIcon);
    m_disturb = m_settings->get("dndmode").toBool();
}

void NotificationsPlugin::updateDockIcon(uint item, const QDBusVariant &var)
{
    if (item == KEY_DNDMODE) {
        m_disturb = var.variant().toBool();
    } else if (item == KEY_SHOWICON) {
        m_isShowIcon = var.variant().toBool();
    }

    m_proxyInter->saveValue(this, PLUGIN_STATE_KEY, m_isShowIcon);
    m_itemWidget->setDisturb(m_disturb);
    refreshPluginItemsVisible();
}

const QString NotificationsPlugin::itemContextMenu(const QString &itemKey)
{
    if (itemKey != "notifications")
        return QString();

    static QGSettings gsettings("com.deepin.dde.dock.module.notifications");
    if (gsettings.keys().contains("menuEnable") && !gsettings.get("menuEnable").toBool())
        return QString();

    QList<QVariant> items;
    items.reserve(2);

    //Add do not disturb mode button
    QMap<QString,QVariant> disturbs;
    disturbs["itemId"] = "disturb";
    disturbs["itemText"] = m_disturb ? tr("Turn off DND mode") : tr("Turn on DND mode");
    disturbs["isActive"] =true;
    items.push_back(disturbs);

    // Add notification settings button
    QMap<QString, QVariant> settings;
    settings["itemId"] = "setting";
    settings["itemText"] = tr("Notification settings");
    settings["isActive"] = true;
    items.push_back(settings);

    QMap<QString, QVariant> menu;
    menu["items"] = items;
    menu["checkableMenu"] = false;
    menu["singleCheck"] = false;

    return QJsonDocument::fromVariant(menu).toJson();
}

void NotificationsPlugin::invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked)
{
    Q_UNUSED(checked);
    Q_UNUSED(itemKey);
    if (menuId == "disturb") {
        m_disturb = m_disturb ? false:true;
        m_itemWidget->setDisturb(m_disturb);
        m_notifyInter->SetSystemInfo(KEY_DNDMODE, QDBusVariant(m_disturb));
    } else if (menuId == "setting") {
        DDBusSender()
            .service("com.deepin.dde.ControlCenter")
            .interface("com.deepin.dde.ControlCenter")
            .path("/com/deepin/dde/ControlCenter")
            .method("ShowPage")
            .arg(QString("notification"))
            .arg(QString("System Notification"))
            .call();
    }
}
