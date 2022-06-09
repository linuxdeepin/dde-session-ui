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

#include "dmemorywarningdialogadaptor.h"

DMemoryWarningDialogAdaptor::DMemoryWarningDialogAdaptor(DMemoryWarningDialog *dialog)
    : QDBusAbstractAdaptor(dialog)

    , dialog(dialog)
{

}

DMemoryWarningDialogAdaptor::~DMemoryWarningDialogAdaptor()
{

}

void DMemoryWarningDialogAdaptor::Show(const QString &launchInfo)
{
    qDebug() << Q_FUNC_INFO << launchInfo;

    dialog->show();
    dialog->updateAppInfo(launchInfo);
}

void DMemoryWarningDialogAdaptor::Hide()
{
    qDebug() << Q_FUNC_INFO;

    dialog->hide();
}
