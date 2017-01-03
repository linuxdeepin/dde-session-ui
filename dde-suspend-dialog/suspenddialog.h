/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SUSPENDDIALOG_H
#define SUSPENDDIALOG_H

#include <ddialog.h>

#include <QTimer>

DWIDGET_USE_NAMESPACE

class SuspendDialog : public DDialog
{
    Q_OBJECT
public:
    explicit SuspendDialog();
    ~SuspendDialog();

private:
    QTimer m_timer;
    int m_timerTick;
};

#endif // SUSPENDDIALOG_H
