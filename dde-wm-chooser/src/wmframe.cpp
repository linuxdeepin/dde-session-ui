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

#include "wmframe.h"

#include <QScreen>
#include <QGSettings>

WMFrame::WMFrame(QWidget *parent)
    : FullscreenBackground(parent)
    , m_blurImageInter(new ImageBlur("com.deepin.daemon.Accounts",
                                     "/com/deepin/daemon/ImageBlur",
                                     QDBusConnection::systemBus(), this))
{
    setAccessibleName("WMFrame");
    m_wmchooser = new WMChooser(this);
    m_wmchooser->setAccessibleName("WMChooser");
    setContent(m_wmchooser);

    QGSettings gsettings("com.deepin.dde.appearance", "", this);
    const QStringList list = gsettings.get("background-uris").toStringList();
    QString wallpaper = list.first();

    const QUrl url(wallpaper);
    wallpaper = url.isLocalFile() ? url.path() : url.url();

    // blur wallpaper
    const QString &w = m_blurImageInter->Get(wallpaper);

    updateBackground(w.isEmpty() ? wallpaper : w);
}

void WMFrame::setConfigPath(const QString &path)
{
    m_wmchooser->setConfigPath(path);
}
