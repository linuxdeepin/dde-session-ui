#include <gtest/gtest.h>

#include "notification/appbodylabel.h"

#include <QApplication>
#include <QMimeData>

class TstAppBodyLabel : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new AppBodyLabel();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    AppBodyLabel *obj = nullptr;
};

TEST_F(TstAppBodyLabel, coverageTest)
{

}

TEST_F(TstAppBodyLabel, textTest)
{
    QString str = "GTEST";
    obj->setText(str);
    EXPECT_TRUE(str == obj->labelText());
}

TEST_F(TstAppBodyLabel, opacityTest)
{
    qreal opacity = 0.5;
    obj->setOpacity(opacity);
    EXPECT_TRUE(opacity == obj->bodyOpacity());
}

TEST_F(TstAppBodyLabel, setAlignmentTest)
{
    Qt::Alignment alignment = Qt::AlignTop;
    obj->setAlignment(alignment);
    EXPECT_TRUE(alignment == obj->bodyAlignment());
}
