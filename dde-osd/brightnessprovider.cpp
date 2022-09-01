// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "brightnessprovider.h"

#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

BrightnessProvider::BrightnessProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_displayInter(new com::deepin::daemon::Display("com.deepin.daemon.Display",
                                                      "/com/deepin/daemon/Display",
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
        return ":/icons/OSD_light.svg";
    }

    return m_displayInter->brightness().value(m_displayInter->primary(), 0);
}

void BrightnessProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant imageData = index.data(Qt::DecorationRole);
    QVariant progressData = index.data(Qt::DisplayRole);

    QString iconPath;
    QColor color;
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        iconPath = imageData.toString();
        color = QColor(Qt::black);
    } else {
        iconPath = QString(imageData.toString()).replace(".svg", "_dark.svg");
        color = QColor(Qt::white);
    }
    DrawHelper::DrawImage(painter, option, iconPath, false, true);
    if (!m_isVisible) {
        color.setAlpha(0.4 * 255);
    }
    DrawHelper::DrawProgressBar(painter, option, progressData.toDouble(), color);
}

QSize BrightnessProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}
