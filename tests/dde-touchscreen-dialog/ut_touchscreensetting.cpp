#include <gtest/gtest.h>

#include "touchscreensetting.h"

#include <QApplication>
#include <QMimeData>

class UT_TouchscreenSetting : public testing::Test
{
public:
    void SetUp() override
    {
        // setting = new TouchscreenSetting("/com/deepin/daemon/Display/1");
    }

    void TearDown() override
    {
//        delete setting;
//        setting = nullptr;
    }

public:
    TouchscreenSetting *setting = nullptr;
};

TEST_F(UT_TouchscreenSetting, coverageTest)
{

}
