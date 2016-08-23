/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SESSIONWIDGET_H
#define SESSIONWIDGET_H

#include <QFrame>
#include <QList>

#include <QLightDM/SessionsModel>

#include "rounditembutton.h"

class SessionWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SessionWidget(QWidget *parent = 0);
    ~SessionWidget();

    const QString currentSessionName() const;
    const QString currentSessionKey() const;

    QStringList Sessions = {
        "Deepin",
        "fluxbox",
        "gnome",
        "kde",
        "ubuntu",
        "xfce"
    };
    void show();
    QString lastSelectedUser() const;
    int sessionCount() const;

public slots:
    void switchToUser(const QString &userName);
    void saveUserLastSession(const QString &userName);
    void leftKeySwitch();
    void rightKeySwitch();
    void chooseSession();
    QString processSessionName(const QString &session);
signals:
    void sessionChanged(const QString &sessionName);

private slots:
    void switchSession(const QString &sessionName);
    int getSessionIndex(const QString &sessionName);
private:
    int m_currentSessionIndex;
    QString m_lastSelectedUser = QString();
    QList<RoundItemButton *> *m_sessionBtns;
    QLightDM::SessionsModel *m_sessionModel;
};

#endif // SESSIONWIDGET_H
