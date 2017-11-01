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

#ifndef KBLAYOUTPROVIDER_H
#define KBLAYOUTPROVIDER_H

#include "osdprovider.h"

#include <com_deepin_daemon_inputdevice_keyboard.h>

using namespace com::deepin::daemon::inputdevice;

class KBLayoutProvider : public AbstractOSDProvider
{
    Q_OBJECT
public:
    explicit KBLayoutProvider(QObject *parent = 0);

    bool checkConditions() const Q_DECL_OVERRIDE;
    QSize contentSize() const Q_DECL_OVERRIDE;
    QMargins contentMargins() const Q_DECL_OVERRIDE;

    void highlightCurrent() Q_DECL_OVERRIDE;
    void highlightNext() Q_DECL_OVERRIDE;

    void sync() Q_DECL_OVERRIDE;

    // model
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int currentIndex() const;

    // delegate
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    KeyboardLayoutList m_database;

    QStringList m_userLayouts;
    QString m_userCurrentLayout;

    Keyboard *m_keyboardInter;

    QString describeLayout(const QString &layout) const;

private slots:
    void currentLayoutChanged(const QString &layout);
    void userLayoutListChanged(const QStringList &layouts);
};

#endif // KBLAYOUTPROVIDER_H
