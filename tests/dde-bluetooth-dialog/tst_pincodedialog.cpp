#include <gtest/gtest.h>

#include "pincodedialog.h"

#include <QApplication>
#include <QMimeData>

class TstPinCodeDialog : public testing::Test
{
public:
    void SetUp() override
    {
//        dialog = new PinCodeDialog("123456", "/com/deepin/", QTime::currentTime().toString());
    }

    void TearDown() override
    {
        delete dialog;
        dialog = nullptr;
    }

public:
    PinCodeDialog *dialog = nullptr;
};

TEST_F(TstPinCodeDialog, coverageTest)
{
//    ASSERT_TRUE("123456" == dialog->pinCode());
}

