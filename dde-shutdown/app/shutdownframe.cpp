/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>

#include "shutdownframe.h"

const QString WallpaperKey = "pictureUri";

ShutdownFrame::ShutdownFrame(QWidget *parent)
    : BoxFrame(parent),
      m_wmInter(new com::deepin::wm("com.deepin.wm", "/com/deepin/wm", QDBusConnection::sessionBus(), this))
{
    this->move(0, 0);
    this->setFixedSize(qApp->desktop()->size());

    qDebug() << "this geometry:" << geometry();

    m_shutdownManager = new ShutdownManager(this);
    initShutdownManager();
    initBackground();
}

void ShutdownFrame::updateScreenPosition() {
    m_shutdownManager->updateGeometry();
}

void ShutdownFrame::powerAction(const Actions action)
{
    m_shutdownManager->powerActionFromExternal(action);
}

void ShutdownFrame::setConfirm(const bool confrim)
{
    m_shutdownManager->setConfrim(confrim);
}

void ShutdownFrame::keyPressEvent(QKeyEvent *e) {
    Q_UNUSED(e);
}

void ShutdownFrame::showEvent(QShowEvent *event)
{
    BoxFrame::showEvent(event);

    initShutdownManager();
}

void ShutdownFrame::initShutdownManager()
{
    QPoint mousePoint = QCursor::pos();

    QList<QScreen *> screenList = qApp->screens();
    for (int i = 0; i < screenList.length(); i++) {
        const QRect rect = screenList[i]->geometry();
        if (rect.contains(mousePoint)) {
            m_shutdownManager->setFixedSize(rect.size());
            m_shutdownManager->move(rect.x(), rect.y());
            qDebug() << "shutdownManager:" << m_shutdownManager->geometry();
            updateScreenPosition();
            continue;
        }
    }
}

void ShutdownFrame::initBackground()
{
    auto callback = [this] (int, int) {
        QDBusPendingCall call = m_wmInter->GetCurrentWorkspaceBackground();
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
        connect(watcher, &QDBusPendingCallWatcher::finished, [this, call] {
            if (!call. isError()) {
                QDBusReply<QString> reply = call.reply();
                const QString uri = reply.value();
                const QString background = QUrl(uri).toLocalFile();
                setBackground(background);
            } else {
                qWarning() << "get current workspace background error: " << call.error().message();
            }
        });
    };

    callback(0, 0);
    connect(m_wmInter, &__wm::WorkspaceSwitched, callback);

    m_dbusAppearance = new Appearance("com.deepin.daemon.Appearance",
                                      "/com/deepin/daemon/Appearance",
                                      QDBusConnection::sessionBus(),
                                      this);

    connect(m_dbusAppearance, &Appearance::Changed, this, [=](const QString &type, const QString &path){
        if (type == "background") {
            setBackground(path);
        }
    });
}

ShutdownFrame::~ShutdownFrame()
{
}

ShutdownFrontDBus::ShutdownFrontDBus(ShutdownFrame *parent):
    QDBusAbstractAdaptor(parent),
    m_parent(parent)
{
}

ShutdownFrontDBus::~ShutdownFrontDBus()
{
}

void ShutdownFrontDBus::Ping()
{
}

void ShutdownFrontDBus::Shutdown()
{
    m_parent->setConfirm(true);
    m_parent->powerAction(Actions::Shutdown);
    m_parent->show();
}

void ShutdownFrontDBus::Restart()
{
    m_parent->setConfirm(true);
    m_parent->powerAction(Actions::Restart);
    m_parent->show();
}

void ShutdownFrontDBus::Logout()
{
    m_parent->setConfirm(true);
    m_parent->powerAction(Actions::Logout);
    m_parent->show();
}

void ShutdownFrontDBus::Suspend()
{
    m_parent->powerAction(Actions::Suspend);
}

void ShutdownFrontDBus::SwitchUser()
{
    m_parent->powerAction(Actions::SwitchUser);
}

void ShutdownFrontDBus::Show()
{
    if (m_parent) {
        m_parent->show();
    }
}
