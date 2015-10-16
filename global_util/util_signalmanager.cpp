#include "util_signalmanager.h"

SignalManager* SignalManager::m_signalManager = NULL;

SignalManager::SignalManager(QObject *parent)
    :QObject(parent)
{
}
SignalManager::~SignalManager()
{
}
SignalManager* SignalManager::getInstance() {
    if (m_signalManager == NULL) {
        m_signalManager = new SignalManager();
    }
    return m_signalManager;
}
