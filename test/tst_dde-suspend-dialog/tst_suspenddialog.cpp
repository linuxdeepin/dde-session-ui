#include <gtest/gtest.h>

#include "suspenddialog.h"

#include <QApplication>
#include <QMimeData>

class TstSuspendDialog : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new SuspendDialog(QRect());
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    SuspendDialog *widget = nullptr;
};

TEST_F(TstSuspendDialog, coverageTest)
{

}
