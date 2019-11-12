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
#include <QDebug>
#include <QProcess>

BubbleWidget_Bg::BubbleWidget_Bg(QWidget *parent)
    : DWidget(parent)
{
    installEventFilter(this);
}

bool BubbleWidget_Bg::eventFilter(QObject *obj, QEvent *event)
{
    // 这里只是为了让界面不被拖动
    if (obj) {
        if (event->type() == QEvent::MouseMove
                || event->type() == QEvent::Move) {
            return true;
        }
    }
    return false;
}

void BubbleWidget_Bg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPalette pe = this->palette();
    QColor brushColor(pe.color(QPalette::Base));
    brushColor.setAlpha(m_hover ? m_hoverAlpha : m_unHoverAlpha);
    painter.setBrush(brushColor);

    DStyleHelper dstyle(style());
    int radius = dstyle.pixelMetric(DStyle::PM_FrameRadius);

    QPen borderPen;
    borderPen.setColor(Qt::transparent);
    painter.setPen(borderPen);

    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, radius, radius);

    return DWidget::paintEvent(event);

}

void BubbleWidget_Bg::enterEvent(QEvent *event)
{
    m_hover = true;

    update();

    return DWidget::enterEvent(event);
}

void BubbleWidget_Bg::leaveEvent(QEvent *event)
{
    m_hover = false;

    update();

    return DWidget::leaveEvent(event);
}

BubbleItem::BubbleItem(QWidget *parent, std::shared_ptr<NotificationEntity> entity)
    : QWidget(parent)
    , m_entity(entity)
    , m_refreshTimer(new QTimer)
    , m_bgWidget(new BubbleWidget_Bg(this))
    , m_titleWidget(new BubbleWidget_Bg(this))
    , m_bodyWidget(new BubbleWidget_Bg(this))
    , m_appNameLabel(new DLabel(this))
    , m_appTimeLabel(new DLabel(this))
    , m_icon(new AppIcon(this))
    , m_body(new AppBody(this))
    , m_actionButton(new ActionButton(this))
    , m_closeButton(new Button(this))

{
    initUI();
    initContent();
}

void BubbleItem::initUI()
{
    setWindowFlags(Qt::Widget);
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
    m_appNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_appTimeLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    processIconData();
    processActions();
}

void BubbleItem::initContent()
{
    if (m_entity == nullptr) return;

    m_body->setTitle(m_entity->summary());
    m_body->setText(m_entity->body());
    m_appNameLabel->setText(m_entity->appName());

    m_refreshTimer->setSingleShot(false);
    m_refreshTimer->start();
    connect(m_refreshTimer, &QTimer::timeout, this, &BubbleItem::onRefreshTime);
    onRefreshTime();

    connect(this, &BubbleItem::havorStateChanged, this, &BubbleItem::onHavorStateChanged);
    connect(m_actionButton, &ActionButton::buttonClicked, this, &BubbleItem::onActionButtonClicked);
    connect(m_closeButton, &Button::clicked, this, &BubbleItem::onCloseBubble);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &BubbleItem::refreshTheme);
    refreshTheme();
}

void BubbleItem::setAlpha(int alpha)
{
    m_titleWidget->setAlpha(alpha);
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
            text = tr("%1 minutes ago").arg(minute);
        } else {
            m_refreshTimer->setInterval(1000 * 60 * 59);
            text = tr("%1 hours ago").arg(minute / 60);
        }
    } else if (elapsedDay == 1) {
        m_refreshTimer->setInterval(1000 * 59);
        text = tr("Yesterday ") + bubbleDateTime.toString("hh:mm");
    } else if (elapsedDay >= 1 && elapsedDay < 7) {
        m_refreshTimer->setInterval(1000 * 60 * 60 * 24);
        text = bubbleDateTime.toString("dddd hh:mm");
    } else {
        text = bubbleDateTime.toString("yyyy/MM/dd");
    }

    m_appTimeLabel->setText(text);
}

void BubbleItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_notifyModel != nullptr && m_entity != nullptr)
        m_notifyModel->expandData(m_entity);

    QWidget::mouseReleaseEvent(event);
}

void BubbleItem::enterEvent(QEvent *event)
{
    Q_EMIT havorStateChanged(true);
    return QWidget::enterEvent(event);
}

void BubbleItem::leaveEvent(QEvent *event)
{
    Q_EMIT havorStateChanged(false);
    return QWidget::leaveEvent(event);
}

void BubbleItem::onHavorStateChanged(bool hover)
{
    if (m_showContent) {
        m_closeButton->setVisible(hover);
        m_appTimeLabel->setVisible(!hover);
    }
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

void BubbleItem::setShowContent(bool visible)
{
    m_icon->setVisible(visible);
    m_body->setVisible(visible);
    m_appNameLabel->setVisible(visible);
    m_appTimeLabel->setVisible(visible);
    m_actionButton->setVisible(visible);
    m_closeButton->setVisible(visible);

    m_showContent = visible;
}

void BubbleItem::refreshTheme()
{
    QPalette pa = m_appNameLabel->palette();
    pa.setBrush(QPalette::WindowText, pa.brightText());
    pa.setColor(QPalette::WindowText, Qt::blue);
    m_appNameLabel->setPalette(pa);
    m_appTimeLabel->setFont(DFontSizeManager::instance()->t8());
}

// Each even element in the list (starting at index 0) represents the identifier for the action.
// Each odd element in the list is the localized string that will be displayed to the user.
void BubbleItem::processActions()
{
    m_actionButton->clear();

    QStringList list = m_entity->actions();
    // the "default" is identifier for the default action
    if (list.contains("default")) {
        const int index = list.indexOf("default");
        m_defaultAction = list[index];
        // Default action does not need to be displayed, removed from the list
        list.removeAt(index + 1);
        list.removeAt(index);
    }

    m_actionButton->addButtons(list);
}

const QPixmap BubbleItem::converToPixmap(const QDBusArgument &value)
{
    // use plasma notify source code to conver photo, solving encoded question.
    const QImage &img = BubbleAbStract::decodeNotificationSpecImageHint(value);
    return QPixmap::fromImage(img).scaled(m_icon->width(), m_icon->height(),
                                          Qt::KeepAspectRatioByExpanding,
                                          Qt::SmoothTransformation);
}

void BubbleItem::processIconData()
{
    const QVariantMap &hints = m_entity->hints();
    QString imagePath;
    QPixmap imagePixmap;

    for (const QString &hint : HintsOrder) {
        const QVariant &source = hints.contains(hint) ? hints[hint] : QVariant();

        if (source.isNull()) continue;

        if (source.canConvert<QDBusArgument>()) {
            QDBusArgument argument = source.value<QDBusArgument>();
            imagePixmap = converToPixmap(argument);
            break;
        }

        imagePath = source.toString();
    }

    if (!imagePixmap.isNull()) {
        m_icon->setPixmap(imagePixmap);
    } else {
        m_icon->setIcon(imagePath.isEmpty() ? m_entity->appIcon() : imagePath, m_entity->appName());
    }
}

void BubbleItem::onActionButtonClicked(const QString &actionId)
{
    qDebug() << "actionId:" << actionId;
    QMap<QString, QVariant> hints = m_entity->hints();
    QMap<QString, QVariant>::const_iterator i = hints.constBegin();
    while (i != hints.constEnd()) {
        QStringList args = i.value().toString().split(",");
        if (!args.isEmpty()) {
            QString cmd = args.first();
            args.removeFirst();
            if (i.key() == "x-deepin-action-" + actionId) {
                QProcess::startDetached(cmd, args);
            }
        }
        ++i;
    }
}

