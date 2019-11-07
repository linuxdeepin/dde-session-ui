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

#include <QDebug>
#include <QTimer>
#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QApplication>
#include <QProcess>
#include <QGSettings>
#include <QMoveEvent>
#include <QBoxLayout>
#include <QX11Info>
#include <DStyleHelper>

#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

void register_wm_state(WId winid)
{
    xcb_ewmh_connection_t m_ewmh_connection;
    xcb_intern_atom_cookie_t *cookie = xcb_ewmh_init_atoms(QX11Info::connection(), &m_ewmh_connection);
    xcb_ewmh_init_atoms_replies(&m_ewmh_connection, cookie, nullptr);

    xcb_atom_t atoms[2];
    atoms[0] = m_ewmh_connection._NET_WM_WINDOW_TYPE_DOCK;
    atoms[1] = m_ewmh_connection._NET_WM_STATE_BELOW;
    xcb_ewmh_set_wm_window_type(&m_ewmh_connection, winid, 1, atoms);
}

Bubble::Bubble(QWidget *parent, std::shared_ptr<NotificationEntity> entity, OSD::ShowStyle style)
    : BubbleAbStract(parent, entity)
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
#endif

    m_outTimer->stop();

    updateContent();

    show();

    if (m_offScreen) {
        m_offScreen = false;
        m_inAnimation->start();
    }

    int timeout = entity->timeout().toInt();
    //  0: never times out
    // -1: default 5s
    m_outTimer->setInterval(timeout == -1 ? BubbleTimeout : (timeout == 0 ? -1 : timeout));
    m_outTimer->start();
}

void Bubble::setPostion(const QPoint &point)
{
    m_bubblePos = point;
    move(m_bubblePos);
}

void Bubble::setBasePosition(int x, int y, QRect rect)
{
    x -= ScreenPadding;
    y += ScreenPadding;

    m_bubblePos.setX((x - OSD::BubbleWidth(m_showStyle)) / 2);
    m_bubblePos.setY(y);

    if (m_inAnimation->state() == QPropertyAnimation::Running) {
        const int baseX = x - OSD::BubbleWidth(m_showStyle);

        m_inAnimation->setStartValue(QPoint(baseX / 2, y - OSD::BubbleHeight(m_showStyle)));
        m_inAnimation->setEndValue(QPoint(baseX / 2, y));
    }

    if (m_outAnimation->state() != QPropertyAnimation::Running) {
        m_outAnimation->setStartValue(1);
        m_outAnimation->setEndValue(0);
    }

    if (m_dismissAnimation->state() != QPropertyAnimation::Running) {
        m_dismissAnimation->setStartValue(1);
        m_dismissAnimation->setEndValue(0);
    }

    if (!rect.isEmpty())
        m_screenGeometry = rect;
}

void Bubble::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_clickPos = event->pos();
        m_pressed = true;
    }

    m_offScreen = true;
    m_outTimer->stop();
}

void Bubble::mouseReleaseEvent(QMouseEvent *event)
{
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

        //TODO:add ignored animation
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

void Bubble::onActionButtonClicked(const QString &actionId)
{
    qDebug() << "actionId:" << actionId;
    QMap<QString, QVariant> hints = m_entity->hints();
    QMap<QString, QVariant>::const_iterator i = hints.constBegin();
    while (i != hints.constEnd()) {
        QStringList args = i.value().toString().split(",");
        if (!args.isEmpty()) {
            QString cmd = args.first();
            args.removeFirst();
            if (i.key() == "x-deepin-action-" + actionId) {
                QProcess::startDetached(cmd, args);
            }
        }
        ++i;
    }

    m_dismissAnimation->start();
    m_outTimer->stop();
    Q_EMIT actionInvoked(this, actionId);
}

void Bubble::onOutTimerTimeout()
{
    if (containsMouse()) {
        m_outTimer->stop();
        m_outTimer->start();
    } else {
        m_offScreen = true;
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
    m_quitTimer->setInterval(60 * 1000);
    m_quitTimer->setSingleShot(true);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    DStyleHelper dstyle(style());
    int radius = dstyle.pixelMetric(DStyle::PM_TopLevelWindowRadius);
    setBlurRectXRadius(radius);
    setBlurRectYRadius(radius);
    m_handle->setShadowRadius(radius);

    setFixedSize(OSD::BubbleSize(OSD::BUBBLEWINDOW));
    m_icon->setFixedSize(OSD::IconSize(OSD::BUBBLEWINDOW));

    m_closeButton->setFixedSize(OSD::CloseButtonSize(OSD::BUBBLEWINDOW));
    m_closeButton->setRadius(99);
    m_closeButton->setText("X");
    m_closeButton->setVisible(false);

    //layout
    m_appNameLabel->setVisible(false);
    m_appTimeLabel->setVisible(false);

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
        register_wm_state(winId());
    });
}

void Bubble::initConnections()
{
    connect(m_actionButton, &ActionButton::buttonClicked, this, &Bubble::onActionButtonClicked);

    connect(m_closeButton, &Button::clicked, this, [ = ] {
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
    m_inAnimation = new QPropertyAnimation(this, "pos", this);
    m_inAnimation->setDuration(300);
    m_inAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_outAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_outAnimation->setDuration(500);
    m_outAnimation->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_outAnimation, &QPropertyAnimation::finished, this, &Bubble::onOutAnimFinished);

    m_dismissAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_dismissAnimation->setDuration(300);
    m_dismissAnimation->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_dismissAnimation, &QPropertyAnimation::finished, this, &Bubble::onDismissAnimFinished);

    m_moveAnimation = new QPropertyAnimation(this, "pos", this);
    m_moveAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

void Bubble::initTimers()
{
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

void Bubble::startMoveAnimation(const QPoint &point)
{
    if (isVisible() && m_outAnimation->state() != QPropertyAnimation::Running) {
        m_bubblePos = point;
        m_moveAnimation->setStartValue(QPoint(x(), y()));
        m_moveAnimation->setEndValue(m_bubblePos);
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
