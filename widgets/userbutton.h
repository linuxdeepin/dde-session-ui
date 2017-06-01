/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef USERBUTTON_H
#define USERBUTTON_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QPushButton>
#include <QtGui/QPaintEvent>
#include <QVBoxLayout>
#include <QObject>
#include <QLabel>
#include <QPropertyAnimation>

#include "useravatar.h"

static const int USER_ICON_WIDTH = 180;
static const int USER_ICON_HEIGHT = 180;

class UserButton:public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
public:
    UserButton(QString iconUrl, QString idName, QWidget* parent=0);
    ~UserButton();


    enum AvatarSize {
        AvatarSmallSize = 90,
        AvatarLargerSize = 110,
    };

    bool selected() const;
    void setSelected(bool selected);
    QString &name();

signals:
    void imageClicked(QString nam);
    void opacityChanged();

public slots:
    void sendClicked();
    void setImageSize(const AvatarSize &avatarsize);
    void setButtonChecked(bool checked);

    void showButton();
    void hide(const int duration = 0);
    void move(const QPoint &position, const int duration = 0);
    void stopAnimation();

    double opacity();
    bool isChecked();
    void setOpacity(double opa);
    void setCustomEffect();
    void addTextShadowAfter();
    void updateAvatar(const QString &icon);
protected:
    void paintEvent(QPaintEvent* event);
private:
    void initUI();
    void initConnect();
    void addTextShadow(bool isEffective);

    bool m_selected = false;
    QString m_iconUrl;
    QString m_buttonId;
    UserAvatar* m_userAvatar;
    QLabel* m_textLabel;
    QLabel *m_checkedMark;
    QHBoxLayout* m_buttonLayout;
    QHBoxLayout *m_nameLayout;
    QVBoxLayout* m_Layout;

    AvatarSize m_avatarsize = AvatarLargerSize;
    int m_borderWidth = 0;

    double m_opacity;

#ifndef DISABLE_ANIMATIONS
    QPropertyAnimation *m_moveAni;
    QPropertyAnimation* m_showAnimation;
    QPropertyAnimation* m_hideAnimation;
#endif

    QGraphicsOpacityEffect* m_opacityEffect;
};
#endif // UserButton

