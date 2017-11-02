/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
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

#include "inhibitwarnview.h"

#include <QHBoxLayout>

InhibitWarnView::InhibitWarnView(QWidget *parent)
    : QWidget(parent)
{
    m_acceptBtn = new DImageButton;
    m_cancelBtn = new DImageButton;
    m_reasonLbl = new QLabel;
    m_acceptLbl = new QLabel;
    m_acceptLbl->setAlignment(Qt::AlignCenter);

    m_cancelBtn->setNormalPic(":/img/cancel_normal.svg");
    m_cancelBtn->setHoverPic(":/img/cancel_hover.svg");
    m_cancelBtn->setPressPic(":/img/cancel_press.svg");

    m_reasonLbl->setText("The reason of inhibit.");
    m_reasonLbl->setAlignment(Qt::AlignCenter);
    m_reasonLbl->setStyleSheet("color:white;");

    QLabel *cancenLbl = new QLabel;
    cancenLbl->setAlignment(Qt::AlignCenter);
    cancenLbl->setText(tr("Cancel"));
    cancenLbl->setStyleSheet("color:white;");

    m_acceptLbl->setStyleSheet("color:white;");

    QVBoxLayout *cancelLayout = new QVBoxLayout;
    cancelLayout->addWidget(m_cancelBtn);
    cancelLayout->addWidget(cancenLbl);

    QVBoxLayout *acceptLayout = new QVBoxLayout;
    acceptLayout->addWidget(m_acceptBtn);
    acceptLayout->addWidget(m_acceptLbl);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addLayout(cancelLayout);
    btnsLayout->addSpacing(20);
    btnsLayout->addLayout(acceptLayout);
    btnsLayout->addStretch();

    QVBoxLayout *centeralLayout = new QVBoxLayout;
    centeralLayout->addStretch();
    centeralLayout->addWidget(m_reasonLbl);
    centeralLayout->addSpacing(20);
    centeralLayout->addLayout(btnsLayout);
    centeralLayout->addStretch();

    setLayout(centeralLayout);
    setFixedSize(500, 300);

    connect(m_cancelBtn, &DImageButton::clicked, this, &InhibitWarnView::cancelled);
    connect(m_acceptBtn, &DImageButton::clicked, [this] {emit actionInvoked(m_action);});
}

void InhibitWarnView::setInhibitReason(const QString &reason)
{
    m_reasonLbl->setText(reason);
}

void InhibitWarnView::setAcceptReason(const QString &reason)
{
    m_acceptLbl->setText(reason);
}

void InhibitWarnView::setAction(const Actions action)
{
    m_action = action;

    switch (action) {
    case Actions::Shutdown:
        m_acceptBtn->setNormalPic(":/img/poweroff_warning_normal.svg");
        m_acceptBtn->setHoverPic(":/img/poweroff_warning_hover.svg");
        m_acceptBtn->setPressPic(":/img/poweroff_warning_press.svg");
        break;
    case Actions::Logout:
        m_acceptBtn->setNormalPic(":/img/logout_warning_normal.svg");
        m_acceptBtn->setHoverPic(":/img/logout_warning_hover.svg");
        m_acceptBtn->setPressPic(":/img/logout_warning_press.svg");
        break;
    default:
        m_acceptBtn->setNormalPic(":/img/reboot_warning_normal.svg");
        m_acceptBtn->setHoverPic(":/img/reboot_warning_hover.svg");
        m_acceptBtn->setPressPic(":/img/reboot_warning_press.svg");
        break;
    }
}

void InhibitWarnView::setAcceptVisible(const bool acceptable)
{
    m_acceptBtn->setVisible(acceptable);
    m_acceptLbl->setVisible(acceptable);
}
