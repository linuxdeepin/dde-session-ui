#include <gtest/gtest.h>

#include "notification/bubblemanager.h"

#include <QApplication>
#include <QMimeData>

class TstBubbleManager : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new BubbleManager();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    BubbleManager *widget = nullptr;
};

TEST_F(TstBubbleManager, coverageTest)
{

}
