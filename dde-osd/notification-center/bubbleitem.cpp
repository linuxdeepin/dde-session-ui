#include "bubbleitem.h"
#include "notification/notificationentity.h"
#include "notification/appicon.h"
#include "notification/appbody.h"
#include "notification/actionbutton.h"
#include "notification/button.h"
#include "notification/icondata.h"
#include "notifymodel.h"

#include <QTimer>
#include <QDateTime>
#include <DStyleHelper>
#include <QPropertyAnimation>
#include <DGuiApplicationHelper>
#include <DFontSizeManager>

BubbleItem::BubbleItem(QWidget *parent, std::shared_ptr<NotificationEntity> entity)
    : BubbleAbStract(parent, entity)
    , m_refreshTimer(new QTimer)
{
    initUI();
    initContent();
}

void BubbleItem::initUI()
{
    setWindowFlags(Qt::Widget);
    setFixedSize(OSD::BubbleSize(OSD::ShowStyle::BUBBLEWIDGET));

    DStyleHelper dstyle(style());
    int radius = dstyle.pixelMetric(DStyle::PM_FrameRadius);
    setBlurRectXRadius(radius);
    setBlurRectYRadius(radius);
    m_handle->setShadowRadius(radius);

    setFixedSize(OSD::BubbleSize(OSD::BUBBLEWIDGET));
    m_icon->setFixedSize(OSD::IconSize(OSD::BUBBLEWIDGET));
    m_closeButton->setFixedSize(OSD::CloseButtonSize(OSD::BUBBLEWIDGET));

    m_titleWidget->setFixedHeight(37);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setSpacing(10);
    titleLayout->setContentsMargins(10, 0, 10, 0);
    titleLayout->addWidget(m_icon);
    titleLayout->addWidget(m_appNameLabel);
    titleLayout->addWidget(m_appTimeLabel);

    m_appNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_appTimeLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    m_closeButton->setRadius(99);
    m_closeButton->setText("X");
    m_closeButton->setVisible(false);
    setAlpha(20);
    titleLayout->addWidget(m_closeButton);
    m_titleWidget->setLayout(titleLayout);
    mainLayout->addWidget(m_titleWidget);
    m_body->setStyle(OSD::BUBBLEWIDGET);

    QHBoxLayout *bodyLayout = new QHBoxLayout;
    bodyLayout->setSpacing(0);
    bodyLayout->setContentsMargins(10, 0, 10, 0);
    bodyLayout->addWidget(m_body);
    bodyLayout->addWidget(m_actionButton);

    m_bodyWidget->setLayout(bodyLayout);
    mainLayout->addWidget(m_bodyWidget);
    m_bgWidget->setLayout(mainLayout);

    QHBoxLayout *l = new QHBoxLayout;
    l->setSpacing(0);
    l->setMargin(0);
    l->addWidget(m_bgWidget);
    setLayout(l);

    m_actionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_closeButton, &Button::clicked, this, &BubbleItem::onCloseBubble);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &BubbleItem::refreshTheme);
    refreshTheme();
}

void BubbleItem::initContent()
{
    if (m_entity == nullptr) return;

    m_refreshTimer->setSingleShot(false);
    m_refreshTimer->start();
    connect(m_refreshTimer, &QTimer::timeout, this, &BubbleItem::onRefreshTime);
    onRefreshTime();

    updateContent();
    connect(this, &BubbleItem::havorStateChanged, this, &BubbleItem::onHavorStateChanged);
}

void BubbleItem::setAlpha(int alpha)
{
    m_titleWidget->setAlpha(alpha);
    m_titleWidget->setHoverAlpha(0);
    m_titleWidget->setUnHoverAlpha(0);
    m_bodyWidget->setAlpha(0);
    m_bgWidget->setHoverAlpha(alpha * 4);
    m_bgWidget->setUnHoverAlpha(alpha * 3);
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

void BubbleItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_notifyModel != nullptr && m_entity != nullptr)
        m_notifyModel->expandData(m_entity);

    BubbleAbStract::mouseReleaseEvent(event);
}

void BubbleItem::enterEvent(QEvent *event)
{
    Q_EMIT havorStateChanged(true);

    return BubbleAbStract::enterEvent(event);
}

void BubbleItem::leaveEvent(QEvent *event)
{
    Q_EMIT havorStateChanged(false);

    return BubbleAbStract::leaveEvent(event);
}

void BubbleItem::onHavorStateChanged(bool hover)
{
    m_closeButton->setVisible(hover);
    m_appTimeLabel->setVisible(!hover);
}

void BubbleItem::onCloseBubble()
{
    if (m_notifyModel != nullptr)
        m_notifyModel->removeNotify(m_entity);
}

void BubbleItem::setModel(NotifyModel *model)
{
    Q_ASSERT(model);
    m_notifyModel = model;
}

void BubbleItem::clearContent()
{
    m_body->setTitle("");
    m_body->setText("");
    m_appNameLabel->setText("");
}

void BubbleItem::refreshTheme()
{
    QPalette pa = m_appNameLabel->palette();
    pa.setBrush(QPalette::WindowText, pa.brightText());
    m_appNameLabel->setPalette(pa);
    m_appTimeLabel->setFont(DFontSizeManager::instance()->t8());
}
