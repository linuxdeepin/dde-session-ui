#include <gtest/gtest.h>

#include "notification-center/notifywidget.h"

#include <QApplication>
#include <QMimeData>

class TstNotifyWidget : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new NotifyWidget();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    NotifyWidget *obj = nullptr;
};

TEST_F(TstNotifyWidget, coverageTest)
{
    obj->view();
    obj->model();
}
