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

#ifndef KBLAYOUTWIDGET
#define KBLAYOUTWIDGET

#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>

#include "constants.h"
#include "util_updateui.h"
#include "xkbparser.h"

class LayoutButton: public QPushButton
{
    Q_OBJECT
public:
    LayoutButton(QString text, QWidget* parent = 0);
    ~LayoutButton();

    void updateStyleSelf();
    void setButtonChecked(bool checked);
    QString m_text;

signals:
    void mouseEnter(QString label);
    void mouseLeave(QString label);
    void onlyOneChecked(QString text);
public slots:
    void OnlyMeChecked(bool checked);
protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

private:

    QLabel* m_iconLabel;
    QLabel* m_textLabel;
};

class KbLayoutWidget: public QListWidget
{
    Q_OBJECT
public:
    KbLayoutWidget(QStringList buttons = QStringList(), QWidget* parent = 0);
    ~KbLayoutWidget();

    void initUI();
    void updateUI();
signals:
    void setButtonClicked(QString text);
    void focusOuted();

public slots:
    void setButtonsChecked(QString text);
    void setListItemChecked(int itemIndex);
    void updateButtonList(const QStringList &buttons);
    void setDefault(const QString &layout);

private:
    void addButton(const QString &button);
    void focusOutEvent(QFocusEvent *event) override;

private:
    const int widget_width = DDESESSIONCC::PASSWDLINEEIDT_WIDTH - 3;
    QHBoxLayout* m_Layout;
    QStringList m_buttons;
    QList<LayoutButton* > m_layoutButtons;
    QMap<QListWidgetItem*, QFrame*> m_layoutItemList;
    QString m_userLayout;
    XkbParser* xkbParse;
    QStringList m_kbdList;
    QStringList m_kbdParseList;
};
#endif // KEYBOARDLAYOUTFRAME

