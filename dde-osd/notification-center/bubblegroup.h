/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
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

#ifndef BUBBLEGROUP_H
#define BUBBLEGROUP_H

#include <DIconButton>
#include <QPointer>
#include <QWidget>
#include <QBoxLayout>
#include <memory>

DWIDGET_USE_NAMESPACE

static const int GroupTitleHeight = 32;
static const int GroupButtonSize = 24;

class BubbleItem;
class NotificationEntity;

class BubbleGroup : public QWidget
{
    Q_OBJECT
public:
    explicit BubbleGroup(QWidget* parent = nullptr, std::shared_ptr<NotificationEntity> entity = nullptr);

    void pushBubble(std::shared_ptr<NotificationEntity> entity);
    void popBubble();
    void closeGroup();

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QList<QPointer<BubbleItem>> m_bubbleList;
    QWidget *m_titleWidget = nullptr;
    QVBoxLayout *mainLayout = nullptr;
    DIconButton *title_close = nullptr;
};

#endif // BUBBLEGROUP_H
