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

#ifndef WMCHOOSER_H
#define WMCHOOSER_H

#include "../boxframe/boxframe.h"

#include <QWidget>
#include <QPushButton>

class WMChooser : public QWidget
{
    Q_OBJECT

public:
    WMChooser(QWidget *parent = 0);

    void setConfigPath(const QString &path);
    void createParentDir(const QString &fullfilepath);

signals:
    void screenChanged();

private slots:
    void saveConfig();

protected:
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    QPushButton *m_gorgeousBtn;
    QPushButton *m_topSpeedBtn;
    QString m_configPath;
};

#endif // WMCHOOSER_H
