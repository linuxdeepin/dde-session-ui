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

#ifndef OSDPROVIDER_H
#define OSDPROVIDER_H

#include <QObject>
#include <QSize>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QListView>

#include "common.h"

class AbstractOSDProvider : public QObject
{
    Q_OBJECT
public:
    explicit AbstractOSDProvider(QObject *parent = 0);

    // match tell if we should handle the param.
    virtual bool match(const QString &param);

    // check whether the conditions are all met so we can show
    // our content.
    virtual bool checkConditions() const;

    // provide hint about how many space we will take.
    virtual QSize contentSize() const;

    virtual QMargins contentMargins() const;

    virtual QListView::Flow flow() const;

    virtual void highlightCurrent();
    virtual void highlightNext();

    virtual void sync();

    // model
    virtual int rowCount(const QModelIndex &parent) const = 0;
    virtual QVariant data(const QModelIndex &index, int role) const = 0;

    // delegate
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;

signals:
    void dataChanged();

protected:
    QStringList m_suitableParams;
};

#endif // OSDPROVIDER_H
