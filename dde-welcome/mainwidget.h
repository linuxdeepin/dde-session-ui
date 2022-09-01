// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "fullscreenbackground.h"

#include <com_deepin_daemon_imageblur.h>

#include <QWidget>

using ImageBlur = com::deepin::daemon::ImageBlur;

class MainWidget : public FullscreenBackground
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void onBlurWallpaperFinished(const QString &source, const QString &blur, bool status);

private:
    ImageBlur *m_blurImageInter;
    QString m_wallpaper;
};

#endif // MAINWIDGET_H
