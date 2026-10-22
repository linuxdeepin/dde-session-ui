// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FULLSCREENMANAGER_H
#define FULLSCREENMANAGER_H

#include <QMap>
#include <QObject>
#include <QPointer>
#include <QScreen>
#include <QWidget>

#include <functional>

class QTimer;

class FullScreenManager : public QObject
{
    Q_OBJECT

public:
    explicit FullScreenManager(std::function<QWidget *(QScreen *)> function, QObject *parent = nullptr);
    ~FullScreenManager();

    typedef QPointer<QScreen> ScreenPtr;

Q_SIGNALS:
    void copyModeChanged(bool isCopyMode);

public Q_SLOTS:
    void screenCountChanged();
    void delayHandleScreenChanged();
    void handleScreenChanged();
    bool isCopyMode() const;
    void checkCopyModeChanged();
    void handleCopyModeChanged(bool isCopyMode);

private:
    std::function<QWidget *(QScreen *)> m_registerFun;
    QMap<QWidget *, ScreenPtr> m_screenContents;
    QTimer *m_delayTimer;
    bool m_copyModeFlag;
};

#endif // FULLSCREENMANAGER_H
