#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include "guardien.h"

Guardien::Guardien(QObject *parent) :
    QObject(parent)
{
    m_shutdown = new QProcess(this);
    m_shutdown->setProgram("dde-shutdown");
    m_shutdown->setArguments(QStringList("--daemon"));

    m_lock = new QProcess(this);
    m_lock->setProgram("dde-lock");
    m_lock->setArguments(QStringList("--daemon"));

    QDesktopWidget *desktop = QApplication::desktop();
    connect(desktop, &QDesktopWidget::resized, this, &Guardien::restart);
    connect(desktop, &QDesktopWidget::workAreaResized, this, &Guardien::restart);

    connect(m_shutdown, SIGNAL(finished(int)), this, SLOT(handleFinished(int)));
    connect(m_lock, SIGNAL(finished(int)), this, SLOT(handleFinished(int)));
}

void Guardien::start()
{
    m_shutdown->start();
    m_lock->start();
}

void Guardien::restart()
{
    m_shutdown->kill();
    m_lock->kill();
}

void Guardien::handleFinished(int)
{
    QProcess *p = qobject_cast<QProcess*>(sender());
    if (p == m_shutdown) {
        m_shutdown->start();
    } else if (p == m_lock) {
        m_lock->start();
    }
}
