#include <gtest/gtest.h>

#include "notification/iconbutton.h"

#include <QApplication>
#include <QMimeData>

class TstIconButton : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new IconButton();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    IconButton *widget = nullptr;
};

TEST_F(TstIconButton, coverageTest)
{

}
