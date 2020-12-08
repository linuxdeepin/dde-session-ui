#include <gtest/gtest.h>

#include "monitorindicator.h"

#include <QApplication>
#include <QMimeData>

class TstMonitorIndicator : public testing::Test
{
public:
    void SetUp() override
    {
//        widget = new MonitorIndicator();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    MonitorIndicator *widget = nullptr;
};

TEST_F(TstMonitorIndicator, coverageTest)
{

}
