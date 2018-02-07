/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
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

#ifndef DMEMORYWARNINGDIALOGADAPTOR_H
#define DMEMORYWARNINGDIALOGADAPTOR_H

#include "dmemorywarningdialog.h"

#include <QDBusAbstractAdaptor>

class DMemoryWarningDialogAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.MemoryWarningDialog")

public:
    explicit DMemoryWarningDialogAdaptor(DMemoryWarningDialog *dialog);
    ~DMemoryWarningDialogAdaptor();

public Q_SLOTS:
    void Show(const QString &launchInfo);
    void Hide();

private:
    DMemoryWarningDialog *dialog;
};

#endif // DMEMORYWARNINGDIALOGADAPTOR_H
