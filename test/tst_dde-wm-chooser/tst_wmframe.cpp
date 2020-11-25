#include <gtest/gtest.h>

#include "wmframe.h"

#include <QApplication>
#include <QMimeData>

class TstWMFrame : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new WMFrame();
    }

    void TearDown() override
    {
//        delete obj;
//        obj = nullptr;
    }

public:
    WMFrame *obj = nullptr;
};

TEST_F(TstWMFrame, coverageTest)
{

}
