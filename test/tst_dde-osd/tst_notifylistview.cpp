#include <gtest/gtest.h>

#include "notification-center/notifylistview.h"

#include <QApplication>
#include <QMimeData>

class TstNotifyListView : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new NotifyListView();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    NotifyListView *widget = nullptr;
};

TEST_F(TstNotifyListView, coverageTest)
{

}
