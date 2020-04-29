/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei_cm@deepin.com>
 *
 * Maintainer: chenwei <chenwei_cm@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bubbleitem.h"
#include "notification/notificationentity.h"
#include "notification/appicon.h"
#include "notification/appbody.h"
#include "notification/actionbutton.h"
#include "notification/button.h"
#include "notification/icondata.h"
#include "notification/bubbletool.h"
#include "notification/iconbutton.h"
#include "notifymodel.h"
#include "notifylistview.h"

#include <QTimer>
#include <QDateTime>
#include <DStyleHelper>
#include <QPropertyAnimation>
#include <DGuiApplicationHelper>
#include <DFontSizeManager>
#include <QDebug>
#include <QProcess>
#include <QMouseEvent>

AlphaWidget::AlphaWidget(QWidget *parent)
    : DWidget(parent)
{
}

void AlphaWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPalette pe = this->palette();
    QColor brushColor(pe.color(QPalette::Base));
    brushColor.setAlpha(m_hasFocus ? m_hoverAlpha : m_unHoverAlpha);
    painter.setBrush(brushColor);

    DStyleHelper dstyle(style());
    int radius = dstyle.pixelMetric(DStyle::PM_FrameRadius);

    QPen borderPen;
    borderPen.setColor(Qt::transparent);
    painter.setPen(borderPen);

    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);

    painter.drawRoundedRect(rect, radius, radius);
    return DWidget::paintEvent(event);
}

BubbleItem::BubbleItem(QWidget *parent, EntityPtr entity)
    : QWidget(parent)
    , m_entity(entity)
    , m_bgWidget(new AlphaWidget(this))
    , m_titleWidget(new AlphaWidget(this))
    , m_bodyWidget(new AlphaWidget(this))
    , m_appNameLabel(new DLabel(this))
    , m_appTimeLabel(new DLabel(this))
    , m_icon(new AppIcon(this))
    , m_body(new AppBody(this))
    , m_actionButton(new ActionButton(this, OSD::BUBBLEWIDGET))
    , m_closeButton(new IconButton(this))

{
    initUI();
    initContent();
}

BubbleItem::~BubbleItem()
{
}

void BubbleItem::initUI()
{
    setWindowFlags(Qt::Widget);
    setFocusPolicy(Qt::StrongFocus);
    resize(OSD::BubbleSize(OSD::BUBBLEWIDGET));
    m_icon->setFixedSize(OSD::IconSize(OSD::BUBBLEWIDGET));
    m_closeButton->setFixedSize(OSD::CloseButtonSize(OSD::BUBBLEWIDGET));
    m_closeButton->setRadius(OSD::CloseButtonSize(OSD::BUBBLEWIDGET).height());
    m_closeButton->setVisible(false);

    m_titleWidget->setFixedHeight(37);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setSpacing(10);
    titleLayout->setContentsMargins(10, 0, 10, 0);
    titleLayout->addWidget(m_icon);
    titleLayout->addWidget(m_appNameLabel);
    titleLayout->addWidget(m_appTimeLabel);

    m_appNameLabel->setForegroundRole(DPalette::TextTitle);
    m_appNameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_appNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_appTimeLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_actionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setAlpha(Notify::BubbleDefaultAlpha);
    titleLayout->addWidget(m_closeButton);
    m_titleWidget->setLayout(titleLayout);
    mainLayout->addWidget(m_titleWidget);
    m_body->setStyle(OSD::BUBBLEWIDGET);

    QHBoxLayout *bodyLayout = new QHBoxLayout;
    bodyLayout->setSpacing(0);
    bodyLayout->setContentsMargins(10, 0, 10, 0);
    bodyLayout->addWidget(m_body);
    bodyLayout->addWidget(m_actionButton);

    m_bodyWidget->setLayout(bodyLayout);
    mainLayout->addWidget(m_bodyWidget);
    m_bgWidget->setLayout(mainLayout);

    QHBoxLayout *l = new QHBoxLayout;
    l->setSpacing(0);
    l->setMargin(0);
    l->addWidget(m_bgWidget);
    setLayout(l);

    BubbleTool::processIconData(m_icon, m_entity);
    m_defaultAction = BubbleTool::processActions(m_actionButton, m_entity->actions());
}

void BubbleItem::initContent()
{
    if (m_entity == nullptr) return;

    m_body->setTitle(m_entity->summary());
    m_body->setText(OSD::removeHTML(m_entity->body()));
    m_appNameLabel->setText(BubbleTool::getDeepinAppName(m_entity->appName()));
    onRefreshTime();

    connect(m_actionButton, &ActionButton::buttonClicked, this, [ = ](const QString & id) {
        BubbleTool::actionInvoke(id, m_entity);

        if (m_model != nullptr)
            onCloseBubble();
    });

    connect(this, &BubbleItem::havorStateChanged, this, &BubbleItem::onHavorStateChanged);
    connect(m_closeButton, &IconButton::clicked, this, &BubbleItem::onCloseBubble);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &BubbleItem::refreshTheme);
    refreshTheme();
}

void BubbleItem::setAlpha(int alpha)
{
    m_titleWidget->setAlpha(alpha);
    m_bodyWidget->setAlpha(0);
    m_bgWidget->setHoverAlpha(alpha * 5);
    m_bgWidget->setUnHoverAlpha(alpha * 3);
}

