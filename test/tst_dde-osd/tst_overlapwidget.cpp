#include <gtest/gtest.h>

#include "notification-center/overlapwidet.h"

#include <QApplication>
#include <QMimeData>

class TstOverLapWidet : public testing::Test
{
public:
    void SetUp() override
    {
//        widget = new OverLapWidet();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    OverLapWidet *widget = nullptr;
};

TEST_F(TstOverLapWidet, coverageTest)
{

}
