/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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

#include <QtGui/QPainter>
#include <QDebug>
#include "imagebutton.h"

ImageButton::ImageButton(QString url, QString name, QWidget *parent)
    : QPushButton(parent)
{
    m_url = url;
    m_name = name;
    this->setObjectName(name);
    setFixedSize(120, 120);

   // connect(this, SIGNAL(clicked()), this, SLOT(sendClicked()));
}
ImageButton::~ImageButton()
{
}
void ImageButton::sendClicked() {
    emit clicked(m_name);
}
void ImageButton::hideIn(QString name) {
    if (name!=this->objectName()) {
        this->hide();
    }
}

void ImageButton::showOut() {
    this->show();
}
void ImageButton::setIconSize(const AvatarSize &avatarsize) {
    int tmpAvatarSize = SMALL_ICON_SIZE;

    if (avatarsize==AvatarBigSize) {
        tmpAvatarSize = LARGE_ICON_SIZE;
    } else {
        tmpAvatarSize = SMALL_ICON_SIZE;
    }

    m_avatarsize = avatarsize;
    this->setFixedSize(tmpAvatarSize, tmpAvatarSize);
}
void ImageButton::paintEvent(QPaintEvent *) {
    int iconSize = SMALL_ICON_SIZE;
    if (m_avatarsize == AvatarBigSize) {
        iconSize = LARGE_ICON_SIZE;
    }

    QPainter painter(this);
    QRect ellipseRec((width() -iconSize) / 2.0, (height() - iconSize) / 2.0, iconSize, iconSize);
    QPainterPath path;
    path.addEllipse(ellipseRec);
    painter.setRenderHints(QPainter::Antialiasing
                           | QPainter::SmoothPixmapTransform);

    painter.setClipPath(path);

    QPixmap pixmap(m_url);
    painter.drawPixmap(ellipseRec, pixmap);
    QColor penColor =  "yellow";

    QPen pen;
    pen.setColor(penColor);
    pen.setWidth(m_borderWidth);
    painter.setPen(pen);
    painter.drawPath(path);
    painter.end();
}
