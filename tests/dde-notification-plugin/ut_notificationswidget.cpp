#define private public
#define protected public
#include "notifications/notificationswidget.h"
#undef private
#undef protected

#include <DGuiApplicationHelper>

#include <QTest>

#include <gtest/gtest.h>

DGUI_USE_NAMESPACE

class UT_NotificationsWidget : public testing::Test
{

};

TEST_F(UT_NotificationsWidget, coverageTest)
{
    NotificationsWidget widget;
    widget.setDisturb(true);
    EXPECT_TRUE(widget.m_disturb);

    QResizeEvent resizeevent = QResizeEvent(QSize(480, 480), QSize(640, 480));
    widget.resizeEvent(&resizeevent);

    widget.resize(480, 480);
    QTest::mousePress(&widget, Qt::LeftButton, Qt::NoModifier);
    EXPECT_TRUE(widget.m_pressed);

    QTest::mouseRelease(&widget, Qt::LeftButton, Qt::NoModifier);
    EXPECT_FALSE(widget.m_pressed);
    EXPECT_FALSE(widget.m_hover);

    QMouseEvent mouseEvent = QMouseEvent(QEvent::MouseMove, QPointF(10, 10), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    widget.mouseMoveEvent(&mouseEvent);

    auto event = QEvent(QEvent::Leave);
    qApp->sendEvent(&widget, &event);
    EXPECT_FALSE(widget.m_pressed);
    EXPECT_FALSE(widget.m_hover);

    QCursor::setPos(600,600);
    EXPECT_FALSE(widget.containCursorPos());

    const QPixmap &pixmap = widget.loadSvg("notification", ":/icons/resources/icons/", PLUGIN_ICON_MAX_SIZE, widget.devicePixelRatioF());
    EXPECT_FALSE(pixmap.isNull());

    DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);
    widget.m_hover = true;
    widget.m_pressed = true;
    widget.paintEvent(nullptr);

    DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::DarkType);
    widget.m_hover = true;
    widget.m_pressed = true;
    widget.paintEvent(nullptr);
}
