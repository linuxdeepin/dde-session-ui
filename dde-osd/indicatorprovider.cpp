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

#include "indicatorprovider.h"

#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

IndicatorProvider::IndicatorProvider(QObject *parent)
    : AbstractOSDProvider(parent)
{
    m_dats["CapsLockOn"] = ":/icons/OSD_caps_lock_on.svg";
    m_dats["CapsLockOff"] = ":/icons/OSD_caps_lock_off.svg";
    m_dats["NumLockOn"] = ":/icons/OSD_num_lock_on.svg";
    m_dats["NumLockOff"] = ":/icons/OSD_num_lock_off.svg";
    m_dats["TouchpadOn"] = ":/icons/OSD_trackpad_on.svg";
    m_dats["TouchpadOff"] = ":/icons/OSD_trackpad_off.svg";
    m_dats["TouchpadToggle"] = ":/icons/OSD_trackpad_toggle.svg";
    m_dats["FnToggle"] = ":/icons/OSD_Fn_toggle.svg";
    m_dats["AirplaneModeOn"] = ":/icons/OSD_airplane_mode_on.svg";
    m_dats["AirplaneModeOff"] = ":/icons/OSD_airplane_mode_off.svg";
    m_dats["balance"] = ":/icons/OSD-power-balance.svg";
    m_dats["powersave"] = ":/icons/OSD-power-powersave.svg";
    m_dats["performance"] = ":/icons/OSD-power-performance.svg";
    m_dats["KbLightClose"] = ":/icons/OSD_keyboard_backlight_close.svg";
    m_dats["KbLightAuto"] = ":/icons/OSD_keyboard_backlight_auto.svg";
    m_dats["KbLightLow"] = ":/icons/OSD_keyboard_backlight_low.svg";
    m_dats["KbLightHigh"] = ":/icons/OSD_keyboard_backlight_high.svg";
}

int IndicatorProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant IndicatorProvider::data(const QModelIndex &, int) const
{
    return m_dats[m_param];
}

void IndicatorProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant pixData = index.data(Qt::DecorationRole);
    QString iconPath;
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        iconPath = pixData.toString();
    } else {
        iconPath = QString(pixData.toString()).replace(".svg", "_dark.svg");
    }
    DrawHelper::DrawImage(painter, option, iconPath);
}

QSize IndicatorProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

bool IndicatorProvider::match(const QString &param)
{
    m_param = param;
    return m_dats.keys().contains(param);
}
