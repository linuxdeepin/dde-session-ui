#include "dmemorywarningdialogadaptor.h"
#include "dmemorywarningdialog.h"

#include <gtest/gtest.h>

class UT_DMemoryWarningDialogAdaptor : public testing::Test
{

};

TEST_F(UT_DMemoryWarningDialogAdaptor, coverageTest)
{
    DMemoryWarningDialog dialog;
    DMemoryWarningDialogAdaptor adaptor(&dialog);

    adaptor.Show({"google-chrome"});
    adaptor.Hide();
}
