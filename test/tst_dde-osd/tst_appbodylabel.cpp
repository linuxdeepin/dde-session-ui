#include <gtest/gtest.h>

#include "notification/appbodylabel.h"

#include <QApplication>
#include <QMimeData>

class TstAppBodyLabel : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new AppBodyLabel();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    AppBodyLabel *widget = nullptr;
};

TEST_F(TstAppBodyLabel, coverageTest)
{

}
