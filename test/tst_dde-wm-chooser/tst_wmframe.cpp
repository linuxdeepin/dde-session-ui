#include <gtest/gtest.h>

#include "wmframe.h"

#include <QApplication>
#include <QMimeData>

class TstWMFrame : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new WMFrame();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    WMFrame *widget = nullptr;
};

TEST_F(TstWMFrame, coverageTest)
{

}
