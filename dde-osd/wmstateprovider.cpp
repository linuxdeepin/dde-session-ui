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

#include "wmstateprovider.h"

WMStateProvider::WMStateProvider(QObject *parent)
    : AbstractOSDProvider(parent)
{
    m_suitableParams << "SwitchWM3D" << "SwitchWM2D" << "SwitchWMError";
}

int WMStateProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant WMStateProvider::data(const QModelIndex &, int role) const
{
    const bool decoration = role == Qt::DecorationRole;
    if (m_param == "SwitchWM3D") {
        return decoration ? ":/icons/OSD_wm_3d.svg" : tr("Window effect enabled");
    } else if (m_param == "SwitchWM2D") {
        return decoration ? ":/icons/OSD_wm_2d.svg" : tr("Window effect disabled");
    }
    return decoration ? ":/icons/OSD_wm_failed.svg" : tr("Failed to enable window effects");
}

void WMStateProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant pixPath = index.data(Qt::DecorationRole);
    QVariant textData = index.data();

    DrawHelper::DrawImage(painter, option, pixPath.toString(), true);
    DrawHelper::DrawText(painter, option, textData.toString());
}

QSize WMStateProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

bool WMStateProvider::match(const QString &param)
{
    m_param = param;
    return m_suitableParams.contains(param);
}
