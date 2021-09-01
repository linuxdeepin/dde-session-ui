#include "mockpersistence.h"
#include "notification-center/notifymodel.h"
#include "notification-center/itemdelegate.h"
#include "notification-center/notifylistview.h"
#include "overlapwidet.h"

#include <QAbstractItemModelTester>
#include <QFocusEvent>
#include <QApplication>
#include <QTimer>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockPersistence;
class PersistenceHelper;

class UT_NotifyListview : public testing::Test
{
public:
    void SetUp() override
    {
        persistence = new MockPersistence;
        helper = new PersistenceHelper;

        EXPECT_CALL(*persistence, getAllNotify()).
                    WillRepeatedly(testing::Invoke(helper, &PersistenceHelper::getAllNotify));

        EXPECT_CALL(*persistence, getAll()).
                    WillRepeatedly(testing::Invoke(helper, &PersistenceHelper::getAll));

        EXPECT_CALL(*persistence, addOne(testing::_)).
                    WillRepeatedly(testing::Invoke(helper, &PersistenceHelper::addOne));

        EXPECT_CALL(*persistence, addAll(testing::_)).
                    WillRepeatedly(testing::Invoke(helper, &PersistenceHelper::addAll));

        EXPECT_CALL(*persistence, removeOne(testing::_)).
                    WillRepeatedly(testing::Invoke(helper, &PersistenceHelper::removeOne));

        EXPECT_CALL(*persistence, removeApp(testing::_)).
                    WillRepeatedly(testing::Invoke(helper, &PersistenceHelper::removeApp));

        EXPECT_CALL(*persistence, getRecordCount()).
                    WillRepeatedly(testing::Invoke(helper, &PersistenceHelper::getRecordCount));

        EXPECT_CALL(*persistence, getById(testing::_)).
                    WillRepeatedly(testing::Invoke(helper, &PersistenceHelper::getById));

        EXPECT_CALL(*persistence, getFrom(testing::_, testing::_)).
                    WillRepeatedly(testing::Invoke(helper, &PersistenceHelper::getFrom));

        obj = new NotifyListView();
        model = new NotifyModel(nullptr, persistence, obj);
        delegate = new ItemDelegate(obj, model);
        obj->setModel(model);
        obj->setItemDelegate(delegate);
    }

    void TearDown() override
    {
        delete obj;
        delete model;
        delete delegate;
        delete persistence;
        delete helper;

        obj = nullptr;
        model = nullptr;
        delegate = nullptr;
        persistence = nullptr;
        helper = nullptr;
    }

public:
    NotifyListView *obj = nullptr;
    NotifyModel *model = nullptr;
    ItemDelegate *delegate = nullptr;
    MockPersistence *persistence = nullptr;
    PersistenceHelper *helper = nullptr;
};

TEST_F(UT_NotifyListview, coverageTest)
{
    auto notification = std::make_shared<NotificationEntity>("deepin-editor");
    ListItem item = model->getAppData("deepin-editor");
    obj->setVisible(true);
    obj->show();
    obj->aniState();
    model->cacheData(notification);
    model->freeData();
    model->removeNotify(notification);
    model->expandData("deepin-editor");
    model->collapseData();
    model->removeAppGroup("deepin-editor");
    model->removeTimeOutNotify();
    model->removeAllData();
}

