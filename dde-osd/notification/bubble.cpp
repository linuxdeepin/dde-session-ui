/*
 * Copyright (C) 2014 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *             listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *             fanpengcheng <fanpengcheng_cm@deepin.com>
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

#include "bubble.h"
#include "notificationentity.h"
#include "appicon.h"
#include "appbody.h"
#include "actionbutton.h"
#include "button.h"
#include "icondata.h"

#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QApplication>
#include <QProcess>
#include <QDBusArgument>
#include <QMoveEvent>
#include <QGSettings>
#include <QSpacerItem>
#include <QGridLayout>
#include <QX11Info>
#include <QDateTime>

#include <DLabel>

#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

static const QStringList HintsOrder {
    "desktop-entry",
    "image-data",
    "image-path",
    "image_path",
    "icon_data"
};

void register_wm_state(WId winid)
{
    xcb_ewmh_connection_t m_ewmh_connection;
    xcb_intern_atom_cookie_t *cookie = xcb_ewmh_init_atoms(QX11Info::connection(), &m_ewmh_connection);
    xcb_ewmh_init_atoms_replies(&m_ewmh_connection, cookie, nullptr);

    xcb_atom_t atoms[2];
    atoms[0] = m_ewmh_connection._NET_WM_WINDOW_TYPE_DOCK;
    atoms[1] = m_ewmh_connection._NET_WM_STATE_BELOW;
    xcb_ewmh_set_wm_window_type(&m_ewmh_connection, winid, 1, atoms);
}

Bubble::Bubble(std::shared_ptr<NotificationEntity> entity, OSD::ShowStyle style)
    : DBlurEffectWidget(nullptr)
    , m_entity(entity)
    , m_titleWidget(new BubbleWidget_Bg(this))
    , m_bodyWidget(new BubbleWidget_Bg(this))
    , m_appNameLabel(new DLabel(this))
    , m_appTimeLabel(new DLabel(this))
    , m_icon(new AppIcon(this))
    , m_body(new AppBody(this))
    , m_actionButton(new ActionButton(this, style))
    , m_closeButton(new Button(this))
    , m_quitTimer(new QTimer(this))
    , m_showStyle(style)
{
    initUI();
    initConnections();
    initAnimations();
    initTimers();

    setEntity(entity);

    QTimer::singleShot(0, this, [ = ] {
        // FIXME: 锁屏不允许显示任何通知，而通知又需要禁止窗管进行管理，
        // 为了避免二者的冲突，将气泡修改为dock，保持在其他程序置顶，又不会显示在锁屏之上。
        register_wm_state(winId());
    });
}

std::shared_ptr<NotificationEntity> Bubble::entity() const
{
    return m_entity;
}

void Bubble::setEntity(std::shared_ptr<NotificationEntity> entity)
{
    if (!entity) return;

    m_canClose = entity->actions().isEmpty() ? true : false;

    m_entity = entity;

    m_outTimer->stop();

    updateContent();

    show();

    if (m_offScreen) {
        m_offScreen = false;
        m_inAnimation->start();
    }

    int timeout = entity->timeout().toInt();
    //  0: never times out
    // -1: default 5s
    m_outTimer->setInterval(timeout == -1 ? BubbleTimeout : (timeout == 0 ? -1 : timeout));
    m_outTimer->start();
}

void Bubble::setPostion(const QPoint &point)
{
    dPos = point;
    move(dPos);
}

void Bubble::setBasePosition(int x, int y, QRect rect)
{
    x -= Padding;
    y += Padding;

    dPos.setX((x - BubbleWidth) / 2);
    dPos.setY(y);
//    const QSize dSize(BubbleWidth, BubbleHeight);

    if (m_inAnimation->state() == QPropertyAnimation::Running) {
        const int baseX = x - BubbleWidth;

        m_inAnimation->setStartValue(QPoint(baseX / 2, y - BubbleHeight));
        m_inAnimation->setEndValue(QPoint(baseX / 2, y));
    }

    if (m_outAnimation->state() != QPropertyAnimation::Running) {
        m_outAnimation->setStartValue(1);
        m_outAnimation->setEndValue(0);
    }

    if (m_dismissAnimation->state() != QPropertyAnimation::Running) {
        m_dismissAnimation->setStartValue(1);
        m_dismissAnimation->setEndValue(0);
    }

    if (!rect.isEmpty())
        m_screenGeometry = rect;
}

void Bubble::compositeChanged()
{
    if (!m_wmHelper->hasComposite()) {
        m_handle->setWindowRadius(0);
        m_handle->setShadowColor(QColor("#E5E5E5"));
    } else {
        m_handle->setWindowRadius(5);
        m_handle->setShadowColor(QColor(0, 0, 0, 100));
    }
}

void Bubble::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!m_defaultAction.isEmpty()) {
            Q_EMIT actionInvoked(this, m_defaultAction);
            m_defaultAction.clear();
        } else {
            //采用动画的方式退出并隐藏，不会丢失窗体‘焦点’，造成控件不响应鼠标进入和离开事件
            m_dismissAnimation->start();
        }
    }

    m_offScreen = true;
    m_outTimer->stop();
}

void Bubble::moveEvent(QMoveEvent *event)
{
    // don't show this bubble on unrelated screens while sliding in.
    if (m_inAnimation->state() == QPropertyAnimation::Running) {
        const bool visible = m_screenGeometry.contains(event->pos());
        //setVisible(visible);
    }

    DBlurEffectWidget::moveEvent(event);
}

void Bubble::showEvent(QShowEvent *event)
{
    DBlurEffectWidget::showEvent(event);

    m_quitTimer->start();
}

void Bubble::hideEvent(QHideEvent *event)
{
    DBlurEffectWidget::hideEvent(event);

    m_outAnimation->stop();
    m_dismissAnimation->stop();

    m_quitTimer->start();
}

void Bubble::enterEvent(QEvent *event)
{
    Q_EMIT havorStateChanged(true);

    return DBlurEffectWidget::enterEvent(event);
}

void Bubble::leaveEvent(QEvent *event)
{
    Q_EMIT havorStateChanged(false);

    return DBlurEffectWidget::leaveEvent(event);
}

void Bubble::onActionButtonClicked(const QString &actionId)
{
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

    m_dismissAnimation->start();
    m_outTimer->stop();
    Q_EMIT actionInvoked(this, actionId);
}

void Bubble::onOutTimerTimeout()
{
    if (containsMouse()) {
        m_outTimer->stop();
        m_outTimer->start();
    } else {
        m_offScreen = true;
        m_outAnimation->start();
    }
}

void Bubble::onOutAnimFinished()
{
    // FIXME: There should be no empty pointers here
    if (m_entity) {
        Q_EMIT expired(this);
    }
}

void Bubble::onDismissAnimFinished()
{
    // FIXME: There should be no empty pointers here
    if (m_entity) {
        Q_EMIT dismissed(this);
    }
}

void Bubble::updateContent()
{
    m_body->setTitle(m_entity->summary());
    m_body->setText(m_entity->body());

    if (m_showStyle == OSD::ShowStyle::BUBBLEWIDGET) {
        setAppName(m_entity->appName());
        setAppTime(m_entity->ctime());
    }

    processIconData();
    processActions();
}

void Bubble::initUI()
{
    m_quitTimer->setInterval(60 * 1000);
    m_quitTimer->setSingleShot(true);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    DStyleHelper dstyle(style());
    int radius = dstyle.pixelMetric(DStyle::PM_TopLevelWindowRadius);
    if (m_showStyle == OSD::ShowStyle::BUBBLEWIDGET) {
        radius = 8;
    }
    setBlurRectXRadius(radius);
    setBlurRectYRadius(radius);

    m_wmHelper = DWindowManagerHelper::instance();

    m_handle = new DPlatformWindowHandle(this);
    m_handle->setTranslucentBackground(true);
    m_handle->setShadowRadius(radius);
    m_handle->setShadowOffset(QPoint(0, 4));

    compositeChanged();

    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setMaskColor(DBlurEffectWidget::LightColor);
    setMouseTracking(true);

    setFixedSize(OSD::BubbleSize(m_showStyle));
    m_icon->setFixedSize(OSD::IconSize(m_showStyle));
    m_closeButton->setFixedSize(OSD::CloseButtonSize(m_showStyle));

    //controls
    m_appNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_appTimeLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    //layout
    if (m_showStyle == OSD::ShowStyle::BUBBLEWINDOW) {
        m_appNameLabel->setVisible(false);
        m_appTimeLabel->setVisible(false);

        m_body->setObjectName("Body");

        QHBoxLayout *layout = new QHBoxLayout;
        layout->setSpacing(10);
        layout->setMargin(PADDING);
        layout->addWidget(m_icon);
        layout->addWidget(m_body);
        layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));

        m_actionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(m_actionButton);

        m_closeButton->setFixedSize(CLOSEBTNSIZE);
        m_closeButton->setRadius(99);
        m_closeButton->setText("X");
        m_closeButton->setVisible(false);
        layout->addWidget(m_closeButton);

        setLayout(layout);
    } else if (m_showStyle == OSD::ShowStyle::BUBBLEWIDGET) {

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
        titleLayout->addWidget(m_closeButton);

        m_titleWidget->setLayout(titleLayout);
        m_titleWidget->setHoverAlpha(0);
        m_titleWidget->setUnHoverAlpha(0);

        mainLayout->addWidget(m_titleWidget);

        QHBoxLayout *bodyLayout = new QHBoxLayout;
        bodyLayout->setSpacing(0);
        bodyLayout->setContentsMargins(10, 0, 10, 0);
        bodyLayout->addWidget(m_body);
        bodyLayout->addWidget(m_actionButton);

        m_bodyWidget->setLayout(bodyLayout);

        mainLayout->addWidget(m_bodyWidget);

        m_actionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QHBoxLayout *l = new QHBoxLayout;
        l->setSpacing(0);
        l->setMargin(0);

        setLayout(mainLayout);
    }
}

void Bubble::initConnections()
{
    connect(m_actionButton, &ActionButton::buttonClicked, this, &Bubble::onActionButtonClicked);

    connect(m_closeButton, &Button::clicked, this, [ = ] {
        m_dismissAnimation->start();
    });

    connect(this, &Bubble::expired, m_actionButton, [ = ]() {
        m_actionButton->expired(int(m_entity->id()));
    });
    connect(this, &Bubble::dismissed, m_actionButton,  [ = ]() {
        m_actionButton->dismissed(int(m_entity->id()));
    });
    connect(this, &Bubble::replacedByOther, m_actionButton, [ = ]() {
        m_actionButton->replacedByOther(int(m_entity->id()));
    });
    connect(this, &Bubble::havorStateChanged, this, &Bubble::onHavorStateChanged);

    connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged, this, &Bubble::compositeChanged);
    connect(m_quitTimer, &QTimer::timeout, this, &Bubble::onDelayQuit);
}

void Bubble::initAnimations()
{
    m_inAnimation = new QPropertyAnimation(this, "pos", this);
    m_inAnimation->setDuration(300);
    m_inAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_outAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_outAnimation->setDuration(500);
    m_outAnimation->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_outAnimation, &QPropertyAnimation::finished, this, &Bubble::onOutAnimFinished);

    m_dismissAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_dismissAnimation->setDuration(300);
    m_dismissAnimation->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_dismissAnimation, &QPropertyAnimation::finished, this, &Bubble::onDismissAnimFinished);

    m_moveAnimation = new QPropertyAnimation(this, "pos", this);
    m_moveAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

void Bubble::initTimers()
{
    m_outTimer = new QTimer(this);
    m_outTimer->setInterval(BubbleTimeout);
    m_outTimer->setSingleShot(true);
    connect(m_outTimer, &QTimer::timeout, this, &Bubble::onOutTimerTimeout);
}

bool Bubble::containsMouse() const
{
    return geometry().contains(QCursor::pos());
}

void Bubble::setAppName(const QString &name)
{
    if (m_appNameLabel)
        m_appNameLabel->setText(name);
}

void Bubble::setAppTime(const QString &time)
{
    if (m_appTimeLabel)
        m_appTimeLabel->setText(CreateTimeString(time));
}

// Each even element in the list (starting at index 0) represents the identifier for the action.
// Each odd element in the list is the localized string that will be displayed to the user.
void Bubble::processActions()
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

void Bubble::processIconData()
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

void Bubble::saveImg(const QImage &image)
{
    QDir dir;
    dir.mkdir(CachePath);

    image.save(CachePath + QString::number(m_entity->id()) + ".png");
}

inline void copyLineRGB32(QRgb *dst, const char *src, int width)
{
    const char *end = src + width * 3;
    for (; src != end; ++dst, src += 3) {
        *dst = qRgb(src[0], src[1], src[2]);
    }
}

inline void copyLineARGB32(QRgb *dst, const char *src, int width)
{
    const char *end = src + width * 4;
    for (; src != end; ++dst, src += 4) {
        *dst = qRgba(src[0], src[1], src[2], src[3]);
    }
}

static QImage decodeNotificationSpecImageHint(const QDBusArgument &arg)
{
    int width, height, rowStride, hasAlpha, bitsPerSample, channels;
    QByteArray pixels;
    char *ptr;
    char *end;

    arg.beginStructure();
    arg >> width >> height >> rowStride >> hasAlpha >> bitsPerSample >> channels >> pixels;
    arg.endStructure();
    //qDebug() << width << height << rowStride << hasAlpha << bitsPerSample << channels;

#define SANITY_CHECK(condition) \
    if (!(condition)) { \
        qWarning() << "Sanity check failed on" << #condition; \
        return QImage(); \
    }

    SANITY_CHECK(width > 0);
    SANITY_CHECK(width < 2048);
    SANITY_CHECK(height > 0);
    SANITY_CHECK(height < 2048);
    SANITY_CHECK(rowStride > 0);

#undef SANITY_CHECK

    QImage::Format format = QImage::Format_Invalid;
    void (*fcn)(QRgb *, const char *, int) = nullptr;
    if (bitsPerSample == 8) {
        if (channels == 4) {
            format = QImage::Format_ARGB32;
            fcn = copyLineARGB32;
        } else if (channels == 3) {
            format = QImage::Format_RGB32;
            fcn = copyLineRGB32;
        }
    }
    if (format == QImage::Format_Invalid) {
        qWarning() << "Unsupported image format (hasAlpha:" << hasAlpha << "bitsPerSample:" << bitsPerSample << "channels:" << channels << ")";
        return QImage();
    }

    QImage image(width, height, format);
    ptr = pixels.data();
    end = ptr + pixels.length();
    for (int y = 0; y < height; ++y, ptr += rowStride) {
        if (ptr + channels * width > end) {
            qWarning() << "Image data is incomplete. y:" << y << "height:" << height;
            break;
        }
        fcn((QRgb *)image.scanLine(y), ptr, width);
    }

    return image;
}

const QPixmap Bubble::converToPixmap(const QDBusArgument &value)
{
    // use plasma notify source code to conver photo, solving encoded question.
    const QImage &img = decodeNotificationSpecImageHint(value);
    saveImg(img);
    return QPixmap::fromImage(img).scaled(m_icon->width(), m_icon->height(),
                                          Qt::KeepAspectRatioByExpanding,
                                          Qt::SmoothTransformation);
}

QString Bubble::CreateTimeString(const QString &time)
{
    qint64 msec = QDateTime::currentMSecsSinceEpoch() - time.toLongLong();
    if (msec < 0) {
        return QString("time error");
    }

    QString text;

    QDateTime bubbleDateTime = QDateTime::fromMSecsSinceEpoch(time.toLongLong());
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int elapsedDay = int(bubbleDateTime.daysTo(currentDateTime));
    int minute = int(msec / 1000 / 60);

//    qDebug() << elapsedDay << minute;

    if (elapsedDay == 0) {
        if (minute == 0) {
            text =  tr("Just Now");
        } else if (minute > 0 && minute < 60) {
            text = QString::number(minute) + tr(" Minute Ago");
        } else {
            text = QString::number(minute / 60) + tr(" Hour Ago");
        }
    } else if (elapsedDay == 1) {
        text = tr("Yesterday ") + currentDateTime.toString("hh:mm");
    } else {
        text = QString::number(elapsedDay) + tr(" Day Ago");
    }
    return text;
}

void Bubble::onDelayQuit()
{
    const QGSettings gsettings("com.deepin.dde.notification", "/com/deepin/dde/notification/");
    if (gsettings.keys().contains("autoExit") && gsettings.get("auto-exit").toBool()) {
        qWarning() << "Killer Timeout, now quiiting...";
        qApp->quit();
    }
}

void Bubble::resetMoveAnim(const QPoint &point)
{
    if (isVisible() && m_outAnimation->state() != QPropertyAnimation::Running) {
        dPos = QPoint(x(), point.y());
        m_moveAnimation->setStartValue(QPoint(x(), y()));
        m_moveAnimation->setEndValue(dPos);

        const QRect &startRect = QRect(dPos, QSize(BubbleWidth, BubbleHeight));
        m_outAnimation->setStartValue(startRect);
        m_outAnimation->setEndValue(QRect(startRect.right(), startRect.y(), 0, BubbleHeight));

        m_moveAnimation->start();
    }
}

void Bubble::onHavorStateChanged(bool hover)
{
    m_havor = hover;

    if (m_canClose || (m_showStyle == OSD::ShowStyle::BUBBLEWIDGET)) {
        m_closeButton->setVisible(hover);
    }

    if (m_showStyle == OSD::ShowStyle::BUBBLEWIDGET) {
        m_appTimeLabel->setVisible(!hover);
    }
}

BubbleTemplate::BubbleTemplate()
    : DBlurEffectWidget(nullptr)
{
    initUI();
}

void BubbleTemplate::setLevel(ShowLevel level)
{
    m_level = level;

    resize(m_sizeMap.value(level));
}

void BubbleTemplate::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    DStyleHelper dstyle(style());
    int radius = dstyle.pixelMetric(DStyle::PM_TopLevelWindowRadius);
    setBlurRectXRadius(radius);
    setBlurRectYRadius(radius);

    initSizeMap(m_sizeMap);

    setLevel(NORMAL);
}

void BubbleTemplate::initSizeMap(SizeMap &map)
{
    map.clear();

    map.insert(ShowLevel::NORMAL, QSize(BubbleWidth, BubbleHeight));
    map.insert(ShowLevel::LEVEL0, QSize((BubbleWidth / 6) * 5, (BubbleHeight / 8) * 7));
    map.insert(ShowLevel::LEVEL1, QSize((BubbleWidth / 6) * 4, (BubbleHeight / 8) * 6));
}

BubbleWidget_Bg::BubbleWidget_Bg(QWidget *parent)
    : DWidget(parent)
{

}

void BubbleWidget_Bg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QColor brushColor(Qt::white);
    brushColor.setAlpha(m_hover ? m_hoverAlpha : m_unHoverAlpha);
    painter.setBrush(brushColor);

    QPen borderPen;
    borderPen.setColor(Qt::transparent);
    painter.setPen(borderPen);
    painter.drawRect(QRectF(0, 0, width(), height())/*, m_radius, m_radius*/);

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
