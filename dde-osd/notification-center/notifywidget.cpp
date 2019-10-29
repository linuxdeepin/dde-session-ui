/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     listenerri <190771752ri@gmail.com>
 *
 * Maintainer: listenerri <190771752ri@gmail.com>
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

#include "notifywidget.h"
#include "appgroupmodel.h"
#include "appgroupdelegate.h"
#include "notification/persistence.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>

NotifyWidget::NotifyWidget(QWidget *parent, Persistence *database)
    : QWidget(parent)
{
    initView(database);

    QVBoxLayout *mainVBLayout = new QVBoxLayout;
    m_noNotify = new QLabel(tr("No system notifications"));

    m_noNotify->setAlignment(Qt::AlignCenter);
    m_noNotify->setVisible(false);

    mainVBLayout->addWidget(m_notifyView);
    mainVBLayout->addWidget(m_noNotify);

    mainVBLayout->setSpacing(10);
    mainVBLayout->setMargin(0);
    mainVBLayout->setContentsMargins(0, 0, 0, 0);

    setContentsMargins(0, 0, 0, 0);
    setLayout(mainVBLayout);
}

void NotifyWidget::initView(Persistence *database)
{
    m_groupModel = new AppGroupModel(this, database);
    m_groupDelegate = new AppGroupDelegate;
    m_notifyView = new QListView;

    m_groupModel->setView(m_notifyView);
    m_notifyView->setModel(m_groupModel);
    m_notifyView->setItemDelegate(m_groupDelegate);
    m_notifyView->setAutoFillBackground(true);

    m_notifyView->setFrameStyle(QFrame::NoFrame);
    m_notifyView->setMouseTracking(true);
    m_notifyView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_notifyView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_notifyView->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_notifyView->setSpacing(0);
    m_notifyView->setContentsMargins(0, 0, 0, 0);
    m_notifyView->setUpdatesEnabled(true);
    m_notifyView->setContentsMargins(0, 0, 0, 0);
    m_notifyView->setUpdatesEnabled(true);
}

