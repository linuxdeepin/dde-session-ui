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

#ifndef DSHINELABEL
#define DSHINELABEL
#include <QtWidgets/QLabel>
#include <QtGui/QPalette>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QTimer>

//The label can just control the fade in or out of text

class DBreathingLabel: public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int alpha READ alpha WRITE setAlpha)
public:
    DBreathingLabel(QWidget *parent=0, Qt::WindowFlags f = 0);
    DBreathingLabel(const QString & text, QWidget * parent = 0,
                    Qt::WindowFlags f = 0);
    ~DBreathingLabel();
    void setColor(QColor color);

public slots:
    void showLabel();
    void hideLabel();
    void setDuration(int duration);
private:
    int alpha_;
    int alpha() const;
    void setAlpha(int alpha);
    QPropertyAnimation* m_showAnimation;
    QPropertyAnimation* m_hideAnimation;
    QPalette m_palette;
};
#endif // DSHINELABEL

