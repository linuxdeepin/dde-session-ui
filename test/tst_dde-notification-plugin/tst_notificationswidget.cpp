#include <gtest/gtest.h>

#include "notifications/notificationswidget.h"

#include <QApplication>
#include <QMimeData>

class TstNotificationsWidget : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new NotificationsWidget();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    NotificationsWidget *widget = nullptr;
};

TEST_F(TstNotificationsWidget, coverageTest)
{

}
