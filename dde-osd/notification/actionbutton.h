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

#ifndef ACTIONBUTTON_H
#define ACTIONBUTTON_H

#include <QFrame>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMap>

class Button : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(bool isHead READ isHead DESIGNABLE true SCRIPTABLE true)

public:
    Button(const QString &text, QWidget *parent = 0);

    bool isHead() const { return m_isHead; }
    void setIsHead(bool head = true);

private:
    bool m_isHead;
};

class ActionButton : public QFrame
{
    Q_OBJECT

public:
    ActionButton(QWidget * parent = 0);

    bool addButtons(const QStringList &list);
    bool isEmpty();
    void clear();

signals:
    void buttonClicked(const QString &id);

private:
    QVBoxLayout *m_layout;
    QList<Button*> m_buttons;
};

#endif // ACTIONBUTTON_H
