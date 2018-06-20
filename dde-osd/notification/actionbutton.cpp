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

#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QtMath>
#include <QDebug>
#include <QStyle>

#include "actionbutton.h"

static const QColor BorderColor = QColor::fromRgbF(0, 0, 0, 0.1);
static const QColor TextColor = QColor(0, 135, 255);
static const QColor TextHover = Qt::white;

ActionButton::ActionButton(QWidget *parent) :
    QFrame(parent)
{
    setObjectName("ActionButton");
    setFixedSize(70, 70);

    m_layout = new QVBoxLayout;
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    setLayout(m_layout);

    setStyleSheet("#ActionButton {"
                  "border: solid rgba(0, 0, 0, 0.1);"
                  "border-left-width: 1px;"
                  "}"
                  "#ActionButton > QPushButton[isHead=true] {"
                  "margin: 0;"
                  "border: none;"
                  "color: rgb(0, 135, 255);"
                  "}"
                  "#ActionButton > QPushButton {"
                  "margin: 0;"
                  "border: solid rgba(0, 0, 0, 0.1);"
                  "border-top-width: 1px;"
                  "color: rgb(0, 135, 255);"
                  "}"
                  "#ActionButton > QPushButton:hover {"
                  "margin: 0;"
                  "border: none;"
                  "color: white;"
                  "background: rgb(0, 135, 255);"
                  "}");
}

bool ActionButton::addButtons(const QStringList &list)
{
    if (list.isEmpty()) return false;

    const int buttonHeight = ((double)height() / (double)list.size()) * 2;

    QString id;

    // Each even element in the list (starting at index 0) represents the
    // identifier for the action. Each odd element in the list is the
    // localized string that will be displayed to the user.
    for (int i = 0; i != list.size(); ++i) {
        if (i % 2 == 0) {
            id = list[i];
        } else {
            Button *button = new Button(list[i]);
            m_buttons << button;

            button->setFixedHeight(buttonHeight);

            connect(button, &Button::clicked, this, [=] {
                emit buttonClicked(id);
            });

            button->setIsHead(false);

            m_layout->addWidget(button);
        }
    }

    m_buttons.first()->setIsHead(true);

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
}

void Button::setIsHead(bool head)
{
    if (m_isHead == head) return;

    m_isHead = head;

    style()->unpolish(this);
    style()->polish(this);
}

Button::Button(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    setIsHead(false);
}
