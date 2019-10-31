#include "bubbleitem.h"
#include "notification/notificationentity.h"
#include "notification/button.h"
#include "notifycommon.h"
#include <QTimer>
#include <QDateTime>

BubbleItem::BubbleItem(QWidget *parent, std::shared_ptr<NotificationEntity> entity)
    : Bubble(parent, entity, OSD::ShowStyle::BUBBLEWIDGET)
    , m_refreshTimer(new QTimer)
{
    setWindowFlags(Qt::Widget);
    setFixedSize(Notify::BubbleItemWidth, Notify::BubbleItemHeight);

    m_refreshTimer->setSingleShot(false);
    m_refreshTimer->start();
    connect(m_refreshTimer, &QTimer::timeout, this, &BubbleItem::onRefreshTime);
    onRefreshTime();

    QFont font;
    font.setPointSize(10);
    m_appTimeLabel->setFont(font);

    QPalette pa;
    pa.setBrush(QPalette::WindowText, pa.brightText());
    m_appNameLabel->setPalette(pa);

    connect(m_closeButton, &Button::clicked, this, &BubbleItem::closeBubble);
}

void BubbleItem::onRefreshTime()
{
    qint64 msec = QDateTime::currentMSecsSinceEpoch() - m_entity->ctime().toLongLong();
    if (msec < 0) {
        return;
    }

    QString text;

    QDateTime bubbleDateTime = QDateTime::fromMSecsSinceEpoch(m_entity->ctime().toLongLong());
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int elapsedDay = int(bubbleDateTime.daysTo(currentDateTime));
    int minute = int(msec / 1000 / 60);

    if (elapsedDay == 0) {
        if (minute == 0) {
            text =  tr("Just Now");
            m_refreshTimer->setInterval(1000 * 3);
        } else if (minute > 0 && minute < 60) {
            m_refreshTimer->setInterval(1000 * 59);
            text = QString::number(minute) + tr(" Minute Ago");
        } else {
            m_refreshTimer->setInterval(1000 * 60 * 59);
            text = QString::number(minute / 60) + tr(" Hour Ago");
        }
    } else if (elapsedDay == 1) {
        m_refreshTimer->setInterval(1000 * 59);
        text = tr("Yesterday ") + bubbleDateTime.toString("hh:mm");
    } else {
        m_refreshTimer->setInterval(1000 * 60 * 60 * 24);
        text = QString::number(elapsedDay) + tr(" Day Ago");
    }

    m_appTimeLabel->setText(text);
}
