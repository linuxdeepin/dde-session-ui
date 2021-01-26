/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
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

#include "notifycenterwidget.h"
#include "notification/persistence.h"
#include "notification/constants.h"
#include "notification/iconbutton.h"

#include <QDesktopWidget>
#include <QBoxLayout>
#include <QDBusInterface>
#include <QPalette>
#include <QDebug>
#include <QTimer>
#include <QScreen>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#include <DIconButton>
#include <DLabel>
#include <DFontSizeManager>
#include <DGuiApplicationHelper>

DWIDGET_USE_NAMESPACE

NotifyCenterWidget::NotifyCenterWidget(Persistence *database, QWidget *parent)
    : DBlurEffectWidget(parent)
    , m_notifyWidget(new NotifyWidget(this, database))
    , m_xAni(new QPropertyAnimation(this, "x"))
    , m_widthAni(new QPropertyAnimation(this, "width"))
    , m_aniGroup(new QSequentialAnimationGroup(this))
    , m_wmHelper(DWindowManagerHelper::instance())
    , m_regionMonitor(new DRegionMonitor(this))
{
    initUI();
    initConnections();
    initAnimations();

    CompositeChanged();

    m_regionMonitor->setCoordinateType(DRegionMonitor::Original);
}

void NotifyCenterWidget::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(0, 0, 0, 0);

    m_headWidget = new QWidget;
    m_headWidget->setFixedSize(Notify::CenterWidth - 2 * Notify::CenterMargin, 32);

    DIconButton *bell_notify = new DIconButton(m_headWidget);
    bell_notify->setFlat(true);
    bell_notify->setIconSize(QSize(Notify::CenterTitleHeight, Notify::CenterTitleHeight));
    bell_notify->setFixedSize(Notify::CenterTitleHeight, Notify::CenterTitleHeight);
    const auto ratio = devicePixelRatioF();
    QIcon icon_pix = QIcon::fromTheme("://icons/notifications.svg").pixmap(bell_notify->iconSize() * ratio);
    bell_notify->setIcon(icon_pix);
    bell_notify->setFocusPolicy(Qt::NoFocus);

    title_label = new DLabel;
    QFont font = title_label->font();
    font.setBold(true);
    font.setWeight(QFont::DemiBold);
    title_label->setFont(font);
    title_label->setText(tr("Notification Center"));
    title_label->setAlignment(Qt::AlignCenter);
    title_label->setForegroundRole(QPalette::BrightText);
    DFontSizeManager::instance()->bind(title_label, DFontSizeManager::T5);

    m_clearButton = new IconButton;
    m_clearButton->setOpacity(IconButton::RELEASE, 255 * 0.0);
    m_clearButton->setRadius(Notify::CenterTitleHeight / 2);
    m_clearButton->setFixedSize(Notify::CenterTitleHeight, Notify::CenterTitleHeight);
    m_clearButton->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout *head_Layout = new QHBoxLayout;
    head_Layout->addWidget(bell_notify, Qt::AlignLeft);
    head_Layout->setMargin(0);
    head_Layout->addStretch();
    head_Layout->addWidget(title_label, Qt::AlignCenter);
    head_Layout->addStretch();
    head_Layout->addWidget(m_clearButton, Qt::AlignRight);
    m_headWidget->setLayout(head_Layout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(Notify::CenterMargin, Notify::CenterMargin, 0, 0);
    mainLayout->addWidget(m_headWidget);
    mainLayout->addWidget(m_notifyWidget);

    setLayout(mainLayout);

    connect(m_clearButton, &IconButton::clicked, this, [ = ]() {
        m_notifyWidget->model()->removeAllData();
    });

    refreshTheme();
}

void NotifyCenterWidget::initAnimations()
{
    m_xAni->setEasingCurve(QEasingCurve::Linear);
    m_xAni->setDuration(AnimationTime / 2);

    m_widthAni->setEasingCurve(QEasingCurve::Linear);
    m_widthAni->setDuration(AnimationTime);

    m_aniGroup->addAnimation(m_xAni);
    m_aniGroup->addAnimation(m_widthAni);
}

void NotifyCenterWidget::initConnections()
{
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &NotifyCenterWidget::refreshTheme);

    connect(m_widthAni, &QVariantAnimation::valueChanged, this, [ = ](const QVariant & value) {
        int width = value.toInt();
        move(m_notifyRect.x() + m_notifyRect.width() + Notify::CenterMargin - width, m_notifyRect.y());
    });

    connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged, this, &NotifyCenterWidget::CompositeChanged, Qt::QueuedConnection);
}

