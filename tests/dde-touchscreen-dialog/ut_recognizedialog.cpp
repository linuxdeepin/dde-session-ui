#include <gtest/gtest.h>

#include "monitorindicator.h"
#include "recognizedialog.h"

#include <QApplication>
#include <QMimeData>

class UT_RecognizeDialog : public testing::Test
{
public:
    void SetUp() override
    {
        // monitor = new Monitor("com.deepin.daemon.Display", "/com/deepin/daemon/Display/1", QDBusConnection::sessionBus());
        // widget = new RecognizeDialog(monitor);
    }

    void TearDown() override
    {
//        delete widget;
//        widget = nullptr;
    }

public:
    Monitor *monitor = nullptr;
    RecognizeDialog *widget = nullptr;
};

TEST_F(UT_RecognizeDialog, coverageTest)
{

}
