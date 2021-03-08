#include <gtest/gtest.h>

#include "notification/appicon.h"

#include <QApplication>
#include <QMimeData>

class UT_AppIcon : public testing::Test
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

TEST_F(UT_AppIcon, coverageTest)
{
    obj->setIcon("dde-control-center");
    obj->setIcon("data:image/dde-control-center");
}
