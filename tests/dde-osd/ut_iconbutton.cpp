#include <gtest/gtest.h>

#define protected public
#include "notification/iconbutton.h"
#undef protected

#include <QApplication>
#include <QMimeData>
#include <QPaintEvent>
#include <QTest>

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
    delete obj;
    obj = nullptr;

    obj = new IconButton("test");
    QPaintEvent painevent(QRect(0, 0, 100, 100));
    obj->paintEvent(&painevent);

    QTest::mousePress(obj, Qt::LeftButton);
    QTest::mouseRelease(obj, Qt::LeftButton);
    QTest::mouseMove(obj);
    QEvent event(QEvent::Leave);
    qApp->sendEvent(obj, &event);
    event = QEvent(QEvent::Enter);
    qApp->sendEvent(obj, &event);
    QTest::keyPress(obj, Qt::Key_Enter);
}
