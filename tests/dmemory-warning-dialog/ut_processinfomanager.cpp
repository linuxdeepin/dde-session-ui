#define private public
#include "processinfomanager.h"
#undef private

#include <gtest/gtest.h>

class UT_ProcessInfoManager : public testing::Test
{

};

TEST_F(UT_ProcessInfoManager, coverageTest)
{
    ProcessInfoManager manager;
    manager.scanChromeTabs();
}
