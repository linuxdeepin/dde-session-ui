#include <gtest/gtest.h>

#include "notification-center/bubbleitem.h"

#include <QApplication>
#include <QMimeData>

class TstBubbleItem : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new BubbleItem();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    BubbleItem *obj = nullptr;
};

TEST_F(TstBubbleItem, coverageTest)
{

}
