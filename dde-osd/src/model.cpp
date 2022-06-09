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

#include "model.h"

#include <QDebug>

#include "abstractosdprovider.h"

Model::Model(QObject *parent)
    : QAbstractListModel(parent),
      m_provider(nullptr)
{

}

int Model::rowCount(const QModelIndex &parent) const
{
    if (!m_provider) return 0;

    return m_provider->rowCount(parent);
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!m_provider) return QVariant();

    return m_provider->data(index, role);
}

void Model::setProvider(AbstractOSDProvider *provider)
{
    beginResetModel();
    m_provider = provider;
    endResetModel();
}

AbstractOSDProvider *Model::provider()
{
    return m_provider;
}
