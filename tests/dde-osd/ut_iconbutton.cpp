#include <gtest/gtest.h>

#include "notification/iconbutton.h"

#include <QApplication>
#include <QMimeData>

class UT_IconButton : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new IconButton();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    IconButton *obj = nullptr;
};

TEST_F(UT_IconButton, coverageTest)
{
    obj->setIcon("");
    obj->setText("Test");
    obj->setRadius(10);
    obj->setFocusState(false);
    obj->setOpacity(IconButton::HOVER, 0.5 * 255);
}
