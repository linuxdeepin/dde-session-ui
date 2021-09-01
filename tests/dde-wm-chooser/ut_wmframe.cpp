#include "wmframe.h"

#include <gtest/gtest.h>

class UT_WMFrame : public testing::Test
{

};

TEST_F(UT_WMFrame, coverageTest)
{
    WMFrame wmFrame;
    wmFrame.setConfigPath("config.ini");
}
