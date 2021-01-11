#include <gtest/gtest.h>

#include "notification/bubblemanager.h"

#include <QApplication>
#include <QMimeData>

class TstBubbleManager : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new BubbleManager();
    }

    void TearDown() override
    {
//        delete obj;
//        obj = nullptr;
    }

public:
    BubbleManager *obj = nullptr;
};

TEST_F(TstBubbleManager, coverageTest)
{
//    obj->Toggle();
//    obj->GetAppList();
//    obj->recordCount();
//    obj->GetSystemInfo(0);
//    obj->ClearRecords();
//    obj->getAllSetting();
//    obj->CloseNotification(0);
}
