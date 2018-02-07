/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

#include "dbus/dbusmediaplayer2.h"

#include <QWidget>
#include <dimagebutton.h>
#include <dmpriscontrol.h>

DWIDGET_USE_NAMESPACE

class MediaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MediaWidget(QWidget *parent = 0);
    void initMediaPlayer();

private slots:
    void onVolumeChanged();
    void onMuteSwitchChanged();
    void changeVisible();

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initConnect();

private:
    DImageButton *m_volumeBtn;
    DMPRISControl *m_dmprisWidget;
    QLabel *m_volumeNums;
    double m_lastVolumeNums;
    DBusMediaPlayer2 *m_dbusInter;
};

#endif // MEDIAWIDGET_H
