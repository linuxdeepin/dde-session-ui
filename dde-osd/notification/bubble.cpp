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

Bubble::Bubble(QWidget *parent, std::shared_ptr<NotificationEntity> entity, OSD::ShowStyle style)
    : DBlurEffectWidget(parent)
    , m_entity(entity)
    , m_icon(new AppIcon(this))
    , m_body(new AppBody(this))
    , m_actionButton(new ActionButton(this))
    , m_closeButton(new DDialogCloseButton(this))
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
//    entity->setActions(actions);
//    entity->setTimeout("0");
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
        m_icon->hide();
        m_body->hide();
    } else {
        m_actionButton->setVisible(!m_actionButton->isEmpty());
        m_icon->show();
        m_body->show();
    }
}

void Bubble::mousePressEvent(QMouseEvent *event)
{
    if (!m_enabled) {
        m_dismissAnimation->start();
        return;
    }

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
    } else if (m_pressed && mapToGlobal(event->pos()).y() < 10) {
        Q_EMIT notProcessedYet(this);

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
    if (!m_enabled)
        return;

    if (m_canClose) {
        m_closeButton->setVisible(true);
    }

    return DBlurEffectWidget::enterEvent(event);
}

void Bubble::leaveEvent(QEvent *event)
{
    if (!m_enabled)
        return;

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

        Q_EMIT notProcessedYet(this);
    }
}

void Bubble::onOutAnimFinished()
{
    if (m_entity) {
        Q_EMIT expired(this);
    }
}

void Bubble::onDismissAnimFinished()
{
    if (m_entity) {
        Q_EMIT dismissed(this);
    }
}

void Bubble::initUI()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool);

    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setMaskColor(DBlurEffectWidget::AutoColor);
    setMouseTracking(true);

    setFixedSize(OSD::BubbleSize(OSD::BUBBLEWINDOW));
    m_icon->setFixedSize(OSD::IconSize(OSD::BUBBLEWINDOW));
    m_body->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_closeButton->setIconSize(OSD::CloseButtonSize(OSD::BUBBLEWINDOW));
    m_closeButton->setVisible(false);
    m_actionButton->setVisible(false);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(BubbleSpacing);
    layout->setMargin(BubblePadding);
    layout->addWidget(m_icon);
    layout->addWidget(m_body);
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

    connect(m_closeButton, &DDialogCloseButton::clicked, this, [ = ]() {
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
}

void Bubble::initAnimations()
{
    m_outAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_outAnimation->setDuration(500);
    m_outAnimation->setEasingCurve(QEasingCurve::Linear);
    if (m_outAnimation->state() != QPropertyAnimation::Running) {
        m_outAnimation->setStartValue(1);
        m_outAnimation->setEndValue(0);
    }
    connect(m_outAnimation, &QPropertyAnimation::finished, this, &Bubble::onOutAnimFinished);

    m_dismissAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_dismissAnimation->setDuration(300);
    m_dismissAnimation->setEasingCurve(QEasingCurve::Linear);
    if (m_dismissAnimation->state() != QPropertyAnimation::Running) {
        m_dismissAnimation->setStartValue(1);
        m_dismissAnimation->setEndValue(0);
    }
    connect(m_dismissAnimation, &QPropertyAnimation::finished, this, &Bubble::onDismissAnimFinished);

    m_moveAnimation = new QPropertyAnimation(this, "geometry", this);
//    m_moveAnimation->setDuration(300);
    m_moveAnimation->setEasingCurve(QEasingCurve::Linear);
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

void Bubble::updateContent()
{
    m_body->setTitle(m_entity->summary());
    m_body->setText(OSD::removeHTML(m_entity->body()));
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
        m_moveAnimation->stop();
        m_moveAnimation->start();
    }
    m_moveAnimation->setStartValue(startRect);
    m_moveAnimation->setEndValue(endRect);

    //calc animation time 72pix/300ms
    int ySpace = ABS(endRect.y() - startRect.y());
    m_moveAnimation->setDuration(int(ySpace * 1.0 / 72 * 300));
    m_moveAnimation->start();

    setEnabled(QSize(endRect.width(), endRect.height()) == OSD::BubbleSize(OSD::BUBBLEWINDOW));
}

void Bubble::setFixedGeometry(QRect rect)
{
    setFixedSize(rect.width(), rect.height());
    setGeometry(rect);
}
