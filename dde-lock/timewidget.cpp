/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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

#include "timewidget.h"

#include <QVBoxLayout>
#include <QDateTime>
#include <QFontDatabase>
#include <QSettings>

TimeWidget::TimeWidget(QWidget *parent)
    : QWidget(parent)
{
    int id = QFontDatabase::addApplicationFont(":/fonts/fonts/MavenProLight-200.otf");
    const QString fontFamily = QFontDatabase::applicationFontFamilies(id).first();
    const QFont timeFont(fontFamily);

    m_timeLabel = new QLabel;
    m_timeLabel->setFont(timeFont);
    m_timeLabel->setAlignment(Qt::AlignLeft);
    m_timeLabel->setStyleSheet("color:white;"
                               "font-size:68px;");
    m_dateLabel = new QLabel;
    m_dateLabel->setAlignment(Qt::AlignLeft);
    m_dateLabel->setStyleSheet("color:white;"
                               "padding:3px 0;"
                               "font-size:16px;");
    refreshTime();

    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(1000);
    m_refreshTimer->start();

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(m_timeLabel);
    vLayout->addWidget(m_dateLabel);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(48, 0, 0, 0);

    setLayout(vLayout);

    connect(m_refreshTimer, &QTimer::timeout, this, &TimeWidget::refreshTime);
}

void TimeWidget::refreshTime()
{
    m_timeLabel->setText(QDateTime::currentDateTime().toString(tr("hh:mm")));
    m_dateLabel->setText(QDateTime::currentDateTime().toString(tr("yyyy-MM-dd dddd")));
}

