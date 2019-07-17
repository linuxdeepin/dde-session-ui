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

#ifndef CONTENTVIEWWIDGET
#define CONTENTVIEWWIDGET
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <memory>

#include <com_deepin_wm.h>
#include <com_deepin_daemon_appearance.h>
#include <com_deepin_daemon_imageblur.h>

#include "rounditembutton.h"
#include "util_updateui.h"
#include "dbus/dbusvariant.h"
#include "dbus/dbuslogin1manager.h"
#include "common.h"
#include "dbus/dbussessionmanager.h"
#include "systemmonitor.h"
#include "warningview.h"
#include "inhibitwarnview.h"

using Appearance = com::deepin::daemon::Appearance;
using ImageBlur = com::deepin::daemon::ImageBlur;

class MultiUsersWarningView;
class SessionBaseModel;
class User;
class DBusControlCenter;
class ContentWidget: public QFrame
{
    Q_OBJECT
public:
    ContentWidget(QWidget* parent = nullptr);
    void setModel(SessionBaseModel * const model);
    ~ContentWidget() override;

signals:
    void requestBackground(const QString &path) const;

protected:
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

public slots:
    void setConfirm(const bool confirm);
    bool powerAction(const Actions action);
    void setPreviousChildFocus();
    void setNextChildFocus();
    void showTips(const QString &tips);
    void hideBtns(const QStringList &btnsName);
    void disableBtns(const QStringList &btnsName);
    void onCancel();
    QList<InhibitWarnView::InhibitorData> listInhibitors(const Actions action);
    void recoveryLayout();
    void runSystemMonitor();

private:
    void initUI();
    void initConnect();
    void initBackground();
    void initData();
    void enterKeyPushed();
    void hideBtn(const QString &btnName);
    void disableBtn(const QString &btnName);
    bool beforeInvokeAction(const Actions action);
    void hideToplevelWindow();
    void shutDownFrameActions(const Actions action);

    void currentWorkspaceChanged();
    void updateWallpaper(const QString &path);
    void onBlurWallpaperFinished(const QString &source, const QString &blur, bool status);
    void onUserListChanged(QList<std::shared_ptr<User>> list);
    void enableHibernateBtn(bool enable);
    void enableSleepBtn(bool enable);

    RoundItemButton *m_currentSelectedBtn = nullptr;
    RoundItemButton *m_shutdownButton;
    RoundItemButton *m_restartButton;
    RoundItemButton *m_suspendButton;
    RoundItemButton *m_hibernateButton;
    RoundItemButton *m_lockButton;
    RoundItemButton *m_logoutButton;
    RoundItemButton *m_switchUserBtn;
    QList<RoundItemButton *> *m_btnsList;

    QWidget *m_tipsWidget;
    QLabel *m_tipsLabel;
    DBusLogin1Manager* m_login1Inter;
    DBusControlCenter* m_controlCenterInter;

    WarningView *m_warningView = nullptr;
    QWidget *m_normalView = nullptr;
    QSpacerItem *m_buttonSpacer = nullptr;
    QStackedLayout *m_mainLayout;

    bool m_confirm = false;

    DBusSessionManagerInterface* m_sessionInterface = nullptr;
    SystemMonitor *m_systemMonitor;
    com::deepin::wm *m_wmInter;
    Appearance *m_dbusAppearance = nullptr;
    ImageBlur *m_blurImageInter;
    SessionBaseModel *m_model;
};
#endif // CONTENTVIEWWIDGET
