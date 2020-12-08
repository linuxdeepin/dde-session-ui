#include <gtest/gtest.h>

#include "mainwidget.h"

#include <QApplication>
#include <QMimeData>

class TstMainWidget : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new MainWidget();
    }

    void TearDown() override
    {
//        delete obj;
//        obj = nullptr;
    }

public:
    MainWidget *obj = nullptr;
};

TEST_F(TstMainWidget, coverageTest)
{

}
