/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QObject>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QKeyEvent>
#include <QFrame>

#include <dloadingindicator.h>

#include <QLightDM/UsersModel>

#include "userbutton.h"
#include "dbus/dbuslockservice.h"

#define ACCOUNT_DBUS_SERVICE "com.deepin.daemon.Accounts"
#define ACCOUNT_DBUS_PATH "/com/deepin/daemon/Accounts"

class UserWidget : public QFrame
{
    Q_OBJECT
public:
    UserWidget(QWidget* parent = 0);
    ~UserWidget();

    static const QString loginUser();
    const QString currentUser();
    inline int count() const {return m_userBtns.count();}
    bool isChooseUserMode = false;

signals:
    void userChanged(const QString &username);
    void chooseUserModeChanged(bool isChoose, QString curUser);

public slots:
    void addUser(QString avatar, QString name);
    void removeUser(QString name);

    void setCurrentUser(const QString &username);
    void expandWidget();
    void saveLastUser();
    inline void showLoadingAni() {m_loadingAni->show();}
    inline void hideLoadingAni() {m_loadingAni->hide();}

    void leftKeySwitchUser();
    void rightKeySwitchUser();
    void switchUserByKey(int i, int j);
    void chooseButtonChecked();

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private slots:
    void initUI();
    void initConnections();
    void updateAvatar(QString username);

    QStringList getUsernameList();

    void handleUserAdded(const QModelIndex &parent, int first, int last);
    void handleUserRemoved(const QModelIndex &parent, int first, int last);

private:
    int m_currentUserIndex = 0;
    int countNum = 0;
    QSettings m_settings;
    QString m_currentUser = QString();
    QStringList m_whiteList;
    DBusLockService m_lockInter;
    UserButton* m_currentBtns = nullptr;
    QList<UserButton *> m_userBtns;
    QLightDM::UsersModel *m_userModel;
    DTK_WIDGET_NAMESPACE::DLoadingIndicator *m_loadingAni;
};

#endif // WIDGET_H
