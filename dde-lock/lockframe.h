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

#ifndef LOCKFRAME
#define LOCKFRAME

#include <QKeyEvent>
#include <QDBusConnection>
#include <QDBusAbstractAdaptor>

#include "fullscreenbackground.h"
#include "lockmanager.h"

const QString DBUS_PATH = "/com/deepin/dde/lockFront";
const QString DBUS_NAME = "com.deepin.dde.lockFront";

class DBusLockService;
class SessionBaseModel;
class LockContent;
class LockFrame: public FullscreenBackground
{
    Q_OBJECT
public:
    LockFrame(SessionBaseModel * const model, QWidget* parent=0);
    ~LockFrame();

signals:
    void requestAuthUser(std::shared_ptr<User> user, const QString &password);
    void requestSwitchToUser(std::shared_ptr<User> user);
    void requestSetLayout(std::shared_ptr<User> user, const QString &layout);

public slots:
    void showUserList();
    void tryGrabKeyboard();
#ifdef LOCK_NO_QUIT
    void hideFrame();
#endif
protected:
    void keyPressEvent(QKeyEvent *e);

private:
    LockContent *m_content;
    Display *m_display;
    SessionBaseModel *m_model;
    int m_failures = 0;
};

#endif // LOCKFRAME

