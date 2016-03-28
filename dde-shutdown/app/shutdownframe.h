/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SHUTDOWNFRAME
#define SHUTDOWNFRAME

#include <QFrame>
#include "boxframe.h"
#include "shutdownmanager.h"

class ShutdownFrontDBus;
class ShutdownFrame:public BoxFrame
{
    Q_OBJECT
public:
    ShutdownFrame(QWidget* parent = 0);
    ~ShutdownFrame();
public slots:
    void updateScreenPosition();
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    ShutdownManager* m_shutdownManager;

    void initShutdownManager();
    void initBackground();
};

class ShutdownFrontDBus : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.shutdownFront")

public:
    ShutdownFrontDBus(ShutdownFrame* parent);
    ~ShutdownFrontDBus();

    Q_SLOT void Ping();
    Q_SLOT void Show();

private:
    ShutdownFrame* m_parent;
};
#endif // SHUTDOWNFRAME

