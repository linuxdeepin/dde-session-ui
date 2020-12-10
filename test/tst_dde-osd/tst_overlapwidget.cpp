#include <gtest/gtest.h>

#include "notification-center/overlapwidet.h"

#include <QApplication>
#include <QMimeData>

class TstOverLapWidet : public testing::Test
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

TEST_F(TstOverLapWidet, coverageTest)
{

}
