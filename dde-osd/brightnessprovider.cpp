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

BrightnessProvider::BrightnessProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_initDisplayTimer(new QTimer(this)),
      m_brightness(0),
      m_tryLimit(0),
      m_displayInter(new com::deepin::daemon::Display("com.deepin.daemon.Display",
                                                      "/com/deepin/daemon/Display",
                                                      QDBusConnection::sessionBus(), this))
{
    m_suitableParams << "BrightnessUp" << "BrightnessDown";

    m_displayInter->setSync(false);
    connect(m_displayInter, &__Display::BrightnessChanged,
            this, &BrightnessProvider::brightnessChanged);
    connect(m_displayInter, &__Display::PrimaryChanged,
            this, &BrightnessProvider::primaryChanged);

    m_initDisplayTimer->setInterval(10000);
    m_initDisplayTimer->setSingleShot(true);
    connect(m_initDisplayTimer, &QTimer::timeout, this, &BrightnessProvider::tryGetDbusData);
    m_initDisplayTimer->start();
}

int BrightnessProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant BrightnessProvider::data(const QModelIndex &, int role) const
{
    if (role == Qt::DecorationRole) {
        return ":/icons/OSD_light.svg";
    }
    return m_brightness;
}

void BrightnessProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant imageData = index.data(Qt::DecorationRole);
    QVariant progressData = index.data(Qt::DisplayRole);

    DrawHelper::DrawImage(painter, option, imageData.toString(), false, true);
    DrawHelper::DrawProgressBar(painter, option, progressData.toDouble());
}

QSize BrightnessProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

void BrightnessProvider::setBrightness(double brightness)
{
    if (brightness == m_brightness) return;

    m_brightness = brightness;
    emit dataChanged();
}

void BrightnessProvider::brightnessChanged(const BrightnessMap &brightness)
{
    if (brightness.size() == 0) return;

    m_brightnessMap = brightness;
    setBrightness(brightness.value(m_primaryScreen, 0));
}

void BrightnessProvider::primaryChanged(const QString &primary)
{
    if (primary == m_primaryScreen) return;

    m_primaryScreen = primary;
    setBrightness(m_brightnessMap.value(primary, 0));
}

void BrightnessProvider::tryGetDbusData()
{
     m_tryLimit++;
    if (m_tryLimit > 10) {
        qDebug() << "init display dbus timeout...";
        return;
    }

    if (m_brightnessMap.isEmpty() || m_primaryScreen.isEmpty()) {
        brightnessChanged(m_displayInter->brightness());
        primaryChanged(m_displayInter->primary());
        m_initDisplayTimer->start();
        return;
    }

    m_initDisplayTimer->deleteLater();
}
