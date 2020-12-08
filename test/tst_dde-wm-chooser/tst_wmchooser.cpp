#include <gtest/gtest.h>

#include "wmchooser.h"

#include <QApplication>
#include <QMimeData>

class TstWMChooser : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new WMChooser();
    }

    void TearDown() override
    {
//        delete obj;
//        obj = nullptr;
    }

public:
    WMChooser *obj = nullptr;
};

TEST_F(TstWMChooser, coverageTest)
{

}
