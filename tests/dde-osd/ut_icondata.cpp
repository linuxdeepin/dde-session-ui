#include "icondata.h"

#include <QDebug>

#include <gtest/gtest.h>


class UT_IconData : public testing::Test
{
public:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }

public:
    IconData obj;
};

TEST_F(UT_IconData, coverageTest)
{
    obj.bit = 1;
    obj.alpha = true;
    obj.array = "";
    obj.width = 10;
    obj.cannel = 1;
    obj.height = 10;
    obj.rowstride = 0;

    qDebug() << obj;
    QDBusArgument arg ;
    arg << obj;
    arg >> obj;
}
