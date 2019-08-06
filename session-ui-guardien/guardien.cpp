/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
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

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include "guardien.h"

Guardien::Guardien(QObject *parent) :
    QObject(parent)
{
    m_lock = new QProcess(this);
    m_lock->setProgram("dde-lock");
    m_lock->setArguments(QStringList("--daemon"));

    connect(m_lock, SIGNAL(finished(int)), this, SLOT(handleFinished(int)));
}

void Guardien::start()
{
    m_lock->start();
}

void Guardien::restart()
{
    m_lock->kill();
}

void Guardien::handleFinished(int)
{
    QProcess *p = qobject_cast<QProcess*>(sender());
    if (p == m_lock) {
        m_lock->start();
    }
}
