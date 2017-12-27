/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QFile>
#include <QFileInfo>
#include <QPainter>
#include <QSvgRenderer>
#include <QIcon>

#include "appicon.h"


AppIcon::AppIcon(QWidget *parent) :
    QLabel(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAlignment(Qt::AlignCenter);
}

void AppIcon::setIcon(const QString &iconPath)
{

    QPixmap pixmap(48, 48);

    // iconPath is an absolute path of the system.
    if (QFile::exists(iconPath) && QFileInfo(iconPath).isAbsolute()) {
        pixmap = QPixmap(iconPath);
    } else if (iconPath.startsWith("data:image/")){
        // iconPath is a string representing an inline image.
        QStringList strs = iconPath.split("base64,");
        if (strs.length() == 2) {
            QByteArray data = QByteArray::fromBase64(strs.at(1).toLatin1());
            pixmap.loadFromData(data);
        }
    } else {
        const QIcon &icon = QIcon::fromTheme(iconPath, QIcon::fromTheme("application-x-desktop"));
        pixmap = icon.pixmap(48, 48);
    }

    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(width(), height(),
                               Qt::KeepAspectRatioByExpanding,
                               Qt::SmoothTransformation);

        setPixmap(pixmap);
    }
}
