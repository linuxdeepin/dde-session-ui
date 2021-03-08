#include <gtest/gtest.h>

#include "notification-center/overlapwidet.h"

#include <QApplication>
#include <QMimeData>

class UT_OverLapWidet : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new OverLapWidet();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    OverLapWidet *obj = nullptr;
};

TEST_F(UT_OverLapWidet, coverageTest)
{

}
