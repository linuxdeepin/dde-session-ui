#include <gtest/gtest.h>

#include "notification/button.h"

#include <QApplication>
#include <QMimeData>

class TstButton : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new Button();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    Button *widget = nullptr;
};

TEST_F(TstButton, coverageTest)
{

}
