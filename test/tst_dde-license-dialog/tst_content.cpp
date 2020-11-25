#include <gtest/gtest.h>

#include "content.h"

#include <QApplication>
#include <QMimeData>

class TstContent : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new Content();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    Content *widget = nullptr;
};

TEST_F(TstContent, coverageTest)
{

}
