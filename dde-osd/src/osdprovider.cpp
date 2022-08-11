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
#include <QApplication>

DGUI_USE_NAMESPACE

OSDProvider::OSDProvider(QObject *parent)
    : AbstractOSDProvider(parent)
{
    initParamsAndSrc();
}

int OSDProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant OSDProvider::data(const QModelIndex &, int role) const
{
    if (m_suitableParamsMap.contains(m_param)) {
        QStringList value = m_suitableParamsMap.value(m_param);
        return role == Qt::DecorationRole ? value.at(0) : value.at(1);
    }

    qDebug() << "Unable to recognize osd type!";
    return "Unable to recognize osd type";
}

void OSDProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString iconName = index.data(Qt::DecorationRole).toString();
    QString textData = index.data(Qt::DisplayRole).toString();

    QColor color;
    bool isLightTheme = false;
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        color = QColor(Qt::black);
        isLightTheme = true;
    }
    else
        color = QColor(Qt::white);

    DrawHelper::DrawImage(painter, option, iconName, isLightTheme);

    if (!textData.isEmpty()) {
        drawText(painter, option, textData, color);
    }
}

QSize OSDProvider::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    return QSize(option.rect.width(), ImageTextItemHeight);
}

QSize OSDProvider::contentSize() const
{
    const int leftMargin = 68;
    const int rightMargin = 26;

    //根据文本长度自适应窗口宽度
    QString textData = data(QModelIndex(), Qt::DisplayRole).toString();

    // TODO 后续需要更换具体字体
    QFont font;
    font.setPixelSize(22);

    QFontMetrics fontMetrics(font);
    return QSize(fontMetrics.horizontalAdvance(textData) + leftMargin + rightMargin, ImageTextItemHeight);
}

bool OSDProvider::match(const QString &param)
{
    m_param = param;
    return m_suitableParamsMap.contains(param);
}

void OSDProvider::drawText(QPainter *painter, const QStyleOptionViewItem &option, const QString &text, const QColor &color) const
{
    const int leftMargin = 68;
    const int topMargin = 13;
    const int height = 30;

    QRect txtRect(option.rect.left() + leftMargin, option.rect.top() + topMargin,
                  option.rect.width() - leftMargin, height);

    QTextOption opt;
    opt.setAlignment(Qt::AlignBottom);

    QFont font;
    font.setPixelSize(22);

    painter->setFont(font);
    painter->setPen(color);
    painter->drawText(txtRect, text, opt);
}

void OSDProvider::initParamsAndSrc()
{
    m_suitableParamsMap["WLANOn"] = QStringList() << "osd_wifi_on" << tr("wifi on");
    m_suitableParamsMap["WLANOff"] = QStringList() << "osd_wifi_off" << tr("wifi off");

    m_suitableParamsMap["CapsLockOn"] = QStringList() << "osd_capslock_on" << tr("uppercase"); // 大写
    m_suitableParamsMap["CapsLockOff"] = QStringList() << "osd_capslock_off" << tr("lowercase"); // 小写

    m_suitableParamsMap["NumLockOn"] = QStringList() << "osd_keyboard_on" << tr("number keyboard on"); // 数字键盘开启
    m_suitableParamsMap["NumLockOff"] = QStringList() << "osd_keyboard_off" << tr("number keyboard off"); // 数字键盘关闭

    m_suitableParamsMap["TouchpadOn"] = QStringList() << "osd_touchpad_on" << tr("touchpad on"); // 触摸板开启
    m_suitableParamsMap["TouchpadOff"] = QStringList() << "osd_touchpad_off" << tr("touchpad off"); // 触摸板关闭
    m_suitableParamsMap["TouchpadToggle"] = QStringList() << "osd_touchpad_exchange" << tr("touchpad toggle"); // 触控板切换

    m_suitableParamsMap["FnToggle"] = QStringList() << "osd_fn" << tr("Fn toggle"); // Fn切换

    m_suitableParamsMap["AirplaneModeOn"] = QStringList() << "osd_airplane_mode_on" << tr("airplane mode on");
    m_suitableParamsMap["AirplaneModeOff"] = QStringList() << "osd_airplane_mode_off" << tr("airplane mode off");

    m_suitableParamsMap["AudioMicMuteOn"] = QStringList() << "osd_mic_on" << tr("audio mic mute on");
    m_suitableParamsMap["AudioMicMuteOff"] = QStringList() << "osd_mic_off" << tr("audio mic mute off");

    m_suitableParamsMap["balance"] = QStringList() << "osd_power_balance" << tr("power balance");
    m_suitableParamsMap["powersave"] = QStringList() << "osd_power_save" << tr("power save");
    m_suitableParamsMap["performance"] = QStringList() << "osd_power_performance" << tr("power performance");

    m_suitableParamsMap["SwitchWM3D"] = QStringList() << "osd_wm_3d" << tr("Window effect enabled");
    m_suitableParamsMap["SwitchWM2D"] = QStringList() << "osd_wm_2d" << tr("Window effect disabled");
    m_suitableParamsMap["SwitchWMError"] = QStringList() << "osd_wm_failed" << tr("Failed to enable window effects");
}
