#include "fullscreenbackground.h"

#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QPainter>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>

FullscreenBackground::FullscreenBackground(QWidget *parent)
    : QWidget(parent),

      m_adjustTimer(new QTimer(this))
{
    m_adjustTimer->setSingleShot(true);

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    connect(m_adjustTimer, &QTimer::timeout, this, &FullscreenBackground::adjustGeometry);
}

const QRect FullscreenBackground::primaryRect() const
{
    return qApp->primaryScreen()->geometry();
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
    QRect r;
    for (const auto *s : qApp->screens())
        r.intersected(s->geometry());

    QWidget::setGeometry(r);

    const QRect pr(qApp->primaryScreen()->geometry());

    if (!m_content.isNull())
        m_content->setGeometry(pr);

    emit primaryRectChanged(pr);
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
