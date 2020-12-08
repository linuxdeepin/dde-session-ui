#include <gtest/gtest.h>

#include "listview.h"

#include <QApplication>
#include <QMimeData>

class TstListView : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new ListView();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    ListView *widget = nullptr;
};

TEST_F(TstListView, coverageTest)
{

}