void NotifyCenterWidget::updateGeometry(QRect screen, QRect dock, OSD::DockPosition pos, int mode)
{
    m_scale = qApp->primaryScreen()->devicePixelRatio();
    m_dockRect = dock;

    int width = Notify::CenterWidth;
    int height = screen.height() - Notify::CenterMargin * 2;
    if (pos == OSD::DockPosition::Top || pos == OSD::DockPosition::Bottom) {
        if(mode == OSD::DockModel::Fashion) {
            height = screen.height() - Notify::CenterMargin * 2 - dock.height();
            if (dock.height() != 0) {
                height -= OSD::DockMargin * 2;
            }
        } else {
            height = screen.height() - Notify::CenterMargin * 2 - dock.height();
        }
    }

    int x = screen.x() + screen.width() - Notify::CenterWidth - Notify::CenterMargin;
    if (pos == OSD::DockPosition::Right) {
        if(mode == OSD::DockModel::Fashion) {
            x =  screen.x() + screen.width() - (Notify::CenterWidth + dock.width() + OSD::DockMargin * 2 + Notify::CenterMargin);
            if (dock.width() == 0) {
                x += OSD::DockMargin * 2;
            }
        } else {
            x =  screen.x() + screen.width() - (Notify::CenterWidth + dock.width() + Notify::CenterMargin);
        }
    }

    int y = screen.y() + Notify::CenterMargin;
    if (pos == OSD::DockPosition::Top) {
        if(mode == OSD::DockModel::Fashion) {
            y = screen.y() + Notify::CenterMargin + dock.height() + OSD::DockMargin * 2;
        } else {
            y = screen.y() + Notify::CenterMargin + dock.height();
        }
    }

    m_notifyRect = QRect(x, y, width, height);
    setGeometry(m_notifyRect);

    m_notifyWidget->setFixedWidth(m_notifyRect.width() - Notify::CenterMargin);
    setFixedSize(m_notifyRect.size());

    m_xAni->setStartValue(m_notifyRect.x());
    m_xAni->setEndValue(m_notifyRect.x() + Notify::CenterMargin);

    m_widthAni->setStartValue(int(m_notifyRect.width()));
    m_widthAni->setEndValue(0);
}

void NotifyCenterWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    return; 
}

void NotifyCenterWidget::hideEvent(QHideEvent *event)
{
    unRegisterRegion();

    return DBlurEffectWidget::hideEvent(event);
}

void NotifyCenterWidget::refreshTheme()
{
    QPalette pa = title_label->palette();
    pa.setBrush(QPalette::WindowText, pa.brightText());
    title_label->setPalette(pa);
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        m_clearButton->setIcon("://icons/list_icon_clear.svg");
        m_clearButton->setOpacity(IconButton::HOVER, 255 * 0.2);
        m_clearButton->setOpacity(IconButton::PRESS, 255 * 0.3);
    } else {
        m_clearButton->setIcon("://icons/list_icon_clear_dark.svg");
        m_clearButton->setOpacity(IconButton::HOVER, 255 * 0.2);
        m_clearButton->setOpacity(IconButton::PRESS, 255 * 0.1);
    }
}

void NotifyCenterWidget::showAni()
{
    registerRegion();

    if (!m_hasComposite) {
        setGeometry(QRect(m_notifyRect.x(), m_notifyRect.y(), m_notifyRect.width(), m_notifyRect.height()));
        setFixedSize(m_notifyRect.size());
        show();
        activateWindow();
        return;
    }
    setFixedWidth(0);
    move(m_notifyRect.x() + m_notifyRect.width(), m_notifyRect.y());
    show();
    activateWindow();

    m_aniGroup->setDirection(QAbstractAnimation::Backward);
    m_aniGroup->start();
}

void NotifyCenterWidget::hideAni()
{
    if (!m_hasComposite) {
        hide();
        return;
    }

    m_aniGroup->setDirection(QAbstractAnimation::Forward);
    m_aniGroup->start();

    QTimer::singleShot(m_aniGroup->duration(), this, [ = ] {
        setVisible(false);
    });
}

void NotifyCenterWidget::registerRegion()
{
    QDBusInterface interface("com.deepin.api.XEventMonitor", "/com/deepin/api/XEventMonitor",
                             "com.deepin.api.XEventMonitor",
                             QDBusConnection::sessionBus());
    if (interface.isValid()) {
        m_regionConnect = connect(m_regionMonitor, &DRegionMonitor::buttonPress, this, [ = ](const QPoint & p, const int flag) {
            Q_UNUSED(flag);
            QPoint pScale(int(qreal(p.x() / m_scale)), int(qreal(p.y() / m_scale)));
            if (!geometry().contains(pScale))
                if (!isHidden()) {
                    hideAni();
                }
        });
    }

    if (!m_regionMonitor->registered()) {
        m_regionMonitor->registerRegion();
    }
}

void NotifyCenterWidget::unRegisterRegion()
{
    if (m_regionMonitor->registered()) {
        m_regionMonitor->unregisterRegion();
        disconnect(m_regionConnect);
    }
}

void NotifyCenterWidget::CompositeChanged()
{
    m_hasComposite = m_wmHelper->hasComposite();
}

void NotifyCenterWidget::setX(int x)
{
    move(x, m_notifyRect.y());
}

void NotifyCenterWidget::showWidget()
{
    if (m_aniGroup->state() == QAbstractAnimation::Running)
        return;

    return isHidden() ? showAni() : hideAni();
}
