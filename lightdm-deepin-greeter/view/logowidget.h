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

#ifndef LOGOWIDGET
#define LOGOWIDGET

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class LogoWidget: public QFrame
{
    Q_OBJECT
public:
    LogoWidget(QWidget* parent=0);
    ~LogoWidget();

    void updateLocale(const QString &locale);

private:
    void initUI();
    QString getVersion();
    QLabel* m_logoLabel;
    QLabel* m_logoVersionLabel;
    QVBoxLayout* m_logoLeftSideLayout;
    QVBoxLayout* m_logoRightSideLayout;
    QHBoxLayout* m_logoLayout;
    QString m_locale;
};
#endif // LOGOFRAME

