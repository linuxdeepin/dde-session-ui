#include <gtest/gtest.h>

#include "notification/appicon.h"

#include <QApplication>
#include <QMimeData>

class TstAppIcon : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new AppIcon();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    AppIcon *obj = nullptr;
};

TEST_F(TstAppIcon, coverageTest)
{
    obj->setIcon("dde-control-center");
}
