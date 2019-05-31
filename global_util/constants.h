/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QStringList>

namespace DDESESSIONCC
{

static const QString CONFIG_FILE("/var/lib/AccountsService/users/");
static const QString DEFAULT_CURSOR_THEME("/usr/share/icons/default/index.theme");
static const QString LAST_USER_CONFIG("/var/lib/lightdm/lightdm-deepin-greeter");
static const int PASSWDLINEEIDT_WIDTH = 280;
static const int PASSWDLINEEDIT_HEIGHT = 36;
static const int LAYOUTBUTTON_HEIGHT =  36;

static const int CapslockWarningWidth = 23;
static const int CapslockWarningRightMargin = 8;

const QStringList session_ui_configs {
    "/etc/lightdm/lightdm-deepin-greeter.conf",
    "/etc/deepin/dde-session-ui.conf",
    "/usr/share/dde-session-ui/dde-session-ui.conf"
};

const QStringList SHUTDOWN_CONFIGS {
    "/etc/lightdm/lightdm-deepin-greeter.conf",
    "/etc/deepin/dde-session-ui.conf",
    "/etc/deepin/dde-shutdown.conf",
    "/usr/share/dde-session-ui/dde-shutdown.conf"
};
}


#endif // CONSTANTS_H

