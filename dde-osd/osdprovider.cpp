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

#include "osdprovider.h"

#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

const QString SwitchWM3D = "SwitchWM3D";
const QString SwitchWM2D = "SwitchWM2D";
const QString SwitchWMError = "SwitchWMError";
const QString AudioMicMuteOn = "AudioMicMuteOn";
const QString AudioMicMuteOff = "AudioMicMuteOff";
const QString WLANOn = "WLANOn";
const QString WLANOff = "WLANOff";

OSDProvider::OSDProvider(QObject *parent)
    : AbstractOSDProvider(parent)
{
    m_suitableParams << SwitchWM3D << SwitchWM2D << SwitchWMError;
    m_suitableParams << AudioMicMuteOn << AudioMicMuteOff;
    m_suitableParams << WLANOn << WLANOff;

    m_suitableParamsMap.insert(SwitchWM3D, QStringList() << ":/icons/OSD_wm_3d.svg" << tr("Window effect enabled"));
    m_suitableParamsMap.insert(SwitchWM2D, QStringList() << ":/icons/OSD_wm_2d.svg" << tr("Window effect disabled"));
    m_suitableParamsMap.insert(SwitchWMError, QStringList() << ":/icons/OSD_wm_failed.svg" << tr("Failed to enable window effects"));

    m_suitableParamsMap.insert(AudioMicMuteOn, QStringList() << ":/icons/OSD_mic_off.svg" << "");
    m_suitableParamsMap.insert(AudioMicMuteOff, QStringList() << ":/icons/OSD_mic_on.svg" << "");

    m_suitableParamsMap.insert(WLANOn, QStringList() << ":/icons/OSD_wifi_on.svg" << "");
    m_suitableParamsMap.insert(WLANOff, QStringList() << ":/icons/OSD_wifi_off.svg" << "");
}

int OSDProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant OSDProvider::data(const QModelIndex &, int role) const
{
    const bool decoration = role == Qt::DecorationRole;

    if (m_suitableParamsMap.contains(m_param)) {
        QStringList value = m_suitableParamsMap.value(m_param);
        return decoration ? value.at(0) : value.at(1).toStdString().c_str();
    }

    qDebug() << "Unable to recognize osd type!";
    return "Unable to recognize osd type";
}

void OSDProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString pixPath = index.data(Qt::DecorationRole).toString();
    QString textData = index.data().toString();
    QString iconPath;
    QColor color;
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        iconPath = pixPath;
        color = QColor(Qt::black);
    } else {
        iconPath = QString(pixPath).replace(".svg", "_dark.svg");
        color = QColor(Qt::white);
    }
    DrawHelper::DrawImage(painter, option, iconPath, true);

    if (!textData.isEmpty()) {
        DrawHelper::DrawText(painter, option, textData, color);
    }
}

QSize OSDProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

bool OSDProvider::match(const QString &param)
{
    m_param = param;
    return m_suitableParamsMap.contains(param);
}
