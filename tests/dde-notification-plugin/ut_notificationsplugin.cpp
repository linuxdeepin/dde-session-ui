#define private public
#define protected public
#include "notifications/notificationsplugin.h"
#include "pluginsiteminterface.h"
#undef private
#undef protected

#include <QSharedPointer>
#include <QGSettings>
#include <QJsonDocument>
#include <QJsonObject>

#include <gtest/gtest.h>

class PluginProxyInterfaceObj : public PluginProxyInterface {
public:
    PluginProxyInterfaceObj() {}
    void itemAdded(PluginsItemInterface * const, const QString &) {}
    void itemUpdate(PluginsItemInterface * const, const QString &) {}
    void itemRemoved(PluginsItemInterface * const, const QString &) {}
    void requestWindowAutoHide(PluginsItemInterface * const, const QString &, const bool) {}
    void requestRefreshWindowVisible(PluginsItemInterface * const, const QString &) {}
    void requestSetAppletVisible(PluginsItemInterface * const, const QString &, const bool) {}
    void saveValue(PluginsItemInterface * const, const QString &, const QVariant &) {}
    const QVariant getValue(PluginsItemInterface * const, const QString &key, const QVariant &) {
        if (key == "enable")
            return QVariant(true);
        else
            return QVariant(DOCK_DEFAULT_POS);
    }
    void removeValue(PluginsItemInterface * const, const QStringList &) {}
};

class UT_NotificationsPlugin : public testing::Test
{

};

TEST_F(UT_NotificationsPlugin, coverageTest)
{
    //保存当前通知图标在通知栏的状态，测试完后恢复状态
    bool isNotificationEnabel = true;
    QGSettings settings("com.deepin.dde.dock", "/com/deepin/dde/dock/");
    if (settings.keys().contains("plugin-settings")) {
        auto jsonstr = settings.get("plugin-settings").toByteArray();
        auto jsondocument = QJsonDocument::fromJson(jsonstr);
        auto jsonobj = jsondocument.object();
        if (jsonobj.contains("notifications")) {
            QJsonObject notifycationsJsonObj = jsonobj.value("notifycations").toObject();
            if (notifycationsJsonObj.contains("enable")) {
                isNotificationEnabel = notifycationsJsonObj.value("enabel").toBool();
            }
        }
    }

    NotificationsPlugin plugin;
    EXPECT_TRUE(plugin.m_notifyInter != nullptr);
    EXPECT_TRUE(plugin.m_tipsLabel != nullptr);
    EXPECT_TRUE(plugin.pluginIsAllowDisable());

    QSharedPointer<PluginProxyInterfaceObj> interfaceObj(new PluginProxyInterfaceObj());
    plugin.init(interfaceObj.data());
    plugin.initPluginState();
    EXPECT_FALSE(plugin.pluginIsDisable());
    plugin.loadPlugin();
    EXPECT_TRUE(plugin.m_pluginLoaded);
    EXPECT_TRUE(plugin.itemWidget("") != nullptr);
    EXPECT_TRUE(plugin.m_isShowIcon);

    EXPECT_EQ(plugin.pluginName(), "notifications");
    EXPECT_FALSE(plugin.pluginDisplayName().isEmpty());

    auto lable = plugin.itemTipsWidget("");
    if (plugin.m_notifyInter->isValid())
        EXPECT_FALSE(plugin.m_tipsLabel->text().isEmpty());
    else
        EXPECT_EQ(lable, nullptr);
    EXPECT_EQ(plugin.itemCommand(""), "");
    plugin.displayModeChanged(Dock::DisplayMode::Fashion);
    static QGSettings gsettings("com.deepin.dde.dock.module.notifications");
    if (gsettings.keys().contains("menuEnable") && !gsettings.get("menuEnable").toBool())
        EXPECT_EQ(plugin.itemContextMenu("notifications"), QString());
    else
        EXPECT_FALSE(plugin.itemContextMenu("notifications").isEmpty());
    EXPECT_EQ(plugin.itemSortKey(""), DOCK_DEFAULT_POS);
    plugin.updateDockIcon(KEY_DNDMODE, QDBusVariant(true));
    EXPECT_TRUE(plugin.m_disturb);
    EXPECT_TRUE(plugin.m_isShowIcon);
    plugin.pluginStateSwitched();
    plugin.updateDockIcon(KEY_SHOWICON, QDBusVariant(isNotificationEnabel));
    plugin.refreshPluginItemsVisible();
    NotificationsWidget *itemWidget = new NotificationsWidget;
    plugin.m_itemWidget = itemWidget;
    plugin.invokedMenuItem("", "disturb", true);
    plugin.invokedMenuItem("", "setting", true);
    plugin.checkSwap();
    plugin.setSortKey("notifycations", 9);
    plugin.pluginSettingsChanged();


    //恢复插件原本的enable状态
    plugin.m_notifyInter->SetSystemInfo(5, QDBusVariant(isNotificationEnabel));
}

