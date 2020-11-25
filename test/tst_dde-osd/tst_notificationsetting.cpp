#include <gtest/gtest.h>

#include "notification/notifysettings.h"

#include <QApplication>
#include <QMimeData>

class TstNotifySettings : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new NotifySettings();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    NotifySettings *widget = nullptr;
};

TEST_F(TstNotifySettings, coverageTest)
{

}
