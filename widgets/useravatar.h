/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef USERAVATAR_H
#define USERAVATAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>
#include <QDebug>

#include <dimagebutton.h>

#include "userbreathinglabel.h"

//class AvatarDeleteButton : public DImageButton
//{
//    Q_OBJECT
//public:
//    AvatarDeleteButton(QWidget *parent = 0);
//};

class UserAvatar : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor borderSelectedColor READ borderSelectedColor WRITE setBorderSelectedColor)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)

public:
    enum AvatarSize {
        AvatarSmallSize,
        AvatarNormalSize,
        AvatarLargeSize
    };

    explicit UserAvatar(QWidget *parent = 0, bool deleteable = false);
    void setIcon(const QString &iconPath, const QSize &size = QSize(0, 0));
    void setAvatarSize(const AvatarSize &avatarSize);
    void setDisabled(bool disable);

    QString iconPath() const;

    QColor borderSelectedColor() const;
    void setBorderSelectedColor(const QColor &borderSelectedColor);

    QColor borderColor() const;
    void setBorderColor(const QColor &borderColor);

    int borderWidth() const;
    void setBorderWidth(int borderWidth);

    void setSelected(bool selected);

    bool deleteable() const;
    void setDeleteable(bool deleteable);

    void showButton();
    void hideButton();
    void setColor(QColor color);
signals:
    void mousePress();
    void requestDelete();
    void userAvatarClicked();
    void alphaChanged();

signals:
    void showFinished();
    void hideFinished();
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    QImage imageToGray(const QImage &image);
    void initDeleteButton();

//    AvatarDeleteButton *m_deleteButton = NULL;
    QLabel *m_iconLabel = NULL;
    QString m_iconPath = "";
    QColor m_borderColor;
    QColor m_borderSelectedColor;
    AvatarSize m_avatarSize = AvatarNormalSize;
    int m_borderWidth = 0;
    bool m_selected = false;
    bool m_deleteable = false;

    const int SMALL_ICON_SIZE = 80;
    const int NORMAL_ICON_SIZE = 90;
    const int LARGE_ICON_SIZE = 100;

    QPalette m_palette;
    QPropertyAnimation *m_showAnimation;
    QPropertyAnimation *m_hideAnimation;
};


#endif // USERAVATAR_H
