#include <gtest/gtest.h>

#include "notification-center/bubbleitem.h"

#include <QApplication>
#include <QMimeData>

class TstBubbleItem : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new BubbleItem();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    BubbleItem *widget = nullptr;
};

TEST_F(TstBubbleItem, coverageTest)
{

}
