#include <gtest/gtest.h>

#include "notification-center/notifylistview.h"
#include "notification-center/notifymodel.h"
#include "notification/persistence.h"

#include <QApplication>
#include <QMimeData>
#include <QtTest>

class TstNotifyListView : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new NotifyListView();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    NotifyListView *obj = nullptr;
};

TEST_F(TstNotifyListView, coverageTest)
{
    // 测试model的方法，直接构造一个QAbstractItemModelTester，Qt会自动测试该model类
    NotifyListView view;
    Persistence database;
    NotifyModel model(&database);
    auto tester = new QAbstractItemModelTester(&model);
    Q_UNUSED(tester)
}
