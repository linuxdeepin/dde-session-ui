/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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

#include "appbody.h"
#include "appbodylabel.h"
#include "constants.h"

#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>

#include <DGuiApplicationHelper>
#include <DFontSizeManager>

DGUI_USE_NAMESPACE

AppBody::AppBody(QWidget *parent)
    : QFrame(parent)
    , m_showStyle(OSD::BUBBLEWINDOW)
{
    m_titleLbl = new AppBodyLabel;
    m_bodyLbl = new AppBodyLabel;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(2);
    layout->addStretch();
    layout->addWidget(m_titleLbl, 0, Qt::AlignVCenter);
    layout->addWidget(m_bodyLbl, 0, Qt::AlignVCenter);
    layout->addStretch();

    setLayout(layout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &AppBody::refreshTheme);
    refreshTheme();
}

void AppBody::setTitle(const QString &title)
{
    m_titleLbl->setText(title);
    m_titleLbl->setVisible(!title.isEmpty());

    updateAlignment();
}

void AppBody::setText(const QString &text)
{
    m_bodyLbl->setText(text);
    m_bodyLbl->setVisible(!text.isEmpty());

    updateAlignment();
}

void AppBody::setStyle(OSD::ShowStyle style)
{
    m_showStyle = style;
    refreshTheme();
}

void AppBody::updateAlignment()
{
    if (!m_titleLbl->isVisible())
        m_bodyLbl->setAlignment(Qt::AlignVCenter);
    else
        m_bodyLbl->setAlignment(Qt::AlignTop);

    if (!m_bodyLbl->isVisible())
        m_titleLbl->setAlignment(Qt::AlignVCenter);
    else
        m_titleLbl->setAlignment(Qt::AlignBottom);
}

void AppBody::refreshTheme()
{
    m_titleLbl->setForegroundRole(QPalette::BrightText);
    m_bodyLbl->setForegroundRole(QPalette::BrightText);

    if (m_showStyle == OSD::BUBBLEWIDGET) {
        m_titleLbl->setOpacity(1.0);
        m_bodyLbl->setOpacity(Notify::BubbleOpacity);
    } else {
        m_titleLbl->setOpacity(Notify::BubbleOpacity);
        m_bodyLbl->setOpacity(1.0);
    }

    qreal scale = qApp->primaryScreen()->devicePixelRatio();
    QFont font;
    font.setBold(false);
    int rate = int((scale - 1) / 0.25);
    font.setPixelSize(FontPixelSize::fontT6 + 2 * rate);
    m_titleLbl->setFont(font);

    font.setBold(false);
    font.setPixelSize(FontPixelSize::fontT7 + 2 * rate);
    m_bodyLbl->setFont(font);
}
