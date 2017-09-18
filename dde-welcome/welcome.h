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

#ifndef WELCOME_H
#define WELCOME_H

#include "update.h"
#include <QWidget>

#include <dpicturesequenceview.h>

class WelcomeService;
class Welcome : public QWidget
{
    Q_OBJECT

    friend class WelcomeService;

public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();

private:
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);

private:
    void dbus_show();
    void dbus_exit();

private slots:
    void clearCursor();
    void onScreenRectChanged();
    bool checkVersion();
    const QString getSystemVersion();

private:
    QTimer *m_sizeAdjustTimer;
    Update *m_update;
    Dtk::Widget::DPictureSequenceView *m_loadingSpinner;
    bool m_isUpgrade;
};

#endif // WELCOME_H
