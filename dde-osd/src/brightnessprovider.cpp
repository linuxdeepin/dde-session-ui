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

#include "brightnessprovider.h"

#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

BrightnessProvider::BrightnessProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_displayInter(new org::deepin::dde::Display1("org.deepin.dde.Display1",
                                                      "/org/deepin/dde/Display1",
                                                      QDBusConnection::sessionBus(), this))
{
    m_suitableParams << "BrightnessUp" << "BrightnessDown" << "BrightnessUpAsh" << "BrightnessDownAsh";

    m_displayInter->setSync(true, false);
}

int BrightnessProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant BrightnessProvider::data(const QModelIndex &, int role) const
{
    if (role == Qt::DecorationRole) {
        return pixmapName();
    }

    return m_displayInter->brightness().value(m_displayInter->primary(), 0);
}

void BrightnessProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString pixmapName = index.data(Qt::DecorationRole).toString();
    QVariant progressData = index.data(Qt::DisplayRole);

    QColor color;
    bool isLightTheme = false;
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        isLightTheme = true;
        color = QColor(Qt::black);
    } else {
        color = QColor(Qt::white);
    }

    DrawHelper::DrawImage(painter, option, pixmapName, isLightTheme);

    if (!m_isVisible) {
        color.setAlpha(0.4 * 255);
    }
    DrawHelper::DrawProgressBar(painter, option, progressData.toDouble(), color);

    int value = progressData.toDouble() * 100;
    DrawHelper::DrawPercentValue(painter, option, value, color);
}

QSize BrightnessProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

QString BrightnessProvider::pixmapName() const
{
    double brightness = m_displayInter->brightness().value(m_displayInter->primary(), 0);

    QString level = "0";
    if (brightness > 0 && brightness <= 0.33)
        level = "33";
    else if (brightness > 0.33 && brightness <= 0.66)
        level = "66";
    else if (brightness > 0.66 && brightness <= 1)
        level = "100";

    return QString("osd_brightness%1").arg(level);
}
