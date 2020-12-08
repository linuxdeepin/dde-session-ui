#include <gtest/gtest.h>

#include "notification-center/bubbletitlewidget.h"

#include <QApplication>
#include <QMimeData>

class TstBubbleTitleWidget : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new BubbleTitleWidget();
    }

    void TearDown() override
    {
//        delete obj;
//        obj = nullptr;
    }

public:
    BubbleTitleWidget *obj = nullptr;
};

TEST_F(TstBubbleTitleWidget, coverageTest)
{

}
