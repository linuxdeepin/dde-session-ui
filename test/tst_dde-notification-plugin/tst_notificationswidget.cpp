#include <gtest/gtest.h>

#include "notifications/notificationswidget.h"

#include <QApplication>
#include <QMimeData>

class TstNotificationsWidget : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new NotificationsWidget();
    }

    void TearDown() override
    {
//        delete obj;
//        obj = nullptr;
    }

public:
    NotificationsWidget *obj = nullptr;
};

TEST_F(TstNotificationsWidget, coverageTest)
{

}
