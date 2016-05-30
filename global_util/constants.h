/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace DDESESSIONCC
{

const QString CONFIG_FILE("/var/lib/lightdm/lightdm-deepin-greeter/state");
const QString LOCK_SHUTDOWN_FLAG("/tmp/expandstate");
const QString DEFAULT_CURSOR_THEME("/usr/share/icons/default/index.theme");

const int PASSWDLINEEIDT_WIDTH = 250;
const int LAYOUTBUTTON_HEIGHT =  36;

const int CapslockWarningWidth = 23;
const int CapslockWarningRightMargin = 8;
}


#endif // CONSTANTS_H

