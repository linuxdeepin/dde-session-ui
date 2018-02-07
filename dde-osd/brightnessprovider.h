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

#ifndef BRIGHTNESSPROVIDER_H
#define BRIGHTNESSPROVIDER_H

#include "osdprovider.h"

#include <com_deepin_daemon_display.h>

#include "common.h"

class BrightnessProvider : public AbstractOSDProvider
{
    Q_OBJECT
public:
    explicit BrightnessProvider(QObject *parent = 0);

    // model
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    // delegate
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setBrightness(double brightness);

private slots:
    void brightnessChanged(const BrightnessMap &brightness);
    void primaryChanged(const QString &primary);

private:
    double m_brightness;

    QString m_primaryScreen;
    BrightnessMap m_brightnessMap;

    com::deepin::daemon::Display *m_displayInter;
};

#endif // BRIGHTNESSPROVIDER_H
