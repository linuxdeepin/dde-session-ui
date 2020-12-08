#include <gtest/gtest.h>

#include "notification/actionbutton.h"

#include <QApplication>
#include <QMimeData>

class TstActionButton : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new ActionButton();
        actionList << "_open" << "查看"
                   << "_close" << "忽略";
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    ActionButton *obj = nullptr;
    QStringList actionList;
};

TEST_F(TstActionButton, coverageTest)
{

}

TEST_F(TstActionButton, addButtonsTest)
{
    obj->addButtons(actionList);
    int count = obj->buttonList().size();

    EXPECT_FALSE(obj->isEmpty());
    EXPECT_EQ(actionList.size() / 2, count);
}

TEST_F(TstActionButton, clearButtonsTest)
{
    obj->clear();
    EXPECT_TRUE(obj->isEmpty());
}

TEST_F(TstActionButton, buttonsSizeTest)
{
    EXPECT_TRUE(QSize(70, 60) == obj->buttonSize());

    QSize size(100, 90);
    obj->setButtonSize(size);
    EXPECT_TRUE(size == obj->buttonSize());
}

TEST_F(TstActionButton, contentSizeTest)
{
    QSize size = obj->contentSize("查看", false);
    EXPECT_TRUE(QSize(70, 40) == size);
}
