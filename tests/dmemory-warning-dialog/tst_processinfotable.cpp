#include <gtest/gtest.h>

#include "processinfotable.h"

#include <QApplication>
#include <QMimeData>

class TstProcessInfoView : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new ProcessInfoView();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    ProcessInfoView *widget = nullptr;
};

TEST_F(TstProcessInfoView, coverageTest)
{

}
