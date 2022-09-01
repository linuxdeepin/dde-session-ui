// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "notificationswidget.h"

#include <QSvgRenderer>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QIcon>
#include <QPainterPath>

#include <DGuiApplicationHelper>
#include <DStyle>

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

NotificationsWidget::NotificationsWidget(QWidget *parent)
    : QWidget(parent)
    , m_disturb(false)
    , m_hover(false)
    , m_pressed(false)
{
    setMouseTracking(true);
    setMinimumSize(PLUGIN_BACKGROUND_MIN_SIZE, PLUGIN_BACKGROUND_MIN_SIZE);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [ = ] {
        update();
    });
}

void NotificationsWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QString iconName = "";
    iconName = m_disturb ? "dnd-notification":"notification";

    QPixmap pixmap;
    int iconSize = PLUGIN_ICON_MAX_SIZE;

    QPainter painter(this);
    if (std::min(width(), height()) > PLUGIN_BACKGROUND_MIN_SIZE) {
        QColor color;
        if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
            color = Qt::black;
            painter.setOpacity(0.5);

            if (m_hover) {
                painter.setOpacity(0.6);
            }

            if (m_pressed) {
                painter.setOpacity(0.3);
            }
        } else {
            color = Qt::white;
            painter.setOpacity(0.1);

            if (m_hover) {
                painter.setOpacity(0.2);
            }

            if (m_pressed) {
                painter.setOpacity(0.05);
            }
        }

        painter.setRenderHint(QPainter::Antialiasing, true);

        DStyleHelper dstyle(style());
        const int radius = dstyle.pixelMetric(DStyle::PM_FrameRadius);

        QPainterPath path;

        int minSize = std::min(width(), height());
        QRect rc(0, 0, minSize, minSize);
        rc.moveTo(rect().center() - rc.center());

        path.addRoundedRect(rc, radius, radius);
        painter.fillPath(path, color);
    } else if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        iconName.append(PLUGIN_MIN_ICON_NAME);
    }

    const auto ratio = devicePixelRatioF();
    painter.setOpacity(1);
    pixmap = loadSvg(iconName, ":/icons/resources/icons/", iconSize, ratio);

    const QRectF &rf = QRectF(rect());
    const QRectF &rfp = QRectF(pixmap.rect());

    painter.drawPixmap(rf.center() - rfp.center() / ratio, pixmap);
}

void NotificationsWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void NotificationsWidget::mousePressEvent(QMouseEvent *event)
{
    if (containCursorPos()) {
        m_pressed = true;
    } else {
        m_pressed = false;
    }

    update();

    QWidget::mousePressEvent(event);
}

void NotificationsWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
    m_hover = false;
    update();

    QWidget::mouseReleaseEvent(event);
}

void NotificationsWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (containCursorPos()) {
        m_hover = true;
    } else {
        m_hover = false;
    }

    QWidget::mouseMoveEvent(event);
}

void NotificationsWidget::leaveEvent(QEvent *event)
{
    m_hover = false;
    m_pressed = false;
    update();

    QWidget::leaveEvent(event);
}

const QPixmap NotificationsWidget::loadSvg(const QString &iconName, const QString &localPath, const int size, const qreal ratio)
{
    QIcon icon = QIcon::fromTheme(iconName);
    if (!icon.isNull()) {
        QPixmap pixmap = icon.pixmap(int(size * ratio));
        pixmap.setDevicePixelRatio(ratio);
        return pixmap;
    }

    QPixmap pixmap(int(size * ratio), int(size * ratio));
    QString localIcon = QString("%1%2%3").arg(localPath).arg(iconName).arg(iconName.contains(".svg") ? "" : ".svg");
    QSvgRenderer renderer(localIcon);
    pixmap.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&pixmap);
    renderer.render(&painter);
    painter.end();
    pixmap.setDevicePixelRatio(ratio);

    return pixmap;
}

bool NotificationsWidget::containCursorPos()
{
    QPoint cursorPos = this->mapFromGlobal(QCursor::pos());
    QRect rect(this->rect());

    int iconSize = qMin(rect.width(), rect.height());
    int w = (rect.width() - iconSize) / 2;
    int h = (rect.height() - iconSize) / 2;

    rect = rect.adjusted(w, h, -w, -h);

    return rect.contains(cursorPos);
}

void NotificationsWidget::setDisturb(bool OK)
{
    m_disturb = OK;
    update();
}

