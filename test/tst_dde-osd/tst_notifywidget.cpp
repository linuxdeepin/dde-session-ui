#include <gtest/gtest.h>

#include "notification-center/notifywidget.h"

#include <QApplication>
#include <QMimeData>

class TstNotifyWidget : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new NotifyWidget();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    NotifyWidget *widget = nullptr;
};

TEST_F(TstNotifyWidget, coverageTest)
{

}
