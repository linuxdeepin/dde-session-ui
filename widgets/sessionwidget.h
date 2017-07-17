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
#include <QSettings>

#include <QLightDM/SessionsModel>

#include "rounditembutton.h"

class SessionWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SessionWidget(QWidget *parent = 0);
    ~SessionWidget();

    void show();
    int sessionCount() const;
    const QString lastSessionName() const;
    const QString currentSessionName() const;
    const QString currentSessionKey() const;
    const QString currentSessionOwner() const { return m_currentUser; }

signals:
    void sessionChanged(const QString &sessionName);

public slots:
    void saveSettings();
    void switchToUser(const QString &userName);
    //    void leftKeySwitch();
    //    void rightKeySwitch();
    //    void chooseSession();

private slots:
    void loadSessionList();

private slots:
    void onSessionButtonClicked();

private:
    int sessionIndex(const QString &sessionName);

private:
    int m_currentSessionIndex;
    QString m_currentUser;
    QSettings m_userSettings;

    QLightDM::SessionsModel *m_sessionModel;
    QList<RoundItemButton *> m_sessionBtns;
};

#endif // SESSIONWIDGET_H
