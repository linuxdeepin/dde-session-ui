#include "fullscreenbackground.h"

#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>

FullscreenBackground::FullscreenBackground(QWidget *parent)
    : QWidget(parent),

      m_adjustTimer(new QTimer(this))
{
    m_adjustTimer->setSingleShot(true);

    connect(m_adjustTimer, &QTimer::timeout, this, &FullscreenBackground::adjustGeometry);
}

void FullscreenBackground::setContent(QWidget * const w)
{
    Q_ASSERT(m_content.isNull());

    m_content = w;

    m_adjustTimer->start();
}

void FullscreenBackground::adjustGeometry()
{
    QRect r;
    for (const auto *s : qApp->screens())
        r.intersected(s->geometry());

    QWidget::setGeometry(r);

    if (!m_content.isNull())
        m_content->setGeometry(qApp->primaryScreen()->geometry());
}

void FullscreenBackground::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    m_adjustTimer->start();
}
