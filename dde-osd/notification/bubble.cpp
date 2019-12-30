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
    , m_outAnimation(new QPropertyAnimation(this, "windowOpacity", this))
    , m_moveAnimation(new QPropertyAnimation(this, "geometry", this))
    , m_icon(new AppIcon(this))
    , m_body(new AppBody(this))
    , m_actionButton(new ActionButton(this))
    , m_closeButton(new DDialogCloseButton(this))
    , m_outTimer(new QTimer(this))
    , m_quitTimer(new QTimer(this))
    , m_showStyle(style)
    , m_posAnimation(new QVariantAnimation(this))
    , m_opacityAnimation(new QVariantAnimation(this))
    , m_posAnimationGroup(new QParallelAnimationGroup)
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

//#ifdef QT_DEBUG
//    //模拟通知消息带菜单的情况
//    QStringList actions;
//    actions << "default";
//    actions << "default";
//    actions << "查看";
//    actions << "查看";
//    actions << "删除";
//    actions << "删除";
//    actions << "取消";
//    actions << "取消";
//    //    entity->setActions(actions);
//    entity->setTimeout("0");
//#endif

    m_outTimer->stop();

    updateContent();

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

void Bubble::stopAnimation()
{
    if (m_moveAnimation->state() == QVariantAnimation::Running)
        m_moveAnimation->stop();

    if (m_outAnimation->state() == QVariantAnimation::Running)
        m_outAnimation->stop();
}

void Bubble::startCalcTimeout()
{
    m_outTimer->stop();
    m_outTimer->setSingleShot(false);
    m_outTimer->start();
}

void Bubble::mousePressEvent(QMouseEvent *event)
{
    if (!m_enabled) {
        Q_EMIT dismissed(this);
        return;
    }

    if (event->button() == Qt::LeftButton) {
        m_clickPos = event->pos();
        m_pressed = true;
        m_isDelete = true;
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
        Q_EMIT dismissed(this);
    } else if (m_pressed && mapToGlobal(event->pos()).y() < 10) {
        Q_EMIT notProcessedYet(this);
        Q_EMIT dismissed(this);
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

void Bubble::initUI()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool | Qt::X11BypassWindowManagerHint);
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

        Q_EMIT dismissed(this);
        m_outTimer->stop();
        m_outTimer->start();
        Q_EMIT actionInvoked(this, action_id);
    });

    connect(m_closeButton, &DDialogCloseButton::clicked, this, [ = ]() {
        Q_EMIT dismissed(this);
    });

    connect(m_quitTimer, &QTimer::timeout, this, &Bubble::onDelayQuit);
}

void Bubble::initAnimations()
{
    m_outAnimation->setDuration(AnimationTime);
    m_outAnimation->setEasingCurve(QEasingCurve::Linear);

    m_posAnimation->setStartValue(1.0);
    m_posAnimation->setEndValue(0.0);
    m_posAnimation->setDuration(BubbleDeleteTimeout);
    m_posAnimation->setEasingCurve(QEasingCurve::InOutCirc);

    connect(m_posAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant & value) {

    m_opacityAnimation->setStartValue(1.0);
    m_opacityAnimation->setEndValue(0.0);
    m_opacityAnimation->setDuration(BubbleDeleteTimeout);
    m_opacityAnimation->setEasingCurve(QEasingCurve::InOutCirc);

    connect(m_opacityAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant & value) {
        window()->setWindowOpacity(value.toDouble());
    });

    m_moveAnimation->setEasingCurve(QEasingCurve::Linear);
    connect(m_moveAnimation, &QPropertyAnimation::finished, this, [ = ] {
        m_outTimer->start();
        if(m_isDelete) this->deleteLater();
    });
    m_posAnimationGroup->addAnimation(m_moveAnimation);
    });
}


void Bubble::initTimers()
{
    m_quitTimer->setInterval(60 * 1000);
    m_quitTimer->setSingleShot(true);

    m_outTimer->setInterval(BubbleTimeout);
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
#ifdef QT_DEBUG
    //移除href标签
    auto removeHref = [ = ](const QString & str) {
        QString text = str.simplified();
        int pos = entity()->body().indexOf("<a href");
        int pos2 = entity()->body().indexOf("</a>");
        if (pos != -1 && pos2 != -1) {
            return text.replace(str.mid(pos, pos2 - pos + QString("</a>").length()), "");
        }
        return str;
    };

    m_body->setText(removeHref(m_entity->body()));
#else
    m_body->setText(OSD::removeHTML(m_entity->body()));
#endif
    m_canClose = m_entity->actions().isEmpty();

    BubbleTool::processIconData(m_icon, m_entity);
    m_defaultAction = BubbleTool::processActions(m_actionButton, m_entity->actions());
}

bool Bubble::containsMouse() const
{
    return geometry().contains(QCursor::pos());
}

void Bubble::startMoveAnimation(const QRect &startRect, const QRect &endRect, const int index)
{
    m_bubbleIndex = index;

    if (startRect.top() > endRect.top() && m_pressed == true) {
        m_posAnimation->start();
    }
    m_posAnimationGroup->start();

    if (startRect.top() > endRect.top() && m_isDelete == true) {
        m_opacityAnimation->start();
    }

    if (m_moveAnimation->state() != QPropertyAnimation::Running) {
        m_moveAnimation->stop();
        m_moveAnimation->start();
    }

    m_moveAnimation->setStartValue(startRect);
    m_moveAnimation->setEndValue(endRect);

    //calc animation time 72pix/300ms
    int ySpace = ABS(endRect.y() - startRect.y());
    m_moveAnimation->setDuration(int(ySpace * 1.0 / 72 * AnimationTime));
    m_moveAnimation->start();

    setEnabled(QSize(endRect.width(), endRect.height()) == OSD::BubbleSize(OSD::BUBBLEWINDOW));
}

void Bubble::setFixedGeometry(QRect rect)
{
    setFixedSize(rect.width(), rect.height());
    setGeometry(rect);
}
