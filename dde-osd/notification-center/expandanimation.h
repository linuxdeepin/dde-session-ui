/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     zorowk <near.kingzero@gmail.com>
 *
 * Maintainer: zorowk <near.kingzero@gmail.com>
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

#ifndef EXPANDANIMATION_H
#define EXPANDANIMATION_H

#include <DBlurEffectWidget>
#include <QPointer>
#include <QParallelAnimationGroup>

#include "notification/constants.h"

class QSequen;
class NotificationEntity;
class BubbleItem;

DWIDGET_USE_NAMESPACE

class ExpandAnimation : public DBlurEffectWidget
{
    Q_OBJECT
public:
    ExpandAnimation(QWidget *parent = nullptr);
    ~ExpandAnimation();
    void expandData(const QList<EntityPtr> &);
    void appendData(const QList<EntityPtr> &);
    void removeData(const QList<EntityPtr> &);
    void start() { m_animationGroup->start(); }

signals:
    void finished();

private:
    QList<QPointer<BubbleItem>> m_bubbleList;
    QParallelAnimationGroup *m_animationGroup = nullptr;
};

#endif // EXPANDANIMATION_H
