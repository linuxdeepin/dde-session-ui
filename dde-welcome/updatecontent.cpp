/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
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

#include "updatecontent.h"
#include <QHBoxLayout>
#include <QEvent>
#include <QLabel>
#include <QApplication>
#include <DHiDPIHelper>

DWIDGET_USE_NAMESPACE

UpdateContent::UpdateContent(const QString &version, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    QLabel *successIcon = new QLabel;
    successIcon->setPixmap(DHiDPIHelper::loadNxPixmap(":/resources/success.svg"));

    QLabel *successTip = new QLabel(tr("Welcome, system updated successfully"));
    QLabel *currentVersion = new QLabel(tr("Current Edition:") + " " + version);
    m_enterBtn = new QPushButton(tr("Enter"), this);
    m_enterBtn->setMinimumWidth(200);

    mainLayout->addStretch();
    mainLayout->addWidget(successIcon, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(successTip, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(25);
    mainLayout->addWidget(currentVersion, 0, Qt::AlignHCenter);
    mainLayout->addStretch();

    m_enterBtn->setStyleSheet("QPushButton {"
                              "background: rgba(255, 255, 255, 0.2);"
                              "border-radius:5px;"
                              "outline: none;"
                              "color: white;"
                              "}"
                              "QPushButton:hover {"
                              "background: rgba(255, 255, 255, 0.3);"
                              "border-radius:5px;"
                              "outline: none;"
                              "color: white;"
                              "}"
                              "QPushButton:pressed {"
                              "background: rgba(255, 255, 255, 0.1);"
                              "border-radius:5px;"
                              "outline: none;"
                              "color: #2ca7f8;"
                              "}");

    successTip->setStyleSheet("font-size: 36px;"
                              "color: white;"
                              "font-weight: 50;");

    currentVersion->setStyleSheet("font-size: 20px;"
                                  "color: white;"
                                  "font-weight: 40;");

    setLayout(mainLayout);

    connect(m_enterBtn, &QPushButton::clicked, qApp, &QApplication::exit);
}

void UpdateContent::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    const QRect &r = rect();
    m_enterBtn->move((r.width() - m_enterBtn->width()) / 2, r.height() - 180);
}

