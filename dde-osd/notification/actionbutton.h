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

#ifndef ACTIONBUTTON_H
#define ACTIONBUTTON_H

#include <QFrame>
#include <QPainter>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMap>

#include "constants.h"

class Button;
class MenuButton;
class CloseButton;
class ActionButton : public QFrame
{
    Q_OBJECT
public:
    ActionButton(QWidget *parent = nullptr, OSD::ShowStyle style = OSD::ShowStyle::BUBBLEWINDOW);

    bool addButtons(const QStringList &list);
    bool isEmpty();
    void clear();

    QSize buttonSize() const {return m_buttonSize;}
    void setButtonSize(const QSize &size);

private:
    void initUI();
    void initConnections();

Q_SIGNALS:
    void buttonClicked(const QString &id);

private:
    OSD::ShowStyle m_showStyle;
    QHBoxLayout *m_layout = nullptr;
    QList<Button *> m_buttons;
    Button *m_menuButton = nullptr;

    QSize m_buttonSize = QSize(70, 60);
};

#endif // ACTIONBUTTON_H
