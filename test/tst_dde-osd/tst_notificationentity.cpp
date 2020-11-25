#include <gtest/gtest.h>

#include "notification/notificationentity.h"

#include <QApplication>
#include <QMimeData>

class TstNotificationEntity : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new NotificationEntity();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    NotificationEntity *widget = nullptr;
};

TEST_F(TstNotificationEntity, coverageTest)
{

}
