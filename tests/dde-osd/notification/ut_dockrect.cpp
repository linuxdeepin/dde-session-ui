#define private public
#include "dockrect.h"
#undef private

#include <QDebug>

#include <gtest/gtest.h>

class UT_DockRect : public testing::Test
{

};

TEST_F(UT_DockRect, coverageTest)
{
    DockRect obj;
    qDebug() << obj;
    QDBusArgument arg ;
    arg << obj;
    arg >> obj;
}
