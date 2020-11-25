#include <gtest/gtest.h>

#include "notification/bubblemanager.h"

#include <QApplication>
#include <QMimeData>

class TstBubbleManager : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new BubbleManager();
    }

    void TearDown() override
    {
//        delete obj;
//        obj = nullptr;
    }

public:
    BubbleManager *obj = nullptr;
};

TEST_F(TstBubbleManager, coverageTest)
{

}
