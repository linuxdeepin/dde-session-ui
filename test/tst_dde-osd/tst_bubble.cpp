#include <gtest/gtest.h>

#include "notification/bubble.h"

#include <QApplication>
#include <QMimeData>

class TstBubble : public testing::Test
{
public:
    void SetUp() override
    {
//        widget = new Bubble();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    Bubble *widget = nullptr;
};

TEST_F(TstBubble, coverageTest)
{

}
