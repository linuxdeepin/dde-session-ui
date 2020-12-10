#include <gtest/gtest.h>

#include "notification/iconbutton.h"

#include <QApplication>
#include <QMimeData>

class TstIconButton : public testing::Test
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

TEST_F(TstIconButton, coverageTest)
{
    obj->setIcon("");
    obj->setText("Test");
    obj->setRadius(10);
    obj->setFocusState(false);
    obj->setBackOpacity(0.5 * 255);
}
