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

//
// Created by hualet on 11/22/16.
//

#include "errortooltip.h"

ErrorTooltip::ErrorTooltip(QString errorMessage, QWidget *parent) :
    DArrowRectangle(DArrowRectangle::ArrowTop, parent),
    m_label(new QLabel(errorMessage))
{
    m_label->setStyleSheet("padding: 5px 10px; color: #f9704f");
    m_label->adjustSize();
    m_label->setWordWrap(true);
    setContent(m_label);
    setBackgroundColor(Qt::white);
    setBorderColor(Qt::white);
}

void ErrorTooltip::setMessage(QString message) {
    m_label->setText(message);
    m_label->adjustSize();
    setContent(m_label);
}
