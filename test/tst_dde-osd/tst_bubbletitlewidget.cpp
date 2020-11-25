#include <gtest/gtest.h>

#include "notification-center/bubbletitlewidget.h"

#include <QApplication>
#include <QMimeData>

class TstBubbleTitleWidget : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new BubbleTitleWidget();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    BubbleTitleWidget *widget = nullptr;
};

TEST_F(TstBubbleTitleWidget, coverageTest)
{

}
