// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "osdprovider.h"

#include <DGuiApplicationHelper>
#include <QApplication>

DGUI_USE_NAMESPACE

const QString SwitchWM3D = "SwitchWM3D";
const QString SwitchWM2D = "SwitchWM2D";
const QString SwitchWMError = "SwitchWMError";
const QString AudioMicMuteOn = "AudioMicMuteOn";
const QString AudioMicMuteOff = "AudioMicMuteOff";
const QString AudioMicMuteOnAsh = "AudioMicMuteOnAsh";
const QString AudioMicMuteOffAsh = "AudioMicMuteOffAsh";
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
    m_suitableParamsMap.insert(AudioMicMuteOnAsh, QStringList() << ":/icons/OSD_mic_off.svg" << "");
    m_suitableParamsMap.insert(AudioMicMuteOffAsh, QStringList() << ":/icons/OSD_mic_off.svg" << "");

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

QSize OSDProvider::contentSize() const
{
    //根据文本长度自适应窗口宽度
    QString textData = OSDProvider::data(QModelIndex(), Qt::DisplayRole).toString();
    QFontMetrics fontMetrics(QApplication::font());
    return QSize(qMax(ImageTextItemWidth,fontMetrics.horizontalAdvance(textData) + 30), ImageTextItemHeight);
}

bool OSDProvider::match(const QString &param)
{
    m_param = param;
    return m_suitableParamsMap.contains(param);
}
