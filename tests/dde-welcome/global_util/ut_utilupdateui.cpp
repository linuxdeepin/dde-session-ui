#include <gtest/gtest.h>

#include "util_updateui.h"

#include <QWidget>

class UtUtilUpdateUI : public testing::Test
{
public:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(UtUtilUpdateUI, coverageTest)
{
    QWidget w;
    updateStyle("://theme/fullfilesystem.qss", &w);
}


