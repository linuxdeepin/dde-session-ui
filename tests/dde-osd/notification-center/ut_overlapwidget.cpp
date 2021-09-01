#define private public
#include "notification-center/overlapwidet.h"
#include "notifymodel.h"
#include "notifylistview.h"
#include "notificationentity.h"
#undef private

#include <QApplication>
#include <QTest>

#include <gtest/gtest.h>

class UT_OverLapWidet : public testing::Test
{
public:
    void SetUp() override
    {
        model = new NotifyModel();
        entityPtr = EntityPtr(new NotificationEntity("deepin-editor", "0", "deepin-editor", "", ""));
        obj = new OverLapWidet(model, entityPtr);
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;

        delete model;
        model = nullptr;
    }

public:
    OverLapWidet *obj = nullptr;
    NotifyModel *model = nullptr;
    EntityPtr entityPtr;
};

TEST_F(UT_OverLapWidet, coverageTest)
{
    model->addNotify(entityPtr);
    NotifyListView *listview = new NotifyListView();
    listview->setModel(model);
    obj->setParentView(listview);
    entityPtr->setHideCount(3);
    obj->initOverlap();
    obj->hideOverlapBubble();
    obj->getFaceItem();
    obj->bubbleElements();
    obj->update();

    QEvent event = QEvent(QEvent::FocusIn);
    qApp->sendEvent(obj, &event);
    event = QEvent(QEvent::FocusOut);
    qApp->sendEvent(obj, &event);
}
