/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     LiLinling <lilinling_cm@deepin.com>
 *
 * Maintainer: LiLinling <lilinling_cm@deepin.com>
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
#include "launcheriteminfo.h"

bool LauncherItemInfo::operator!=(const LauncherItemInfo &itemInfo)
{
    return itemInfo.path != path;
}

QDBusArgument &operator<<(QDBusArgument &argument, const LauncherItemInfo &itemInfo)
{
    argument.beginStructure();
    argument << itemInfo.path << itemInfo.name << itemInfo.id << itemInfo.icon << itemInfo.categoryId << itemInfo.timeInstalled << itemInfo.keywords;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, LauncherItemInfo &itemInfo)
{
    argument.beginStructure();
    argument >> itemInfo.path >> itemInfo.name >> itemInfo.id >> itemInfo.icon >> itemInfo.categoryId >> itemInfo.timeInstalled >> itemInfo.keywords;
    argument.endStructure();
    return argument;
}

void registerLauncherItemInfoMetaType()
{
    qRegisterMetaType<LauncherItemInfo>("ItemInfo");
    qDBusRegisterMetaType<LauncherItemInfo>();
}
