#include <gtest/gtest.h>

#include "processinfomanager.h"

#include <QApplication>
#include <QMimeData>

class TstProcessInfoManager : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new ProcessInfoManager();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    ProcessInfoManager *widget = nullptr;
};

TEST_F(TstProcessInfoManager, coverageTest)
{

}
