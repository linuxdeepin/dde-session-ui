#include <gtest/gtest.h>

#include "touchscreensetting.h"

#include <QApplication>
#include <QMimeData>

class TstTouchscreenSetting : public testing::Test
{
public:
    void SetUp() override
    {
//        widget = new RecognizeDialog();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    TouchscreenSetting *widget = nullptr;
};

TEST_F(TstTouchscreenSetting, coverageTest)
{

}
