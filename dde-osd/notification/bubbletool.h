/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     zorowk <near.kingzero@gmail.com>
 *
 * Maintainer: zorowk <near.kingzero@gmail.com>
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
#ifndef BUBBLETOOL_H
#define BUBBLETOOL_H

#include <QRgb>
#include <QDBusArgument>
#include <QImage>
#include <memory>

class ActionButton;
class AppIcon;
class NotificationEntity;
class BubbleTool
{
public:
    static QString processActions(ActionButton *action, QStringList action_list);
    static void processIconData(AppIcon *icon, std::shared_ptr<NotificationEntity> entity);
    static void actionInvoke(const QString &actionId, std::shared_ptr<NotificationEntity> entity);
    static void register_wm_state(WId winid);
    static const QString getDeepinAppName(const QString &name);

private:
    static void saveImg(const QImage &image, uint id);
    static QPixmap converToPixmap(AppIcon *icon, const QDBusArgument &value, uint id);
    static inline void copyLineRGB32(QRgb *dst, const char *src, int width);
    static inline void copyLineARGB32(QRgb *dst, const char *src, int width);
    static QImage decodeNotificationSpecImageHint(const QDBusArgument &arg);
};

#endif // BUBBLETOOL_H
