#include <gtest/gtest.h>

#include "recognizedialog.h"

#include <QApplication>
#include <QMimeData>

class TstRecognizeDialog : public testing::Test
{
public:
    void SetUp() override
    {
//        widget = new RecognizeDialog();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    RecognizeDialog *widget = nullptr;
};

TEST_F(TstRecognizeDialog, coverageTest)
{

}
