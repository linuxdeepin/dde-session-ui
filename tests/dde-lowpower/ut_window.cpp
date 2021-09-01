#include "window.h"

#include <gtest/gtest.h>

class UT_Window : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new Window();
        adaptor = new LowPowerAdaptor(widget);
    }

    void TearDown() override
    {
        if (widget) {
            delete widget;
            widget = nullptr;
        }
    }

public:
    Window *widget = nullptr;
    LowPowerAdaptor *adaptor = nullptr;
};

TEST_F(UT_Window, coverageTest)
{
    adaptor->Raise();
    adaptor->Quit();
}
