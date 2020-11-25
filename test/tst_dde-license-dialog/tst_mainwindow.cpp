#include <gtest/gtest.h>

#include "mainwindow.h"

#include <QApplication>
#include <QMimeData>

class TstMainWindow : public testing::Test
{
public:
    void SetUp() override
    {
//        widget = new MainWindow();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    MainWindow *widget = nullptr;
};

TEST_F(TstMainWindow, coverageTest)
{

}
