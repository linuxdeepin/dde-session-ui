#include "notification/bubbletool.h"

#include <QWidget>

#include <gtest/gtest.h>

class UT_BubbleTool : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new QWidget(nullptr);
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    QWidget *obj = nullptr;
};

TEST_F(UT_BubbleTool, coverageTest)
{
//    BubbleTool::register_wm_state(obj->winId());
}

TEST_F(UT_BubbleTool, getAppNameTest)
{
    QString key = "deepin-control-center";
    QString appName = BubbleTool::getDeepinAppName(key);//获取应用名称
//    EXPECT_EQ("控制中心", appName);
}
