#include <gtest/gtest.h>

#include "notification/appbodylabel.h"

#include <QApplication>
#include <QMimeData>

class UT_AppBodyLabel : public testing::Test
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

TEST_F(UT_AppBodyLabel, coverageTest)
{
    obj->update();
}

TEST_F(UT_AppBodyLabel, textTest)
{
    QString str = "文件已保存到/home/chenwei/Desktop/截图录屏_dde-desktop_20210305170903.gif";
    obj->setText(str);
    EXPECT_TRUE(obj->minimumSizeHint() == QSize(640, 26));
    EXPECT_TRUE(obj->sizeHint() == QSize(640, 52));
    EXPECT_TRUE(str == obj->labelText());
}

TEST_F(UT_AppBodyLabel, opacityTest)
{
    qreal opacity = 0.5;
    obj->setOpacity(opacity);
    EXPECT_TRUE(opacity == obj->bodyOpacity());
}

TEST_F(UT_AppBodyLabel, setAlignmentTest)
{
    Qt::Alignment alignment = Qt::AlignTop;
    obj->setAlignment(alignment);
    EXPECT_TRUE(alignment == obj->bodyAlignment());
}
