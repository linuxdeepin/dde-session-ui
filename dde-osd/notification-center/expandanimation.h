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

class BubbleItem;

DWIDGET_USE_NAMESPACE
/*!
 * \~chinese \class ExpandAnimation
 * \~chinese \brief 动画类,用于添加数据移除数据时产生动画效果的类
 */
class ExpandAnimation : public DBlurEffectWidget
{
    Q_OBJECT
public:
    ExpandAnimation(QWidget *parent = nullptr);
    ~ExpandAnimation();
    void expandData(const QList<EntityPtr> &);  //展开通知时的动画
    void appendData(const QList<EntityPtr> &);  //添加数据时的动画
    void removeData(const QList<EntityPtr> &);  //移除数据时的动画
    void start() {m_animationGroup->start(); } //开始动画

signals:
    void finished();

private:
    QList<QPointer<BubbleItem>> m_bubbleList;
    QParallelAnimationGroup *m_animationGroup = nullptr;
};

#endif // EXPANDANIMATION_H
