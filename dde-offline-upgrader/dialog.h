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

#ifndef DIALOG_H
#define DIALOG_H

#include <QDBusAbstractAdaptor>

#include <ddialog.h>

DWIDGET_USE_NAMESPACE

class Adaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.OfflineUpgrader")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.deepin.dde.OfflineUpgrader\">\n"
"    <method name=\"RaiseWindow\"/>\n"
"  </interface>\n"
        "")

public:
    Adaptor(QObject *parent);

public slots:
    void RaiseWindow();
};

class Dialog : public DDialog
{
	Q_OBJECT
public:
    Dialog();
};

#endif // DIALOG_H
