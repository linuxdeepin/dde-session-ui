#include <gtest/gtest.h>

#include "notification/appbody.h"

#include <QApplication>
#include <QMimeData>

class TstAppBody : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new AppBody();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    AppBody *obj = nullptr;
};

TEST_F(TstAppBody, coverageTest)
{
    obj->setTitle("控制中心");
    obj->setText("已连接“有线链接2”");
}

