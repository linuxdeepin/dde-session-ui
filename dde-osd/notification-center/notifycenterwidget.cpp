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
#include <QApplication>
#include <QDesktopWidget>
#include <QStackedLayout>
#include <diconbutton.h>
#include <DLabel>

DWIDGET_USE_NAMESPACE

NotifyCenterWidget::NotifyCenterWidget(QWidget *parent)
    : DBlurEffectWidget(parent)
    , m_timeRefersh(new QTimer(this))
    , m_contentLayout(new QStackedLayout)
    , m_notifyWidget(new NotifyWidget(this))
{
    initUI();
    initAnimations();

    m_dockSizeInter =  new QDBusInterface("com.deepin.dde.daemon.Dock", "/com/deepin/dde/daemon/Dock",
                                          "com.deepin.dde.daemon.Dock", QDBusConnection::sessionBus(), this);
}

void NotifyCenterWidget::initUI()
{
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_contentLayout->setMargin(0);
    m_contentLayout->setSpacing(0);
    m_contentLayout->addWidget(m_notifyWidget);

    m_timeRefersh->setInterval(1000);
    m_timeRefersh->setSingleShot(false);
    m_timeRefersh->start();


    DIconButton *bell_notify = new DIconButton(this);
    bell_notify->setIcon(QIcon("://icons/notifications.svg"));

    DLabel *title_label = new DLabel(this);
    title_label->setText(tr("Notification Center"));
    title_label->setAlignment(Qt::AlignCenter);

    DIconButton *close_btn = new DIconButton(DStyle::SP_CloseButton);
    connect(close_btn, &DIconButton::clicked, this, [=] (){
        m_outAnimation->start();
    });

    QHBoxLayout *head_Layout = new QHBoxLayout;
    head_Layout->addWidget(bell_notify, Qt::AlignLeft);
    head_Layout->addStretch();
    head_Layout->addWidget(title_label, Qt::AlignCenter);
    head_Layout->addStretch();
    head_Layout->addWidget(close_btn, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(head_Layout);
    mainLayout->addLayout(m_contentLayout);

    setLayout(mainLayout);
}

void NotifyCenterWidget::initAnimations()
{
    m_inAnimation = new QPropertyAnimation(this, "pos", this);
    m_inAnimation->setDuration(300);
    m_inAnimation->setEasingCurve(QEasingCurve::OutCirc);

    m_outAnimation = new QPropertyAnimation(this, "pos", this);
    m_outAnimation->setDuration(300);
    m_outAnimation->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_outAnimation, &QPropertyAnimation::finished, this , &NotifyCenterWidget::hide);
}

void NotifyCenterWidget::updateGeometry()
{
    QDesktopWidget *desktop = QApplication::desktop();
    QRect rect = desktop->screenGeometry();
    int dock_size =  m_dockSizeInter->property("WindowSize").toInt();

    int height = rect.height() - CenterMargin * 2 - dock_size;
    int width = CenterWidth;
    int x = rect.width() - (CenterWidth + CenterMargin);
    int y = rect.y() + CenterMargin;

    if(m_inAnimation->state() != QPropertyAnimation::Running) {
        m_inAnimation->setStartValue(QPoint(rect.width(), y));
        m_inAnimation->setStartValue(QPoint(x, y));
    }

    if(m_outAnimation->state() != QPropertyAnimation::Running) {
        m_outAnimation->setStartValue(QPoint(x, y));
        m_outAnimation->setEndValue(QPoint(rect.width(), y));
    }

    setGeometry(x, y, width, height);
}

void NotifyCenterWidget::showEvent(QShowEvent *event)
{
    updateGeometry();
    m_inAnimation->start();
    DBlurEffectWidget::showEvent(event);
}

void NotifyCenterWidget::hideEvent(QHideEvent *event)
{
    m_outAnimation->stop();
    DBlurEffectWidget::hideEvent(event);
}
