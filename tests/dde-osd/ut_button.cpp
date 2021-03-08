#include <gtest/gtest.h>

#include "notification/button.h"

#include <QApplication>
#include <QMimeData>
#include <QAction>
#include <QPainter>

class UT_Button : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new Button();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    Button *obj = nullptr;
};

TEST_F(UT_Button, coverageTest)
{

}

TEST_F(UT_Button, valueTest)
{
    obj->setRadius(10);
    EXPECT_TRUE(10 == obj->radius());

    obj->setId("Test");
    EXPECT_TRUE("Test" == obj->id());

    obj->setText("Test");
    EXPECT_TRUE("Test" == obj->text());

    obj->setTextAlignment(Qt::AlignTop);
    EXPECT_TRUE(Qt::AlignTop == obj->textAlignment());

    obj->setHoverState(true);
    EXPECT_TRUE(obj->boverState());

    QSize size(60, 50);
    EXPECT_TRUE(size == obj->sizeHint());

    QSize minisize(30, 25);
    EXPECT_TRUE(minisize == obj->minimumSizeHint());

    obj->setPixmap(QPixmap());
}

TEST_F(UT_Button, actionTest)
{
    QAction action(nullptr);
    obj->addAction(&action);
}
