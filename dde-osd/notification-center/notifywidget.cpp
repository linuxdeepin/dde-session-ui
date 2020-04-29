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
#include <QScroller>
#include <QLabel>

#include "itemdelegate.h"
#include "notifymodel.h"

NotifyWidget::NotifyWidget(QWidget *parent, Persistence *database)
    : QWidget(parent)
{
    initView(database);

    QVBoxLayout *mainVBLayout = new QVBoxLayout;
    m_noNotify = new QLabel(tr("No system notifications"));

    m_noNotify->setAlignment(Qt::AlignCenter);
    m_noNotify->setVisible(false);

    mainVBLayout->addWidget(m_mainList);
    mainVBLayout->addWidget(m_noNotify);
    mainVBLayout->setMargin(0);
    setLayout(mainVBLayout);
}

void NotifyWidget::initView(Persistence *database)
{
    m_mainList = new NotifyListView(this);

    m_model = new NotifyModel(this,database);
    m_model->setView(m_mainList);
    m_mainList->setModel(m_model);
    m_mainList->setItemDelegate(new ItemDelegate(m_mainList ,m_model, this));
    m_mainList->setAutoFillBackground(false);
    m_mainList->viewport()->setAutoFillBackground(false);
    m_mainList->setFrameStyle(QFrame::NoFrame);
    m_mainList->setMouseTracking(true);
    m_mainList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_mainList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_mainList->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_mainList->setContentsMargins(0, 0, 0, 0);
    m_mainList->setUpdatesEnabled(true);
    m_mainList->setSelectionMode(QListView::NoSelection);
    m_mainList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_mainList->setFocusPolicy(Qt::NoFocus);

    QPalette pa = m_mainList->palette();
    pa.setColor(QPalette::Highlight, Qt::transparent);
    m_mainList->setPalette(pa);
}

void NotifyWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    m_model->resetModel();
}

void NotifyWidget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    m_model->collapseData();
}
