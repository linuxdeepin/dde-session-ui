/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *             listenerri <listenerri@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *             listenerri <listenerri@gmail.com>
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

#ifndef LOCKMANAGER_H
#define LOCKMANAGER_H

#include <QFrame>

#include "sessionbasewindow.h"

#include "timewidget.h"
#include "shutdownwidget.h"
#include "userwidget.h"
#include "controlwidget.h"
#include "kblayoutwidget.h"

#include "dbus/dbuslockservice.h"
#include "dbus/dbusinputdevices.h"
#include "dbus/dbuskeyboard.h"
#include "dbus/dbussessionmanager.h"
#include "dbus/dbushotzone.h"
#include "darrowrectangle.h"
#include "dbus/dbusmediaplayer2.h"
#include "xkbparser.h"

#include <dpasswdeditanimated.h>
#include <com_deepin_daemon_imageblur.h>

#define LOCKSERVICE_PATH "/com/deepin/dde/LockService"
#define LOCKSERVICE_NAME "com.deepin.dde.LockService"

#define LOCK_KEYBOARDLAYOUT_PATH "/com/deepin/daemon/InputDevice/Keyboard"
#define LOCK_KEYBOARDLAYOUT_NAME "com.deepin.daemon.InputDevice"

DWIDGET_USE_NAMESPACE

using ImageBlur = com::deepin::daemon::ImageBlur;

class LockManager : public QFrame
{
    Q_OBJECT

public:
    enum Actions {Unlock, Restart, Suspend, Shutdown};

public:
    LockManager(QWidget* parent = 0);

    void initUI();
    void initConnect();
    void initData();
    void updateUI();
    void enableZone();
    void disableZone();
    ControlWidget *control();

signals:
    void requestSetBackground(const QString &background);
#ifdef LOCK_NO_QUIT
    void checkedHide();
#endif
public slots:
    void setCurrentKeyboardLayout(QString keyboard_value);
    void passwordMode();
    void shutdownMode();
    void updateWidgetsPosition();

    void keybdLayoutWidgetPosit();

    void chooseUserMode();
    void onUnlockFinished(const bool unlocked);
//    void onUserUnlock(const QString& username);

protected:
    void mouseReleaseEvent(QMouseEvent *e) override;
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

private:
    void initBackend();
    void unlock();
    void lockServiceEvent(quint32 eventType, quint32 pid, const QString &username, const QString &message);
    bool checkUserIsNoPWGrp(User *user);
    void updatePasswordEditVisible(User *user);
    void onCurrentUserChanged(User *user);
    void switchToUser(User *user);
    void onBlurWallpaperFinished(const QString &source, const QString &blur, bool status);
    void backgroundChanged(const QString &path);

    // TODO: FIXME
    void saveUser(User *user);

private:
    enum UserState {
        Passwd,
        NoPasswd
    };


    enum LayoutState {
        UnlockState,
        PowerState,
        UsersState
    };

private:
    Actions m_action = Unlock;

    ShutdownWidget* m_requireShutdownWidget;
    TimeWidget *m_timeWidget;
    UserWidget *m_userWidget;
    ControlWidget *m_controlWidget;
    DPasswdEditAnimated *m_passwdEditAnim;
    QPushButton *m_unlockButton;
    DBusLockService *m_lockInter;

    QMap<QString, QString> m_keybdInfoMap;
    DBusKeyboard* m_keyboardLayoutInterface;
    KbLayoutWidget* m_keybdLayoutWidget;
    DArrowRectangle* m_keybdArrowWidget;
    DBusSessionManagerInterface* m_sessionManagerIter;

    DBusHotzone* m_hotZoneInterface;
    const QString m_activatedUser;

    DBusMediaPlayer2 *m_mprisInter = nullptr;
    bool m_authenticating = false;
    bool m_isThumbAuth = false;

    UserState m_userState;
    LayoutState m_layoutState;

    // refactor
    User *m_currentUser;
    ImageBlur *m_blurImageInter;
};

#endif // LOCKMANAGER_H
