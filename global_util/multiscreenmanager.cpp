#include "multiscreenmanager.h"

#include <QGuiApplication>
#include <QDesktopWidget>
#include <QDebug>

MultiScreenManager::MultiScreenManager(QObject *parent)
    : QObject(parent)
    , m_registerFunction(nullptr)
{
    connect(qApp, &QGuiApplication::screenAdded, this, &MultiScreenManager::onScreenAdded);
    connect(qApp, &QGuiApplication::screenRemoved, this, &MultiScreenManager::onScreenRemoved);
}

void MultiScreenManager::register_for_mutil_screen(std::function<QWidget *(QScreen *)> function)
{
    m_registerFunction = function;

    // update all screen
    for (QScreen *screen : qApp->screens()) {
        m_frames[screen] = function(screen);
    }
}

void MultiScreenManager::onScreenAdded(QScreen *screen)
{
    if (!m_registerFunction) {
        return;
    }

    m_frames[screen] = m_registerFunction(screen);
}

void MultiScreenManager::onScreenRemoved(QScreen *screen)
{
    if (!m_registerFunction) {
        return;
    }

    m_frames[screen]->deleteLater();
    m_frames.remove(screen);
}
