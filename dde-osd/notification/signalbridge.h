/*
 * Copyright (C) 2016 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     yinjie <yinjie@uniontech.com>
 *
 * Maintainer: yinjie <yinjie@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SIGNALBRIDGE_H
#define SIGNALBRIDGE_H

#include <QObject>
#include <DSingleton>

class SignalBridge : public QObject, public Dtk::Core::DSingleton<SignalBridge>
{
    Q_OBJECT
    friend class Dtk::Core::DSingleton<SignalBridge>;
private:
    explicit SignalBridge(QObject *parent = nullptr) : QObject(parent) {}

signals:
    /**
     * @brief actionInvoked 提醒action已经执行
     */
    void actionInvoked(uint, const QString &);
};

#endif // SIGNALBRIDGE_H
