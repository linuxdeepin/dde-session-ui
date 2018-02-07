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

#include "deepinwave.h"

#include <QPainter>
#include <QTimer>
#include <QDebug>

DeepinWave::DeepinWave(QWidget *parent) :
    QWidget(parent),
    m_waveTimer(new QTimer(this))
{
    setFixedSize(240, 70);
    setAttribute(Qt::WA_TranslucentBackground);

    m_waveTimer->setInterval(1000 / 30); // 30 fps
    connect(m_waveTimer, &QTimer::timeout, [this]{
        m_count = (++m_count % 124);
        update();
    });
    m_waveTimer->start();
}

void DeepinWave::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QString num; num.sprintf("%03d", m_count);
    QString wavePath = QString(":/dde-offline-upgrader/images/waves/wave_00%1.png").arg(num);
    QPixmap wave(wavePath);
    QPixmap cover(":/dde-offline-upgrader/images/cover.png");
    QPixmap mask(":/dde-offline-upgrader/images/mask.png");

    painter.drawPixmap(rect(), wave, QRect(QPoint(0, height() * m_progress), size()));
    painter.save();
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawPixmap(rect(), mask);
    painter.restore();
    painter.drawPixmap(rect(), cover);

    painter.end();
}

float DeepinWave::progress() const
{
    return m_progress;
}

void DeepinWave::setProgress(const float &progress)
{
    if (progress != m_progress) {
        m_progress = progress;

        update();
    }
}
