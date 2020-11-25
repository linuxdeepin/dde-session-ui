#include <gtest/gtest.h>

#include "wmchooser.h"

#include <QApplication>
#include <QMimeData>

class TstWMChooser : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new WMChooser();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    WMChooser *widget = nullptr;
};

TEST_F(TstWMChooser, coverageTest)
{

}
