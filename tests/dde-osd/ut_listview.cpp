#include <gtest/gtest.h>

#include "listview.h"

#include <QApplication>
#include <QMimeData>

class UT_ListView : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new ListView();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    ListView *obj = nullptr;
};

TEST_F(UT_ListView, coverageTest)
{

}
