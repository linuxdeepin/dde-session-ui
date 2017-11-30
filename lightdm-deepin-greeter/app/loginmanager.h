/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef LoginManager_H
#define LoginManager_H

#include <QApplication>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

#include <QLightDM/Greeter>
#include <QLightDM/SessionsModel>

#include "util_updateui.h"
#include "userwidget.h"
#include "passwdedit.h"
#include "view/logowidget.h"
#include "sessionwidget.h"
#include "kblayoutwidget.h"
#include "shutdownwidget.h"
#include "xkbparser.h"
#include "dbus/dbuskeyboard.h"
#include "keyboardmonitor.h"
#include "controlwidget.h"
#include "dbus/dbusdisplaymanager.h"
#include "dbus/dbusvariant.h"
#include "dbus/dbuslogin1manager.h"
#include "dbus/dbuslockservice.h"

#include "darrowrectangle.h"

class LoginManager: public QFrame
{
    Q_OBJECT
public:
    LoginManager(QWidget* parent=0);

signals:
    void requestBackground(const QString &background) const;
    void requestBackground(const QPixmap &background) const;

public slots:
    /*Update the position of the widgets after finished the layout of ui*/
    void updateWidgetsPosition();
    void updateBackground(QString username);
    void updateUserLoginCondition(QString username);

    void authenticate();
    void startSession();

protected:
    void resizeEvent(QResizeEvent *e) override;
    void keyPressEvent(QKeyEvent* e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;

private:
//    void recordPid();
    void initUI();
    void initData();
    void initConnect();

    void initDateAndUpdate();

    void expandUserWidget();

    void message(QString text, QLightDM::Greeter::MessageType type);
    void prompt(QString text, QLightDM::Greeter::PromptType type);
    void authenticationComplete();

    void login();

    void chooseUserMode();
    void chooseSessionMode();
    void choosedSession();

    void showShutdownFrame();
    void keyboardLayoutUI();
    void keybdLayoutWidgetPosit();
    void setCurrentKeybdLayoutList(QString keyboard_value);

    void setShutdownAction(const ShutdownWidget::Actions action);
    void leftKeyPressed();
    void rightKeyPressed();

    void saveNumlockStatus(const bool &on);
    void restoreNumlockStatus();

    LogoWidget* m_logoWidget;
    UserWidget* m_userWidget;
    PassWdEdit* m_passWdEdit;
    SessionWidget *m_sessionWidget;
    QVBoxLayout* m_Layout;

    ShutdownWidget* m_requireShutdownWidget;
    KbLayoutWidget* m_keybdLayoutWidget;
    DArrowRectangle* m_keybdArrowWidget;

    QLightDM::Greeter *m_greeter;
    DBusLogin1Manager* m_login1ManagerInterface;

    KeyboardMonitor *m_keyboardMonitor;

    ControlWidget *m_controlWidget;

    QString m_lastUser;
    QPushButton *m_loginButton;
    QSize m_passwdEditSize;
    QHBoxLayout *m_passWdEditLayout;
    bool m_isThumbAuth = false;
};
#endif // LoginManager

