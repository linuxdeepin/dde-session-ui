// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fullscreenmanager.h"

#include "abstractfullbackgroundinterface.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>
#include <QTimer>

FullScreenManager::FullScreenManager(std::function<QWidget *(QScreen *)> function, QObject *parent)
    : QObject(parent)
    , m_registerFun(function)
    , m_delayTimer(new QTimer(this))
    , m_copyModeFlag(false)
{
    Q_ASSERT(m_registerFun);

    connect(qApp, &QGuiApplication::screenAdded, this, &FullScreenManager::screenCountChanged);
    connect(qApp, &QGuiApplication::screenRemoved, this, &FullScreenManager::screenCountChanged);

    screenCountChanged();

    // 处理可能的窗管配置延迟，可能导致对复制模式的判断逻辑错误
    // 在初始化3秒内持续检查复制模式
    // https://pms.uniontech.com/bug-view-229469.html
    QTimer *modeCheckTimer = new QTimer(this);
    modeCheckTimer->setInterval(100);
    connect(modeCheckTimer, &QTimer::timeout, this, &FullScreenManager::checkCopyModeChanged);
    modeCheckTimer->start();
    QTimer::singleShot(3000, modeCheckTimer, &QTimer::stop);

    m_delayTimer->setInterval(500);
    m_delayTimer->setSingleShot(true);
    connect(m_delayTimer, &QTimer::timeout, this, &FullScreenManager::handleScreenChanged);
}

FullScreenManager::~FullScreenManager()
{
    m_delayTimer->stop();

    // 各 FullScreen 由 m_registerFun 直接 new 出来，没有 parent 负责释放，这里统一清理。
    // 先把映射清空再删除，避免删除过程中 map 里残留悬空指针。
    const auto frames = m_screenContents.keys();
    m_screenContents.clear();
    for (QWidget *frame : frames) {
        delete frame;
    }
}

/**判断屏幕是否为复制模式的依据，第一个屏幕的X和Y值是否和其他的屏幕的X和Y值相等
 * 对于复制模式，这两个值肯定是相等的，如果不是复制模式，这两个值肯定不等，目前支持双屏
 * @brief DisplayManager::isCopyMode
 * @return
 */
bool FullScreenManager::isCopyMode() const
{
    QList<QScreen *> screens = qApp->screens();
    if (screens.size() < 2)
        return false;

    // 在多个屏幕的情况下，如果所有屏幕的位置的X和Y值都相等，则认为是复制模式
    QRect screenRect = screens[0]->availableGeometry();
    for (int i = 1; i < screens.size(); i++) {
        QRect rect = screens[i]->availableGeometry();
        if (screenRect.x() != rect.x() || screenRect.y() != rect.y())
            return false;
    }

    return true;
}

void FullScreenManager::checkCopyModeChanged()
{
    if (m_copyModeFlag != isCopyMode()) {
        m_copyModeFlag = isCopyMode();
        handleCopyModeChanged(m_copyModeFlag);

        Q_EMIT copyModeChanged(m_copyModeFlag);
    }
}

// 主动检查是否复制模式，响应其变化，避免 wayland 下显示模式设置在应用启动后
void FullScreenManager::handleCopyModeChanged(bool isCopyMode)
{
    Q_UNUSED(isCopyMode);
    screenCountChanged();
}

void FullScreenManager::screenCountChanged()
{
    const auto screens = qApp->screens();
    QList<ScreenPtr> activeScreens;
    const bool copyMode = isCopyMode();

    // Monitor every screen, including those without a window in mirror mode.
    for (auto *screen : screens) {
        connect(screen, &QScreen::geometryChanged, this, &FullScreenManager::delayHandleScreenChanged, Qt::UniqueConnection);
        connect(screen, &QScreen::availableGeometryChanged, this, &FullScreenManager::delayHandleScreenChanged, Qt::UniqueConnection);
        connect(screen, &QScreen::physicalSizeChanged, this, &FullScreenManager::delayHandleScreenChanged, Qt::UniqueConnection);
        connect(screen, &QScreen::physicalDotsPerInchChanged, this, &FullScreenManager::delayHandleScreenChanged, Qt::UniqueConnection);
        connect(screen, &QScreen::logicalDotsPerInchChanged, this, &FullScreenManager::delayHandleScreenChanged, Qt::UniqueConnection);
        connect(screen, &QScreen::virtualGeometryChanged, this, &FullScreenManager::delayHandleScreenChanged, Qt::UniqueConnection);
        connect(screen, &QScreen::primaryOrientationChanged, this, &FullScreenManager::delayHandleScreenChanged, Qt::UniqueConnection);
        connect(screen, &QScreen::orientationChanged, this, &FullScreenManager::delayHandleScreenChanged, Qt::UniqueConnection);
        connect(screen, &QScreen::refreshRateChanged, this, &FullScreenManager::delayHandleScreenChanged, Qt::UniqueConnection);
        if (!copyMode || activeScreens.isEmpty())
            activeScreens.append(screen);
    }

    // Keep unused windows for reuse, but clear the actual stored association.
    for (auto it = m_screenContents.begin(); it != m_screenContents.end(); ++it) {
        if (!activeScreens.contains(it.value())) {
            it.key()->hide();
            it.value() = nullptr;
        }
    }

    for (const auto &s : activeScreens) {
        if (m_screenContents.values().contains(s))
            continue;

        auto findContentNotUsed = [](const QMap<QWidget *, ScreenPtr> &map) -> QWidget * {
            QMapIterator<QWidget *, ScreenPtr> it(map);
            while (it.hasNext()) {
                it.next();
                if (!it.value() && it.key()) {
                    return it.key();
                }
            }
            return nullptr;
        };

        // try to find a content frame not used now
        QWidget *content = findContentNotUsed(m_screenContents);

        // or create new content frame
        if (!content) {
            content = m_registerFun(s);
        }

        m_screenContents.insert(content, s);
    }

    delayHandleScreenChanged();
}

void FullScreenManager::delayHandleScreenChanged()
{
    m_delayTimer->start(500);
}

void FullScreenManager::handleScreenChanged()
{
    checkCopyModeChanged();

    qInfo() << Q_FUNC_INFO << "screen contents size :" << m_screenContents.count();
    for (QWidget *w : m_screenContents.keys()) {
        auto inter = dynamic_cast<AbstractFullBackgroundInterface *>(w);
        if (inter && m_screenContents.value(w)) {
            inter->setScreen(m_screenContents.value(w));
        }
    }
}
