/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei@uniontech.com>
 *
 * Maintainer: chenwei <chenwei@uniontech.com>
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
#ifndef ACCESSIBLE_H
#define ACCESSIBLE_H

#include "accessibledefine.h"
#include "notification/actionbutton.h"

#include "notification-center/notifycenterwidget.h"
#include "notification-center/bubbleitem.h"
#include "notification-center/bubbletitlewidget.h"
#include "notification-center/overlapwidet.h"
#include "notification-center/notifylistview.h"
#include "notification/appbody.h"
#include "notification/iconbutton.h"
#include "notification/bubble.h"

#include <DLabel>
#include <DIconButton>

#include <QWidget>

SET_FORM_ACCESSIBLE(NotifyCenterWidget, "NotifyCenterWidget")
SET_FORM_ACCESSIBLE(NotifyListView, "NotifyListView")
SET_FORM_ACCESSIBLE(BubbleItem, "BubbleItem")
SET_FORM_ACCESSIBLE(BubbleTitleWidget, m_w->objectName().isEmpty() ? "DIconButton" : m_w->objectName())
SET_FORM_ACCESSIBLE(OverLapWidet, "BubbleItem")
SET_FORM_ACCESSIBLE(Bubble, "Bubble")
SET_FORM_ACCESSIBLE(AlphaWidget, m_w->objectName().isEmpty() ? "AlphaWidget" : m_w->objectName())
SET_FORM_ACCESSIBLE(HalfRoundedRectWidget, "HalfRoundedRectWidget")

SET_BUTTON_ACCESSIBLE(ActionButton, "ActionButton")
SET_BUTTON_ACCESSIBLE(IconButton, m_w->objectName().isEmpty() ? "IconButton" : m_w->objectName())
SET_BUTTON_ACCESSIBLE(DIconButton, m_w->objectName().isEmpty() ? "DIconButton" : m_w->objectName())
SET_LABEL_ACCESSIBLE(DLabel, m_w->objectName().isEmpty() ? m_w->text().isEmpty() ? "DLabel" : m_w->text() : m_w->objectName())

QAccessibleInterface *accessibleFactory(const QString &classname, QObject *object)
{
    QAccessibleInterface *interface = nullptr;

    USE_ACCESSIBLE(classname, NotifyCenterWidget)
    USE_ACCESSIBLE(classname, NotifyListView)
    USE_ACCESSIBLE(classname, BubbleItem)
    USE_ACCESSIBLE(classname, BubbleTitleWidget)
    USE_ACCESSIBLE(classname, OverLapWidet)
    USE_ACCESSIBLE(classname, ActionButton)
    USE_ACCESSIBLE(classname, IconButton)
    USE_ACCESSIBLE(classname, Bubble)
    USE_ACCESSIBLE(classname, AlphaWidget)
    USE_ACCESSIBLE(classname, HalfRoundedRectWidget)
    USE_ACCESSIBLE(QString(classname).replace("Dtk::Widget::", ""), DIconButton)
    USE_ACCESSIBLE(QString(classname).replace("Dtk::Widget::", ""), DLabel)

    return interface;
}
#endif // ACCESSIBLE_H

