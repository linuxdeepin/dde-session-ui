#define private public
#include "dockrect.h"
#undef private

#include <QDebug>

#include <gtest/gtest.h>

class UT_DockRect : public testing::Test
{
public:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }

public:
    DockRect obj;
};

TEST_F(UT_DockRect, coverageTest)
{
    qDebug() << obj;
    QDBusArgument arg ;
    arg << obj;
    arg >> obj;
}
