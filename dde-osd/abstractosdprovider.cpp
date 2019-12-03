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

#include "abstractosdprovider.h"

AbstractOSDProvider::AbstractOSDProvider(QObject *parent)
    : QObject(parent)
{

}

bool AbstractOSDProvider::match(const QString &param)
{
    return m_suitableParams.contains(param);
}

bool AbstractOSDProvider::checkConditions() const
{
    return true;
}

QSize AbstractOSDProvider::contentSize() const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

QMargins AbstractOSDProvider::contentMargins() const
{
    return QMargins();
}

QListView::Flow AbstractOSDProvider::flow() const
{
    return QListView::TopToBottom;
}

void AbstractOSDProvider::highlightCurrent()
{

}

void AbstractOSDProvider::highlightNext()
{

}

void AbstractOSDProvider::sync()
{

}

void AbstractOSDProvider::sync(const QModelIndex &index)
{
    Q_UNUSED(index);
}

int AbstractOSDProvider::currentRow()
{
    return -1;
}
