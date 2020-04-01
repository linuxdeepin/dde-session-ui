#include "clearbutton.h"

#include <QPainter>
#include <QIcon>
#include <QStyle>
#include <QDebug>

ClearButton::ClearButton(QWidget *parent)
    : DWidget(parent)
    , m_iconPath("")
    , m_hasFocus(false)
    , m_hover(false)
    , m_opacity(0)
    , m_radius(8)
    , m_hasBackColor(false)
{

}

ClearButton::ClearButton(const QString &text, QWidget *parent)
    : ClearButton(parent)
{
    setIconPath(text);
}

void ClearButton::setIconPath(const QString &iconPath)
{
    m_iconPath = iconPath;

    update();
}

void ClearButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QColor color;
    color = palette().color(QPalette::Base);
    if (m_hasBackColor) {
        color.setAlpha(m_hover ? m_opacity : (m_opacity / 2));
    } else {
        color.setAlpha(m_hasFocus ? 80 : (m_hover ? 0 : 255));
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawRoundedRect(rect(), m_radius, m_radius);
    QPixmap pix = QIcon(m_iconPath).pixmap(size());
    painter.drawPixmap(rect(), pix);
}

void ClearButton::setFocusState(bool has)
{
    m_hasFocus = has;

    update();
}

void ClearButton::setBackOpacity(int opacity)
{
    if (opacity < 0 || m_opacity > 255)
        return;

    m_opacity = opacity;

    m_hasBackColor = true;

    update();
}

void ClearButton::setRadius(int radius)
{
    m_radius = radius;

    update();
}

void ClearButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    Q_EMIT clicked();
}

void ClearButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_hover = true;

    update();

    return DWidget::enterEvent(event);
}

void ClearButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_hover = false;

    update();

    return DWidget::leaveEvent(event);
}
