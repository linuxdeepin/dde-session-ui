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

#include "delegate.h"

#include <QPainter>
#include <QDebug>

#include "common.h"
#include "abstractosdprovider.h"

Delegate::Delegate(QWidget *parent)
    : QStyledItemDelegate(parent),
      m_provider(nullptr)
{

}

void Delegate::setProvider(AbstractOSDProvider *provider)
{
    m_provider = provider;
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);

    if (m_provider) {
        m_provider->paint(painter, option, index);
    }
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (m_provider)
        return m_provider->sizeHint(option, index);
    return QStyledItemDelegate::sizeHint(option, index);
}
