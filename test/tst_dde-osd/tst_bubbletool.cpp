#include <gtest/gtest.h>

#include "notification/bubbletool.h"

#include <QApplication>
#include <QMimeData>

class TstBubbleTool : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new BubbleTool();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    BubbleTool *widget = nullptr;
};

TEST_F(TstBubbleTool, coverageTest)
{

}
