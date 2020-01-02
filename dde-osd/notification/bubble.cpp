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
#include <QParallelAnimationGroup>

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
<<<<<<< HEAD
    , m_showStyle(style)
    , m_tranAnimation(new QVariantAnimation(this))
    , m_opacityAnimation(new QVariantAnimation(this))
=======
>>>>>>> feat：format code
{
    initUI();
    initAnimations();
    initTimers();
    initConnections();

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
    entity->setTimeout("0");
#endif

    m_outTimer->stop();

    updateContent();

    int timeout = entity->timeout().toInt();
    //  0: never times out
    // -1: default 5s
    m_outTimer->setInterval(timeout == -1 ? BubbleTimeout : (timeout == 0 ? -1 : timeout));
    m_outTimer->setSingleShot(true);
    m_outTimer->start();
}

void Bubble::setEnabled(bool enable)
{
    if (!enable) {
        m_actionButton->hide();
        m_icon->hide();
        m_body->hide();
    } else {
        m_actionButton->setVisible(!m_actionButton->isEmpty());
        m_icon->show();
        m_body->show();
    }

    DWidget::setEnabled(enable);
}

void Bubble::updateGeometry()
{
    if (m_geometryAnimation->state() == QVariantAnimation::Running)
        m_geometryAnimation->stop();

//    if (m_opacityAnimation->state() == QVariantAnimation::Running)
//        m_opacityAnimation->stop();

    m_outTimer->stop();
    m_outTimer->setSingleShot(false);
    m_outTimer->start();
}

void Bubble::mousePressEvent(QMouseEvent *event)
{
<<<<<<< HEAD
    if (!m_enabled) {
=======
    if (!isEnabled()) {
        Q_EMIT dismissed(this);
>>>>>>> feat：format code
        return;
    }

    if (event->button() == Qt::LeftButton) {
        m_clickPos = event->pos();
        m_pressed = true;
        m_isDelete = true;
    }
}

void Bubble::mouseReleaseEvent(QMouseEvent *event)
{
    if (!isEnabled())
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

//    m_opacityAnimation->stop();
    m_quitTimer->start();
}

void Bubble::enterEvent(QEvent *event)
{
    if (!isEnabled())
        return;

    if (m_canClose) {
        m_closeButton->setVisible(true);
    }

    return DBlurEffectWidget::enterEvent(event);
}

void Bubble::leaveEvent(QEvent *event)
{
    if (!isEnabled())
        return;

    if (m_canClose) {
        m_closeButton->setVisible(false);
    }

    return DBlurEffectWidget::leaveEvent(event);
}

void Bubble::closeEvent(QCloseEvent *event)
{
    QPropertyAnimation *opacityAni = new QPropertyAnimation(this, "windowOpacity", this);
    QPropertyAnimation *geometryAni = new QPropertyAnimation(this, "geometry", this);
    opacityAni->setStartValue(1);
    opacityAni->setEndValue(0);
    geometryAni->setStartValue(geometry());

    QRect rect;
    rect.setX(this->y());
    rect.setY(-60);//TODO
    rect.setSize(geometry().size());
    geometryAni->setEndValue(rect);

    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    group->addAnimation(opacityAni);
    group->addAnimation(geometryAni);

    group->start(QAbstractAnimation::DeleteWhenStopped);

    QTimer::singleShot(group->duration(), this, [ = ] {
        DBlurEffectWidget::closeEvent(event);
    });
}

void Bubble::onOutTimerTimeout()
{
    if (containsMouse()) {
        m_outTimer->stop();
        m_outTimer->setSingleShot(true);
        m_outTimer->start();
    } else {
//        m_opacityAnimation->start();

        Q_EMIT notProcessedYet(this);
        close();
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
        Q_EMIT actionInvoked(this, action_id);

        Q_EMIT dismissed(this);
<<<<<<< HEAD
        Q_EMIT actionInvoked(this, action_id);
=======
>>>>>>> feat：format code
    });

    connect(m_closeButton, &DDialogCloseButton::clicked, this, [ = ]() {
        Q_EMIT dismissed(this);
    });

    connect(m_quitTimer, &QTimer::timeout, this, &Bubble::onDelayQuit);

//    connect(m_opacityAnimation, &QPropertyAnimation::finished, this, &Bubble::onOutAnimFinished);
    connect(m_geometryAnimation, &QPropertyAnimation::finished, this, [ = ] {
        m_outTimer->start();
    });

    connect(m_outTimer, &QTimer::timeout, this, &Bubble::onOutTimerTimeout);
}

void Bubble::initAnimations()
{
<<<<<<< HEAD
    m_outAnimation->setDuration(AnimationTime);
    m_outAnimation->setEasingCurve(QEasingCurve::Linear);
    if (m_outAnimation->state() != QPropertyAnimation::Running) {
        m_outAnimation->setStartValue(1);
        m_outAnimation->setEndValue(0);
    }

    m_tranAnimation->setStartValue(1.0);
    m_tranAnimation->setEndValue(0.0);
    m_tranAnimation->setDuration(BubbleDeleteTimeout);
    m_tranAnimation->setEasingCurve(QEasingCurve::InOutCirc);

    connect(m_tranAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant & value) {
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
            if (m_isDelete) this->deleteLater();
        });
    });
=======
//    m_opacityAnimation = new QPropertyAnimation(this, "windowOpacity", this);
//    m_opacityAnimation->setDuration(AnimationTime);
//    m_opacityAnimation->setEasingCurve(QEasingCurve::Linear);
//    m_opacityAnimation->setStartValue(1);
//    m_opacityAnimation->setEndValue(0);

    m_geometryAnimation = new QPropertyAnimation(this, "geometry", this);
    m_geometryAnimation->setEasingCurve(QEasingCurve::Linear);
>>>>>>> feat：format code
}


void Bubble::initTimers()
{
    m_quitTimer->setInterval(60 * 1000);
    m_quitTimer->setSingleShot(true);

    m_outTimer->setInterval(BubbleTimeout);
    m_outTimer->setSingleShot(true);
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

void Bubble::startMove(const QRect &startRect, const QRect &endRect)
{
<<<<<<< HEAD
    m_bubbleIndex = index;

    if (startRect.top() > endRect.top() && m_pressed == true) {
        m_tranAnimation->start();
        m_opacityAnimation->start();
    }

    if (m_moveAnimation->state() != QPropertyAnimation::Running) {
        m_moveAnimation->stop();
        m_moveAnimation->start();
    }

    m_moveAnimation->setStartValue(startRect);
    m_moveAnimation->setEndValue(endRect);
=======
    if (m_geometryAnimation->state() != QPropertyAnimation::Running) {
        m_geometryAnimation->stop();
        m_geometryAnimation->start();
    }
    m_geometryAnimation->setStartValue(startRect);
    m_geometryAnimation->setEndValue(endRect);
>>>>>>> feat：format code

    //calc animation time 72pix/300ms
    int ySpace = ABS(endRect.y() - startRect.y());
    m_geometryAnimation->setDuration(int(ySpace * 1.0 / 72 * AnimationTime));
    m_geometryAnimation->start();

    setEnabled(QSize(endRect.width(), endRect.height()) == OSD::BubbleSize(OSD::BUBBLEWINDOW));
}

void Bubble::setBubbleIndex(int index)
{
    m_bubbleIndex = index;
}

void Bubble::setFixedGeometry(QRect rect)
{
    setFixedSize(rect.width(), rect.height());
    setGeometry(rect);
}
