#include <gtest/gtest.h>

#include"notifications/notificationsplugin.h"

#include <QApplication>
#include <QMimeData>

class TstNotificationsPlugin : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new NotificationsPlugin();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    NotificationsPlugin *widget = nullptr;
};

TEST_F(TstNotificationsPlugin, coverageTest)
{

}
