/*
 * Copyright (C) 2014 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *             listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *             fanpengcheng <fanpengcheng_cm@deepin.com>
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

#include "bubble.h"
#include "notificationentity.h"
#include "appicon.h"
#include "appbody.h"
#include "actionbutton.h"
#include "button.h"
#include "icondata.h"
#include "bubbletool.h"

#include <QDebug>
#include <QTimer>
#include <QPropertyAnimation>
#include <QApplication>
#include <QGSettings>
#include <QMoveEvent>
#include <QBoxLayout>
#include <DStyleHelper>


Bubble::Bubble(QWidget *parent, std::shared_ptr<NotificationEntity> entity, OSD::ShowStyle style)
    : DBlurEffectWidget(parent)
    , m_entity(entity)
    , m_icon(new AppIcon(this))
    , m_body(new AppBody(this))
    , m_actionButton(new ActionButton(this))
    , m_closeButton(new Button(this))
    , m_outTimer(new QTimer(this))
    , m_quitTimer(new QTimer(this))
    , m_showStyle(style)
{
    initUI();
    initConnections();
    initAnimations();
    initTimers();

    setEntity(entity);

    installEventFilter(this);
}

std::shared_ptr<NotificationEntity> Bubble::entity() const
{
    return m_entity;
}

void Bubble::setEntity(std::shared_ptr<NotificationEntity> entity)
{
    if (!entity) return;

    m_entity = entity;

#ifdef QT_DEBUG
    //模拟通知消息带菜单的情况
    QStringList actions;
    actions << "default";
    actions << "default";
    actions << "查看";
    actions << "查看";
    actions << "删除";
    actions << "删除";
    actions << "取消";
    actions << "取消";
    entity->setActions(actions);
    entity->setTimeout("0");
#endif

    m_outTimer->stop();

    updateContent();

    show();

    int timeout = entity->timeout().toInt();
    //  0: never times out
    // -1: default 5s
    m_outTimer->setInterval(timeout == -1 ? BubbleTimeout : (timeout == 0 ? -1 : timeout));
    m_outTimer->start();
}

void Bubble::setEnabled(bool enable)
{
    m_enabled = enable;

    if (!enable) {
        m_actionButton->hide();
        m_closeButton->hide();
        m_icon->hide();
        m_body->hide();
    } else {
        m_actionButton->show();
//        m_closeButton->setVisible(m_canClose);
        m_icon->show();
        m_body->show();
    }
}

void Bubble::StartMoveIn(int x, int y)
{
    if (m_moveAnimation->state() != QPropertyAnimation::Running) {
        m_moveAnimation->setStartValue(QRect((x - OSD::BubbleWidth(m_showStyle)) / 2, 0,
                                             OSD::BubbleWidth(OSD::BUBBLEWINDOW),
                                             OSD::BubbleHeight(OSD::BUBBLEWINDOW)));
        m_moveAnimation->setEndValue(QRect((x - OSD::BubbleWidth(m_showStyle)) / 2,
                                           y,
                                           OSD::BubbleWidth(OSD::BUBBLEWINDOW),
                                           OSD::BubbleHeight(OSD::BUBBLEWINDOW)));
        m_moveAnimation->start();
    }
}

void Bubble::StartMoveIn(const QRect &startRect, const QRect &endRect)
{
    if (m_moveAnimation->state() != QPropertyAnimation::Running) {
        m_moveAnimation->setStartValue(startRect);
        m_moveAnimation->setEndValue(endRect);
        m_moveAnimation->start();
    }
}

void Bubble::mousePressEvent(QMouseEvent *event)
{
    if (!m_enabled)
        return;

    if (event->button() == Qt::LeftButton) {
        m_clickPos = event->pos();
        m_pressed = true;
    }

    m_outTimer->stop();
}

void Bubble::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_enabled)
        return;

    if (m_pressed && m_clickPos == event->pos()) {
        if (!m_defaultAction.isEmpty()) {
            Q_EMIT actionInvoked(this, m_defaultAction);
            qDebug() << "actionId:" << m_defaultAction;
            m_defaultAction.clear();
        }
        // FIX ME:采用动画的方式退出并隐藏，不会丢失窗体‘焦点’，造成控件不响应鼠标进入和离开事件
        m_dismissAnimation->start();
    } else if (m_pressed && event->pos().y() < 10) {
        Q_EMIT ignored(this);
        m_dismissAnimation->start();
    } else {
        m_outTimer->start();
    }

    m_pressed = false;

    return DBlurEffectWidget::mouseReleaseEvent(event);
}

bool Bubble::eventFilter(QObject *obj, QEvent *event)
{
    //FIX ME:过滤掉这两个事件，否则窗体上鼠标点击后，在界面之外进行mouseRlease,窗体接收不到mouseReleaseEvent
    if (obj) {
        if (event->type() == QEvent::MouseMove
                || event->type() == QEvent::Move) {
            return true;
        }
    }
    return false;
}

void Bubble::showEvent(QShowEvent *event)
{
    DBlurEffectWidget::showEvent(event);

    m_quitTimer->start();
}

void Bubble::hideEvent(QHideEvent *event)
{
    DBlurEffectWidget::hideEvent(event);

    m_outAnimation->stop();
    m_dismissAnimation->stop();

    m_quitTimer->start();
}

void Bubble::enterEvent(QEvent *event)
{
    if (m_canClose && m_enabled) {
        m_closeButton->setVisible(true);
    }

    return DBlurEffectWidget::enterEvent(event);
}

void Bubble::leaveEvent(QEvent *event)
{
    if (m_canClose) {
        m_closeButton->setVisible(false);
    }

    return DBlurEffectWidget::leaveEvent(event);
}

void Bubble::onOutTimerTimeout()
{
    if (containsMouse()) {
        m_outTimer->stop();
        m_outTimer->start();
    } else {
        m_outAnimation->start();
    }
}

void Bubble::onOutAnimFinished()
{
    // FIXME: There should be no empty pointers here
    if (m_entity) {
        Q_EMIT expired(this);
    }
}

void Bubble::onDismissAnimFinished()
{
    // FIXME: There should be no empty pointers here
    if (m_entity) {
        Q_EMIT dismissed(this);
    }
}

void Bubble::initUI()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    m_wmHelper = DWindowManagerHelper::instance();
    m_handle = new DPlatformWindowHandle(this);
    m_handle->setTranslucentBackground(true);
    m_handle->setShadowOffset(QPoint(0, 4));

    compositeChanged();

    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setMaskColor(DBlurEffectWidget::AutoColor);
    setMouseTracking(true);

    DStyleHelper dstyle(style());
    int radius = dstyle.pixelMetric(DStyle::PM_TopLevelWindowRadius);
    m_handle->setShadowRadius(radius);

    setFixedSize(OSD::BubbleSize(OSD::BUBBLEWINDOW));
    m_icon->setFixedSize(OSD::IconSize(OSD::BUBBLEWINDOW));

    m_closeButton->setFixedSize(OSD::CloseButtonSize(OSD::BUBBLEWINDOW));
    m_closeButton->setRadius(99);
    m_closeButton->setText("X");
    m_closeButton->setVisible(false);
    m_body->setObjectName("Body");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(BubbleSpacing);
    layout->setMargin(BubblePadding);
    layout->addWidget(m_icon);
    layout->addWidget(m_body);
    layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));

    m_actionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_actionButton);
    layout->addWidget(m_closeButton);

    setLayout(layout);

    QTimer::singleShot(0, this, [ = ] {
        // FIXME: 锁屏不允许显示任何通知，而通知又需要禁止窗管进行管理，
        // 为了避免二者的冲突，将气泡修改为dock，保持在其他程序置顶，又不会显示在锁屏之上。
        BubbleTool::register_wm_state(winId());
    });
}

void Bubble::initConnections()
{
    connect(m_actionButton, &ActionButton::buttonClicked, this, [ = ](const QString & action_id) {
        BubbleTool::actionInvoke(action_id, m_entity);

        m_dismissAnimation->start();
        m_outTimer->stop();
        Q_EMIT actionInvoked(this, action_id);
    });

    connect(m_closeButton, &Button::clicked, this, [ = ]() {
        m_dismissAnimation->start();
    });

    connect(this, &Bubble::expired, m_actionButton, [ = ]() {
        m_actionButton->expired(int(m_entity->id()));
    });
    connect(this, &Bubble::dismissed, m_actionButton,  [ = ]() {
        m_actionButton->dismissed(int(m_entity->id()));
    });
    connect(this, &Bubble::replacedByOther, m_actionButton, [ = ]() {
        m_actionButton->replacedByOther(int(m_entity->id()));
    });

    connect(m_quitTimer, &QTimer::timeout, this, &Bubble::onDelayQuit);
    connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged, this, &Bubble::compositeChanged);
}

void Bubble::initAnimations()
{
    m_outAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_outAnimation->setDuration(500);
    m_outAnimation->setEasingCurve(QEasingCurve::OutCubic);
    if (m_outAnimation->state() != QPropertyAnimation::Running) {
        m_outAnimation->setStartValue(1);
        m_outAnimation->setEndValue(0);
    }
    connect(m_outAnimation, &QPropertyAnimation::finished, this, &Bubble::onOutAnimFinished);

    m_dismissAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_dismissAnimation->setDuration(300);
    m_dismissAnimation->setEasingCurve(QEasingCurve::OutCubic);
    if (m_dismissAnimation->state() != QPropertyAnimation::Running) {
        m_dismissAnimation->setStartValue(1);
        m_dismissAnimation->setEndValue(0);
    }
    connect(m_dismissAnimation, &QPropertyAnimation::finished, this, &Bubble::onDismissAnimFinished);

    m_moveAnimation = new QPropertyAnimation(this, "geometry", this);
    m_moveAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

void Bubble::initTimers()
{
    m_quitTimer->setInterval(60 * 1000);
    m_quitTimer->setSingleShot(true);

    m_outTimer->setInterval(BubbleTimeout);
    m_outTimer->setSingleShot(true);
    connect(m_outTimer, &QTimer::timeout, this, &Bubble::onOutTimerTimeout);
}

void Bubble::onDelayQuit()
{
    const QGSettings gsettings("com.deepin.dde.notification", "/com/deepin/dde/notification/");
    if (gsettings.keys().contains("autoExit") && gsettings.get("auto-exit").toBool()) {
        qWarning() << "Killer Timeout, now quiiting...";
        qApp->quit();
    }
}

void Bubble::compositeChanged()
{
    if (!m_wmHelper->hasComposite()) {
        m_handle->setWindowRadius(0);
        m_handle->setShadowColor(QColor("#E5E5E5"));
    } else {
        m_handle->setWindowRadius(5);
        m_handle->setShadowColor(QColor(0, 0, 0, 100));
    }
}

void Bubble::updateContent()
{
    m_body->setTitle(m_entity->summary());
    m_body->setText(m_entity->body());
    m_canClose = m_entity->actions().isEmpty();

    BubbleTool::processIconData(m_icon, m_entity);
    m_defaultAction = BubbleTool::processActions(m_actionButton, m_entity->actions());
}

bool Bubble::containsMouse() const
{
    return geometry().contains(QCursor::pos());
}

void Bubble::startMoveAnimation(const QRect &startRect, const QRect &endRect)
{
    if (m_moveAnimation->state() != QPropertyAnimation::Running) {
        m_moveAnimation->setStartValue(startRect);
        m_moveAnimation->setEndValue(endRect);
        m_moveAnimation->start();
    }
}

BubbleTemplate::BubbleTemplate()
    : DBlurEffectWidget(nullptr)
{
    initUI();

    this->installEventFilter(this);
}

void BubbleTemplate::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    DStyleHelper dstyle(style());
    int radius = dstyle.pixelMetric(DStyle::PM_TopLevelWindowRadius);
    setBlurRectXRadius(radius);
    setBlurRectYRadius(radius);
}

bool BubbleTemplate::eventFilter(QObject *object, QEvent *event)
{
    if (object) {
        if (event->type() == QEvent::Type::MouseButtonPress
                || event->type() == QEvent::Type::MouseButtonRelease
                || event->type() == QEvent::Type::MouseButtonDblClick
                || event->type() == QEvent::Type::MouseMove
                || event->type() == QEvent::Type::Move) {
            return true;
        }
    }

    return false;
}
