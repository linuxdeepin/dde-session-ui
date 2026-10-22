// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ABSTRACTFULLBACKGROUNDINTERFACE_H
#define ABSTRACTFULLBACKGROUNDINTERFACE_H

#include <QPointer>
#include <QScreen>

class AbstractFullBackgroundInterface
{
public:
    virtual ~AbstractFullBackgroundInterface() = default;

    virtual void setScreen(QPointer<QScreen> screen, bool isVisible = true) = 0;
};

#endif // ABSTRACTFULLBACKGROUNDINTERFACE_H
