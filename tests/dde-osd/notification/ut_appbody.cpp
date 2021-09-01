#include "notification/appbody.h"

#include <gtest/gtest.h>

class UT_AppBody : public testing::Test
{

};

TEST_F(UT_AppBody, coverageTest)
{
    AppBody appbody;
    appbody.setTitle("控制中心");
    appbody.setText("已连接“有线链接2”");
}

