/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "util_signalmanager.h"

SignalManager* signalManager = SignalManager::getInstance();

SignalManager* SignalManager::m_signalManager = NULL;

SignalManager::SignalManager(QObject* parent)
    :QObject(parent)
{
}
SignalManager* SignalManager::getInstance() {
    if (m_signalManager == NULL) {
        m_signalManager = new SignalManager();
    }
    return m_signalManager;
}
SignalManager::~SignalManager()
{
}
