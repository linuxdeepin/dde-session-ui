#include <gtest/gtest.h>

#include "mainwidget.h"

#include <QApplication>
#include <QMimeData>

class TstMainWidget : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new MainWidget();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    MainWidget *widget = nullptr;
};

TEST_F(TstMainWidget, coverageTest)
{

}
