#include "systemmonitor.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPainter>
#include <QtSvg/QSvgRenderer>
#include <QPaintEvent>
#include <QApplication>
#include <QScreen>
#include <QDebug>

SystemMonitor::SystemMonitor(QWidget *parent) : QWidget(parent)
{
    m_state = Leave;

    const qreal ratio = qApp->primaryScreen()->devicePixelRatio();

    m_icon = new QWidget;
    m_icon->installEventFilter(this);
    m_icon->setFixedSize(24 * ratio, 24 * ratio);

    m_text = new QLabel(tr("Start system monitor"));
    m_text->setStyleSheet("color: white;"
                          "font-size: 14px;"
                          "font-weight: 400;");

    setFixedHeight(40 * ratio);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setContentsMargins(30, 0, 30, 0);

    layout->addWidget(m_icon, 0, Qt::AlignVCenter);
    layout->addSpacing(10);
    layout->addWidget(m_text, 0, Qt::AlignVCenter);

    setLayout(layout);
}

void SystemMonitor::setState(SystemMonitor::State state)
{
    m_state = state;
    update();
}

void SystemMonitor::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    m_text->setStyleSheet("color: white;"
                          "font-size: 14px;"
                          "font-weight: 400;");

    m_state = Enter;
    update();
}

void SystemMonitor::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    m_text->setStyleSheet("color: white;"
                          "font-size: 14px;"
                          "font-weight: 400;");

    m_state = Leave;
    update();
}

void SystemMonitor::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    m_text->setStyleSheet("color: white;"
                          "font-size: 14px;"
                          "font-weight: 400;");

    m_state = Release;
    update();

    emit clicked();
}

void SystemMonitor::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    m_text->setStyleSheet("color: #2ca7f8;"
                          "font-size: 14px;"
                          "font-weight: 400;");

    m_state = Press;
    update();

    event->accept();
}

bool SystemMonitor::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_icon) {
        if (event->type() == QEvent::Paint) {
            QPainter painter(m_icon);
            const QString pix(":/img/deepin-system-monitor.svg");
            QSvgRenderer render(pix);
            render.render(&painter, m_icon->rect());
        }
    }

    return false;
}

void SystemMonitor::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPainterPath path;
    path.addRoundedRect(rect(), 5, 5);
    painter.setClipPath(path);

    switch(m_state) {
    case Enter:
    case Release:{
        painter.fillRect(rect(), QColor(255, 255, 255, 0.1 * 255));
        break;
    }
    case Press: {
        painter.fillRect(rect(), QColor(255, 255, 255, 0.05 * 255));
        break;
    }
    case Leave:
    default:
        break;
    }

}

void SystemMonitor::keyPressEvent(QKeyEvent *event)
{
    if (m_state == Enter && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)) {
        event->accept();
        emit clicked();
    }
}
