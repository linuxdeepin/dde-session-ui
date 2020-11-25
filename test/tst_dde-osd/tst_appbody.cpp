#include <gtest/gtest.h>

#include "notification/appbody.h"

#include <QApplication>
#include <QMimeData>

class TstAppBody : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new AppBody();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    AppBody *widget = nullptr;
};

TEST_F(TstAppBody, coverageTest)
{

}

