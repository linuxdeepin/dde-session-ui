#include <gtest/gtest.h>

#include "window.h"

#include <QApplication>
#include <QMimeData>

class TstWindow : public testing::Test
{
public:
    void SetUp() override
    {
//        widget = new Window();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    Window *widget = nullptr;
};

TEST_F(TstWindow, coverageTest)
{

}
