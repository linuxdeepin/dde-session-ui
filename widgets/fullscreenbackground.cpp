#include "fullscreenbackground.h"

#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QPainter>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>
#include <QKeyEvent>

FullscreenBackground::FullscreenBackground(QWidget *parent)
    : QWidget(parent),

      m_adjustTimer(new QTimer(this))
{
    m_adjustTimer->setSingleShot(true);

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    connect(m_adjustTimer, &QTimer::timeout, this, &FullscreenBackground::adjustGeometry);
}

void FullscreenBackground::setBackground(const QString &file)
{
    const QUrl url(file);
    if (url.isLocalFile())
        return setBackground(url.path());

    Q_ASSERT(QFileInfo(file).isFile());

    setBackground(QPixmap(file));
}

void FullscreenBackground::setBackground(const QPixmap &pixmap)
{
    Q_ASSERT(!pixmap.isNull());

    m_background = pixmap;
}

void FullscreenBackground::setContent(QWidget * const w)
{
    Q_ASSERT(m_content.isNull());

    m_content = w;
    m_content->installEventFilter(this);

    m_adjustTimer->start();
}

void FullscreenBackground::adjustGeometry()
{
    const QPoint cp(QCursor::pos());
    QRect r, pr;
    for (const auto *s : qApp->screens())
    {
        const QRect sr = s->geometry();
        if (sr.contains(cp))
            pr = sr;

        r = r.united(sr);
    }

    QWidget::setGeometry(r);

    if (!m_content.isNull())
        m_content->setGeometry(pr);
}

bool FullscreenBackground::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_content && event->type() == QEvent::Leave)
        m_adjustTimer->start();

    return QWidget::eventFilter(watched, event);
}

void FullscreenBackground::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    m_adjustTimer->start();
}

void FullscreenBackground::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    for (auto *s : qApp->screens())
        painter.drawPixmap(s->geometry(), m_background);
}

void FullscreenBackground::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e);

    switch (e->key())
    {
#ifdef QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    default:;
    }
}
