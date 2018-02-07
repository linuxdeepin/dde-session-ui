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

#ifndef USERBREATHINGLABEL
#define USERBREATHINGLABEL

#include <QLabel>
#include <QPainter>
#include <QPalette>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QObject>

class UserBreathingLabel:public QLabel
{
    Q_OBJECT
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
public:

    UserBreathingLabel(QWidget *parent=0);
    ~UserBreathingLabel();
    QGraphicsOpacityEffect* label_effect;
    QTimer* timer_fadeIn;
    QTimer* timer_fadeOut;
    double opacity() const;
signals:
    void opacityChanged();
    void showFinished();
    void hideFinished();
public slots:
    void showLabel();
    void hideLabel();

    void setOpacity(double opa);
    void setEffect();
private:
    double m_opacity;

    QPropertyAnimation* m_showOpacityAnimation;
    QPropertyAnimation* m_hideOpacityAnimation;
};
#endif // USERBREATHINGLABEL