void BubbleItem::onRefreshTime()
{
    qint64 msec = QDateTime::currentMSecsSinceEpoch() - m_entity->ctime().toLongLong();
    if (msec < 0) {
        return;
    }

    QString text;

    QDateTime bubbleDateTime = QDateTime::fromMSecsSinceEpoch(m_entity->ctime().toLongLong());
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int elapsedDay = int(bubbleDateTime.daysTo(currentDateTime));
    int minute = int(msec / 1000 / 60);

    if (elapsedDay == 0) {
        if (minute == 0) {
            text =  tr("Just Now");
        } else if (minute > 0 && minute < 60) {
            text = tr("%1 minutes ago").arg(minute);
        } else {
            text = tr("%1 hours ago").arg(minute / 60);
        }
    } else if (elapsedDay >= 1 && elapsedDay < 2) {
        text = tr("Yesterday ") + " " + bubbleDateTime.toString("hh:mm");
    } else if (elapsedDay >= 2 && elapsedDay < 7) {
        text = bubbleDateTime.toString("ddd hh:mm");
    } else {
        text = bubbleDateTime.toString("yyyy/MM/dd");
    }
    m_appTimeLabel->setText(text);
}

void BubbleItem::mousePressEvent(QMouseEvent *event)
{
    m_pressPoint = event->pos();

    return DWidget::mousePressEvent(event);
}

void BubbleItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_pressPoint == event->pos()) {
        if(!m_defaultAction.isEmpty())
        {
            BubbleTool::actionInvoke(m_defaultAction, m_entity);
            m_defaultAction.clear();

            if (m_model != nullptr)
                onCloseBubble();
        }
    }
    DWidget::mouseReleaseEvent(event);
}

void BubbleItem::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return) {
        if(m_defaultAction.isNull()) {
            return DWidget::keyPressEvent(event);
        } else {
            BubbleTool::actionInvoke(m_defaultAction, m_entity);
            m_defaultAction.clear();
            if (m_model != nullptr)
                onCloseBubble();
        }
    }
    return DWidget::keyPressEvent(event);
}

void BubbleItem::enterEvent(QEvent *event)
{
    setFocus();
    Q_EMIT havorStateChanged(true);
    return DWidget::enterEvent(event);
}

void BubbleItem::leaveEvent(QEvent *event)
{
    Q_EMIT havorStateChanged(false);
    return DWidget::leaveEvent(event);
}

void BubbleItem::focusInEvent(QFocusEvent *event)
{
    m_bgWidget->setHasFocus(true);
    Q_EMIT havorStateChanged(true);
    return DWidget::focusInEvent(event);
}

void BubbleItem::focusOutEvent(QFocusEvent *event)
{
    m_bgWidget->setHasFocus(false);
    Q_EMIT havorStateChanged(false);
    return DWidget::focusOutEvent(event);
}

void BubbleItem::onHavorStateChanged(bool hover)
{
    if (m_showContent) {
        m_closeButton->setVisible(hover);
        m_appTimeLabel->setVisible(!hover);

        if (!hover) {
            m_closeButton->setFocusState(false);
        }
    }
}

void BubbleItem::onCloseBubble()
{
    QPropertyAnimation *rightMoveAni = new QPropertyAnimation(this, "pos", this);
    rightMoveAni->setStartValue(this->pos());
    rightMoveAni->setEndValue(this->pos() + QPoint(OSD::BubbleWidth(OSD::ShowStyle::BUBBLEWIDGET), 0));
    rightMoveAni->setDuration(AnimationTime);
    rightMoveAni->start(QPropertyAnimation::DeleteWhenStopped);
    m_view->setAniState(true);
    if (m_model->isExpand(m_entity->appName()))
        m_view->createRemoveAnimation(m_indexRow);

    QTimer::singleShot(AnimationTime + 10, this, [ = ] {
        m_view->setAniState(false);
        if (m_model != nullptr)
            m_model->removeNotify(m_entity);
    });
}

void BubbleItem::setParentModel(NotifyModel *model)
{
    Q_ASSERT(model);
    m_model = model;
}

void BubbleItem::setParentView(NotifyListView *view)
{
    m_view = view;
    connect(m_view, &NotifyListView::refreshItemTime, this, &BubbleItem::onRefreshTime);
}

void BubbleItem::refreshTheme()
{
    m_appNameLabel->setForegroundRole(QPalette::BrightText);
    m_appTimeLabel->setFont(DFontSizeManager::instance()->t8());
}

QList<QPointer<QWidget>> BubbleItem::bubbleElements()
{
    QList<QPointer<QWidget>> bubble_elements;
    bubble_elements.append(m_closeButton); m_actionButton->buttonList();
    foreach (auto btn, m_actionButton->buttonList()) {
        bubble_elements.append(btn);
    }
    return bubble_elements;
}

void BubbleItem::setIndexRow(int row)
{
    m_indexRow = row;
}

void BubbleItem::setHasFocus(bool focus)
{
    m_bgWidget->setHasFocus(focus);
    Q_EMIT havorStateChanged(focus);
}
