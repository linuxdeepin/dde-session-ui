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

#include "warningdialog.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QIcon>

WarningDialog::WarningDialog(QWidget *parent)
    : DDialog(parent)
{
    const QString title = tr("Kindly Reminder");
    const QString body = tr("This application can not run without window effect");
    setTitle(title);
    setMessage(body);
    setIconPixmap(QIcon::fromTheme("dialog-warning").pixmap(48, 48));
    addButton(tr("OK"), true);

    const QDesktopWidget *desktop = QApplication::desktop();
    const QPoint cursorPoint = QCursor::pos();
    const QRect geom = desktop->screenGeometry(cursorPoint);

    move(geom.width() - width() / 2,
         geom.height() - height() / 2);

}

WarningDialog::~WarningDialog()
{
}
