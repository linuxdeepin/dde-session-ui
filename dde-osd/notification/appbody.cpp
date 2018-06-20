/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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

#include "appbody.h"
#include <QPainter>
#include <QVBoxLayout>

AppBody::AppBody(QWidget *parent)
    : QFrame(parent)
{
    m_titleLbl = new appBodyLabel;
    m_bodyLbl = new appBodyLabel;

    m_titleLbl->setStyleSheet("QLabel { font-weight: 460; color: #303030; }");
    m_bodyLbl->setStyleSheet("QLabel { color: rgba(0,0,0,0.9); }");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(2);
    layout->addStretch();
    layout->addWidget(m_titleLbl, 0, Qt::AlignVCenter);
    layout->addWidget(m_bodyLbl, 0, Qt::AlignVCenter);
    layout->addStretch();

    setLayout(layout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void AppBody::setTitle(const QString &title)
{
    m_titleLbl->setText(title);
    m_titleLbl->setVisible(!title.isEmpty());

    updateAlignment();
}

void AppBody::setText(const QString &text)
{
    m_bodyLbl->setText(text);
    m_bodyLbl->setVisible(!text.isEmpty());

    updateAlignment();
}

void AppBody::updateAlignment()
{
    if (!m_titleLbl->isVisible())
        m_bodyLbl->setAlignment(Qt::AlignVCenter);
    else
        m_bodyLbl->setAlignment(Qt::AlignTop);

    if (!m_bodyLbl->isVisible())
        m_titleLbl->setAlignment(Qt::AlignVCenter);
    else
        m_titleLbl->setAlignment(Qt::AlignBottom);
}
