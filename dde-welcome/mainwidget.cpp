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

#include "mainwidget.h"
#include "updatecontent.h"
#include "utils.h"

#include <QGSettings/QGSettings>
#include <QVariant>
#include <QKeyEvent>
#include <QApplication>

#include "dtkcore_global.h"
#if (DTK_VERSION >= DTK_VERSION_CHECK(2, 0, 8, 0))
#include <DDBusSender>
#else
#include <QProcess>
#endif

MainWidget::MainWidget(QWidget *parent)
    : FullscreenBackground(parent)
    , m_blurImageInter(new ImageBlur("com.deepin.daemon.Accounts",
                                     "/com/deepin/daemon/ImageBlur",
                                     QDBusConnection::systemBus(), this))
{
    setAccessibleName("MainWidget");
    connect(m_blurImageInter, &ImageBlur::BlurDone, this, &MainWidget::onBlurWallpaperFinished);
    connect(qApp, &QApplication::aboutToQuit, this, [=] {
        DDBusSender()
            .service("com.deepin.daemon.Zone")
            .interface("com.deepin.daemon.Zone")
            .path("/com/deepin/daemon/Zone")
            .method("EnableZoneDetected")
            .arg(true)
            .call();

    });

    QGSettings gsettings("com.deepin.dde.appearance", "", this);
    const QStringList list = gsettings.get("background-uris").toStringList();
    m_wallpaper = list.first();

    const QUrl url(m_wallpaper);
    m_wallpaper = url.isLocalFile() ? url.path() : m_wallpaper;

    UpdateContent *content = new UpdateContent(GetSystemVersion(), this);
    content->setAccessibleName("UpdateContent");
    setContent(content);
    content->hide();

    // blur wallpaper
    const QString &w = m_blurImageInter->Get(m_wallpaper);

    updateBackground(w.isEmpty() ? m_wallpaper : w);

    DDBusSender()
        .service("com.deepin.daemon.Zone")
        .interface("com.deepin.daemon.Zone")
        .path("/com/deepin/daemon/Zone")
        .method("EnableZoneDetected")
        .arg(false)
        .call();
}

void MainWidget::onBlurWallpaperFinished(const QString &source, const QString &blur, bool status)
{
    const QString &sourcePath = QUrl(source).isLocalFile() ? QUrl(source).toLocalFile() : source;

    if (status && m_wallpaper == sourcePath)
        updateBackground(blur);
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
#ifdef  QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    case Qt::Key_Enter:
    case Qt::Key_Return:
        qApp->quit();
    default:;
    }

    FullscreenBackground::keyPressEvent(event);
}
