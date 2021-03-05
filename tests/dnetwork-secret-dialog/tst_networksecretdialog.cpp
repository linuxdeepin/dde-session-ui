#include <gtest/gtest.h>

#include "networksecretdialog.h"

#include <QApplication>
#include <QMimeData>

class TstNetworkSecretDialog : public testing::Test
{
public:
    void SetUp() override
    {
//        widget = new NetworkSecretDialog();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    NetworkSecretDialog *widget = nullptr;
};

TEST_F(TstNetworkSecretDialog, coverageTest)
{

}
