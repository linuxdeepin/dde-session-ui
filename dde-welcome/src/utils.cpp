/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     justforlxz <zhangdingyuan@deepin.com>
 *
 * Maintainer: justforlxz <zhangdingyuan@deepin.com>
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

#include "utils.h"

const std::pair<QString, QString> GetSystemVersion()
{
    QSettings lsbSetting("/etc/deepin-version", QSettings::IniFormat);
    lsbSetting.setIniCodec("utf-8");
    lsbSetting.beginGroup("Release");
    QLocale locale;

    if (locale.language() == QLocale::Chinese)
        return std::pair<QString, QString>{ lsbSetting.value("Version").toString(),
                    lsbSetting.value(QString("Type[%1]").arg(locale.name()), "").toString() };

    return std::pair<QString, QString>{ lsbSetting.value("Version").toString(),
                lsbSetting.value(QString("Type"), "").toString() };
}
