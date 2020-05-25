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
#include <QIcon>
#include <QSettings>

#include <DGuiApplicationHelper>
#include <DApplication>
#include <DDBusSender>

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
    changeTheme();
    initPluginState();
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &NotificationsPlugin::changeTheme);
    connect(m_notifyInter, &NotifyInter::systemSettingChanged, this, &NotificationsPlugin::updateDockIcon);
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
    m_proxyInter->saveValue(this, PLUGIN_STATE_KEY, !m_proxyInter->getValue(this, PLUGIN_STATE_KEY, true).toBool());

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

    if (mode == Dock::DisplayMode::Fashion) {
        return m_proxyInter->getValue(this, key, 2).toInt();
    } else {
        return m_proxyInter->getValue(this, key, 5).toInt();
    }
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

    m_pluginLoaded = true;

    m_itemWidget = new NotificationsWidget;

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
        setIconShow(false);
        m_proxyInter->itemRemoved(this, pluginName());    
    } else {
        setIconShow(true);
        if (!m_pluginLoaded) {
            loadPlugin();
            return;
        }
        m_proxyInter->itemAdded(this, pluginName());
    }
}

void NotificationsPlugin::initPluginState()
{
    QJsonObject obj = QJsonDocument::fromJson(m_notifyInter->systemSetting().toUtf8()).object();
    if (!obj.isEmpty()) {
        QJsonObject systemObj = obj["SystemNotify"].toObject();
        m_disturb = systemObj["DoNotDisturb"].toBool();
        m_isShowIcon = systemObj["ShowIconOnDock"].toBool();
    }
}

void NotificationsPlugin::updateDockIcon()
{
    initPluginState();

    m_proxyInter->saveValue(this, PLUGIN_STATE_KEY, m_isShowIcon);
    if (m_isShowIcon)
        m_itemWidget->setDisturb(m_disturb);
    refreshPluginItemsVisible();
}

void NotificationsPlugin::setIconShow(bool isShow)
{
    QMap<QString,QVariant> SystemNotify;
    QMap<QString,QVariant> setDoNotDisturb;
    setDoNotDisturb["ShowIconOnDock"] = isShow;
    SystemNotify["SystemNotify"] = setDoNotDisturb;
    QString Dbusmsg = QJsonDocument::fromVariant(SystemNotify).toJson();
    m_notifyInter->setSystemSetting(Dbusmsg);
}

const QString NotificationsPlugin::itemContextMenu(const QString &itemKey)
{
    if (itemKey != "notifications")
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
    if (menuId == "disturb")
    {
        m_disturb = m_disturb ? false:true;
        m_itemWidget->setDisturb(m_disturb);
        //接口来自接口文档
        QMap<QString,QVariant> SystemNotify;
        QMap<QString,QVariant> setDoNotDisturb;
        setDoNotDisturb["DoNotDisturb"] = m_disturb;
        SystemNotify["SystemNotify"] = setDoNotDisturb;
        //转成json
        QString Dbusmsg = QJsonDocument::fromVariant(SystemNotify).toJson();
        //发送数据给dbus接口
        m_notifyInter->setSystemSetting(Dbusmsg);
    }
    else if (menuId == "setting")
    {
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
