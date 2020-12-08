#include <gtest/gtest.h>

#include "notification/appicon.h"

#include <QApplication>
#include <QMimeData>

class TstAppIcon : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new AppIcon();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    AppIcon *widget = nullptr;
};

TEST_F(TstAppIcon, coverageTest)
{

}
