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
#include "notification/persistence.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>

NotifyWidget::NotifyWidget(QWidget *parent, Persistence* database)
    : QWidget(parent),
      m_notifyView(new NotifyView(this, database))
{
    initUI();
}

void NotifyWidget::initUI()
{
    QScrollArea *scroll_area = new QScrollArea(this);
    scroll_area->setWidgetResizable(true);
    scroll_area->horizontalScrollBar()->setEnabled(false);
    scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_area->setWidget(m_notifyView);
    scroll_area->viewport()->setAutoFillBackground(false);
    m_notifyView->setAutoFillBackground(false);

    QVBoxLayout *mainVBLayout = new QVBoxLayout;
    m_noNotify = new QLabel(tr("No system notifications"));

    m_noNotify->setAlignment(Qt::AlignCenter);
    m_noNotify->setVisible(false);

    mainVBLayout->addWidget(scroll_area);
    mainVBLayout->addWidget(m_noNotify);

    mainVBLayout->setSpacing(1);
    mainVBLayout->setMargin(0);
    mainVBLayout->setContentsMargins(0, 0, 0, 0);

    setContentsMargins(0, 0, 0, 0);
    setLayout(mainVBLayout);
}



