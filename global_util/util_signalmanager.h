/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SIGNALMANAGER
#define SIGNALMANAGER
#include <QObject>

class SignalManager: public QObject
{
    Q_OBJECT
public:
    SignalManager(QObject* parent=0);
    ~SignalManager();
    static SignalManager* getInstance();
signals:
    void setButtonCheck(QString buttonId);
    void setButtonHover(QString buttonId);
    void buttonStyleChanged();
private:
    static SignalManager* m_signalManager;

};

extern  SignalManager* signalManager;

#endif // SIGNALMANAGER

