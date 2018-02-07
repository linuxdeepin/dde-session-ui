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

#include "dialog.h"

Dialog::Dialog() : DDialog()
{
    setTitle(tr("Updates downloaded, restart to update?"));
    setIcon(QIcon::fromTheme("system_updated", QIcon::fromTheme("system-software-update")).pixmap(48));

    addButton(tr("Remind Later"));
    addButton(tr("Not Now"));
    addButton(tr("Update Now"));

    new Adaptor(this);
}

Adaptor::Adaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

void Adaptor::RaiseWindow()
{
    QWidget * pp = qobject_cast<QWidget*>(parent());
    pp->activateWindow();
    pp->raise();
}
