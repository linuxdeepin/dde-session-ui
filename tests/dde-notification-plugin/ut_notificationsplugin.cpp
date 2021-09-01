#define private public
#define protected public
#include "notifications/notificationsplugin.h"
#include "pluginsiteminterface.h"
#undef private
#undef protected

#include <QSharedPointer>
#include <QGSettings>

#include <gtest/gtest.h>

class PluginProxyInterfaceObj : public PluginProxyInterface {
public:
    PluginProxyInterfaceObj() {}
    void itemAdded(PluginsItemInterface * const itemInter, const QString &itemKey) {}
    void itemUpdate(PluginsItemInterface * const itemInter, const QString &itemKey) {}
    void itemRemoved(PluginsItemInterface * const itemInter, const QString &itemKey) {}
    void requestWindowAutoHide(PluginsItemInterface * const itemInter, const QString &itemKey, const bool autoHide) {}
    void requestRefreshWindowVisible(PluginsItemInterface * const itemInter, const QString &itemKey) {}
    void requestSetAppletVisible(PluginsItemInterface * const itemInter, const QString &itemKey, const bool visible) {}
    void saveValue(PluginsItemInterface * const itemInter, const QString &key, const QVariant &value) {}
    const QVariant getValue(PluginsItemInterface * const itemInter, const QString &key, const QVariant &fallback) {
        if (key == "enable")
            return QVariant(true);
        else
            return QVariant(DOCK_DEFAULT_POS);
    }
    void removeValue(PluginsItemInterface * const itemInter, const QStringList &keyList) {}
};

class UT_NotificationsPlugin : public testing::Test
{

};

TEST_F(UT_NotificationsPlugin, coverageTest)
{
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
    plugin.refreshPluginItemsVisible();
    plugin.updateDockIcon(KEY_DNDMODE, QDBusVariant(true));
    EXPECT_TRUE(plugin.m_disturb);
    plugin.updateDockIcon(KEY_SHOWICON, QDBusVariant(true));
    EXPECT_TRUE(plugin.m_isShowIcon);
    plugin.pluginStateSwitched();
    NotificationsWidget *itemWidget = new NotificationsWidget;
    plugin.m_itemWidget = itemWidget;
    plugin.invokedMenuItem("", "disturb", true);
    plugin.invokedMenuItem("", "setting", true);
    plugin.checkSwap();
    plugin.setSortKey("notifycations", 9);
    plugin.pluginSettingsChanged();
}

