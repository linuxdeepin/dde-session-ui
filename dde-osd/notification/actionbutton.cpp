/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: fanpengcheng <fanpengcheng_cm@deepin.com>
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

#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QtMath>
#include <QDebug>
#include <QStyle>
#include <QMenu>
#include <QAction>
#include <QGridLayout>

#include "actionbutton.h"
#include "button.h"

//#include <DPushButton>

static const QColor BorderColor = QColor::fromRgbF(0, 0, 0, 0.1);
static const QColor TextColor = QColor(0, 135, 255);
static const QColor TextHover = Qt::white;

ActionButton::ActionButton(QWidget *parent) :
    QFrame(parent)
    , m_closeButton(new Button(this))
    , m_menuButton(new Button())
{
    initUI();
    initConnections();
}

bool ActionButton::addButtons(const QStringList &list)
{
    if (list.isEmpty()) {
        m_closeButton->show();
        return true;
    }

    // Each even element in the list (starting at index 0) represents the
    // identifier for the action. Each odd element in the list is the
    // localized string that will be displayed to the user.
    QString id;

    for (int i = 0; i != list.size(); ++i) {
        //先获取action ID
        if (i % 2 == 0) {
            id = list[i];
        } else {
            if (i == 1) {
                Button *button = new Button();
                button->setText(list[i]);
                button->setRadius(15);
                button->setFixedSize(BUTTONWIDTH, BUTTONHEIGHT);

                m_layout->addWidget(button);

                connect(button, &Button::clicked, this, [ = ] {
                    emit buttonClicked(id);
                });

                m_buttons << button;
            } else if (i == 3) {
                m_menuButton->setText(list[i]);
                m_menuButton->setId(id);
            } else {
                QAction *action = new QAction(list[i]);

                connect(action, &QAction::triggered, this, [ = ] {
                    emit buttonClicked(id);
                });

                m_menuButton->addAction(action);
            }
        }
    }

    if (list.size() > 2) {
        m_menuButton->show();
    }

    return true;
}

bool ActionButton::isEmpty()
{
    return m_buttons.isEmpty();
}

void ActionButton::clear()
{
    for (QWidget *w : m_buttons) {
        m_layout->removeWidget(w);
        w->deleteLater();
    }

    m_buttons.clear();

    m_menuButton->clear();
    m_menuButton->hide();

    m_closeButton->hide();
}

void ActionButton::initUI()
{
    setObjectName("ActionButton");
    m_closeButton->setFixedSize(CLOSEBTNSIZE);
    m_closeButton->setRadius(99);
    m_closeButton->setText("X");
    m_menuButton->setFixedSize(BUTTONWIDTH, BUTTONHEIGHT);
    m_menuButton->setRadius(20);

    m_layout = new QHBoxLayout;
    m_layout->setMargin(0);
    m_layout->setSpacing(10);
    m_layout->setAlignment(Qt::AlignRight);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(10);
    layout->setMargin(0);
    layout->addLayout(m_layout);
    layout->addWidget(m_menuButton);
    layout->addWidget(m_closeButton);

    setLayout(layout);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void ActionButton::initConnections()
{
    connect(this, &ActionButton::expired, m_menuButton, &Button::hideMenu);
    connect(this, &ActionButton::dismissed, m_menuButton, &Button::hideMenu);
    connect(this, &ActionButton::replacedByOther, m_menuButton, &Button::hideMenu);
    connect(m_closeButton, &Button::clicked, this, &ActionButton::closeButtonClicked);
    connect(m_menuButton, &Button::toggled, this, &ActionButton::buttonClicked);
}
