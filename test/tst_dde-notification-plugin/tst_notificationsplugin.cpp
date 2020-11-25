#include <gtest/gtest.h>

#include"notifications/notificationsplugin.h"

#include <QApplication>
#include <QMimeData>

class TstNotificationsPlugin : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new NotificationsPlugin();
    }

    void TearDown() override
    {
//        delete obj;
//        obj = nullptr;
    }

public:
    NotificationsPlugin *obj = nullptr;
};

TEST_F(TstNotificationsPlugin, coverageTest)
{

}
