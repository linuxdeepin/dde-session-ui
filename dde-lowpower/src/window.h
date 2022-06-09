/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
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

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDBusAbstractAdaptor>
#include <QDBusInterface>
#include <QDBusConnection>

class QLabel;
class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();
    bool SleepLock();

private:
    QLabel * m_image;
    QLabel * m_text;
    bool m_bSleepLock;

    void setupSize();
    void setupUI();
public slots:
    void HideShowToRaise(bool visible);
};

class LowPowerAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.LowPower")

public:
    explicit LowPowerAdaptor(Window * parent);
    ~LowPowerAdaptor();

public slots:
    void Raise();
    void Quit();
};

#endif // WINDOW_H
