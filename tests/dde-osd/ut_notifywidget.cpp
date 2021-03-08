#include <gtest/gtest.h>

#include "notification-center/notifywidget.h"

#include <QApplication>
#include <QMimeData>

class UT_NotifyWidget : public testing::Test
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

TEST_F(UT_NotifyWidget, coverageTest)
{
    obj->view();
    obj->model();
}
