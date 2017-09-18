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

#include "indicatorprovider.h"

IndicatorProvider::IndicatorProvider(QObject *parent)
    : AbstractOSDProvider(parent)
{
    m_suitableParams << "CapsLockOn" << "CapsLockOff" << "NumLockOn" << "NumLockOff";
    m_suitableParams << "TouchpadOn" << "TouchpadOff";
}

int IndicatorProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant IndicatorProvider::data(const QModelIndex &, int) const
{
    if (m_param == "CapsLockOn") {
        return ":/icons/OSD_caps_lock_on.svg";
    } else if (m_param == "CapsLockOff") {
        return ":/icons/OSD_caps_lock_off.svg";
    } else if (m_param == "NumLockOn") {
        return ":/icons/OSD_num_lock_on.svg";
    } else if (m_param == "NumLockOff") {
        return ":/icons/OSD_num_lock_off.svg";
    } else if (m_param == "TouchpadOn") {
        return ":/icons/OSD_trackpad_on.svg";
    } else if (m_param == "TouchpadOff") {
        return ":/icons/OSD_trackpad_off.svg";
    }

    return "";
}

void IndicatorProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant pixData = index.data(Qt::DecorationRole);
    DrawHelper::DrawImage(painter, option, pixData.toString());
}

QSize IndicatorProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

bool IndicatorProvider::match(const QString &param)
{
    m_param = param;
    return m_suitableParams.contains(param);
}
