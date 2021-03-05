#include <gtest/gtest.h>

#include "warningdialog.h"

#include <QApplication>
#include <QMimeData>

class TstWarningDialog : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new WarningDialog();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    WarningDialog *widget = nullptr;
};

TEST_F(TstWarningDialog, coverageTest)
{

}
