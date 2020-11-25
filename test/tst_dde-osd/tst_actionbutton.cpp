#include <gtest/gtest.h>

#include "notification/actionbutton.h"

#include <QApplication>
#include <QMimeData>

class TstActionButton : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new ActionButton();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    ActionButton *widget = nullptr;
};

TEST_F(TstActionButton, coverageTest)
{

}
