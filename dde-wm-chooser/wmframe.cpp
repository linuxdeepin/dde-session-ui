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

#include "wmframe.h"

#include <QScreen>

WMFrame::WMFrame(QWidget *parent) : BoxFrame(parent)
{
    m_wmchooser = new WMChooser(this);

    updateScreenPosition();

    connect(m_wmchooser, &WMChooser::screenChanged, this, &WMFrame::updateScreenPosition);
}

void WMFrame::setConfigPath(const QString &path)
{
    m_wmchooser->setConfigPath(path);
}

void WMFrame::updateScreenPosition()
{
    QList<QScreen *> screenList = qApp->screens();
    QPoint mousePoint = QCursor::pos();
    for (const QScreen *screen : screenList)
    {
        if (screen->geometry().contains(mousePoint))
        {
            const QRect &geometry = screen->geometry();
            m_wmchooser->setFixedSize(geometry.size());
            m_wmchooser->move(geometry.x(), geometry.y());
            m_wmchooser->updateGeometry();

            return;
        }
    }
}
