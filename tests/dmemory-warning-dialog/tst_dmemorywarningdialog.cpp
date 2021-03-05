#include <gtest/gtest.h>

#include "dmemorywarningdialog.h"

#include <QApplication>
#include <QMimeData>

class TstDMemoryWarningDialog : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new DMemoryWarningDialog();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    DMemoryWarningDialog *widget = nullptr;
};

TEST_F(TstDMemoryWarningDialog, coverageTest)
{

}
