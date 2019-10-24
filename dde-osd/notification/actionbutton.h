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

#include <DPushButton>

DWIDGET_USE_NAMESPACE

#define BUTTONWIDTH (70)
#define BUTTONHEIGHT (60)
#define PADDING (10)
#define CLOSEBTNSIZE (QSize(30,30))

class Button;
class MenuButton;
class CloseButton;
class ActionButton : public QFrame
{
    Q_OBJECT
public:
    ActionButton(QWidget *parent = nullptr);

    bool addButtons(const QStringList &list);
    bool isEmpty();
    void clear();

public Q_SLOTS:
    void onFocusChanged(bool has);

private:
    void initUI();
    void initConnections();

Q_SIGNALS:
    void closeButtonClicked();
    void buttonClicked(const QString &id);
    void expired(int);
    void dismissed(int);
    void replacedByOther(int);

private:
    bool m_canClose = false;
    QHBoxLayout *m_layout = nullptr;
    QList<Button *> m_buttons;
    Button *m_closeButton = nullptr;
    Button *m_menuButton = nullptr;
};

#endif // ACTIONBUTTON_H
