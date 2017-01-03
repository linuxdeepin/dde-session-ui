/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "suspenddialog.h"

SuspendDialog::SuspendDialog()
    : DDialog(tr("External monitor detected, suspend?"), tr("%1s").arg(15)),
      m_timerTick(0)
{
    QIcon icon( QIcon::fromTheme("system-settings") );
    setIconPixmap(icon.pixmap(48, 48));

    QStringList buttons;
    buttons << tr("Cancel") << tr("Suspend");
    addButtons(buttons);

    m_timer.setInterval(1000);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        m_timerTick++;
        if (m_timerTick >= 15) {
            done(1);
        } else {
            setMessage(tr("%1s").arg(15 - m_timerTick));
        }
    });

    m_timer.start();
}

SuspendDialog::~SuspendDialog()
{

}
