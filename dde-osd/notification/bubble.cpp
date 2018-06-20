/*
 * Copyright (C) 2014 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *             listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
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

#include "notificationentity.h"
#include "appicon.h"
#include "appbody.h"
#include "actionbutton.h"
#include "icondata.h"

DWIDGET_USE_NAMESPACE

static const QString BubbleStyleSheet = "QFrame#Background { "
                                        "background-color: rgba(0, 0, 0, 60);"
                                        "border-radius: 8px;"
                                        "}"
                                        "QLabel#Title {"
                                        "font-size: 11px;"
                                        "color: black;"
                                        "}";
static const int Padding = 20;
static const int BubbleWidth = 300;
static const int BubbleHeight = 70;

Bubble::Bubble(NotificationEntity *entity)
    : DBlurEffectWidget(nullptr)
    , m_entity(entity)
    , m_icon(new AppIcon(this))
    , m_body(new AppBody(this))
    , m_actionButton(new ActionButton(this))
    , m_quitTimer(new QTimer(this))

{
    m_quitTimer->setInterval(60 * 1000);
    m_quitTimer->setSingleShot(true);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    m_wmHelper = DWindowManagerHelper::instance();

    m_handle = new DPlatformWindowHandle(this);
    m_handle->setTranslucentBackground(true);
    m_handle->setShadowRadius(14);
    m_handle->setShadowOffset(QPoint(0, 4));

    compositeChanged();

    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setMaskColor(DBlurEffectWidget::LightColor);

    initUI();
    initAnimations();
    initTimers();

    setEntity(entity);

    connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged, this, &Bubble::compositeChanged);
    connect(m_quitTimer, &QTimer::timeout, this, &Bubble::onDelayQuit);
}

NotificationEntity *Bubble::entity() const
{
    return m_entity;
}

void Bubble::setEntity(NotificationEntity *entity)
{
    if (!entity) return;

    m_entity = entity;

    m_outTimer->stop();

    updateContent();

    show();

    m_outTimer->start();
}


void Bubble::setBasePosition(int x, int y, QRect rect)
{
    x -= Padding;
    y += Padding;

    const QPoint dPos(x - BubbleWidth, y);
    const QSize dSize(BubbleWidth, BubbleHeight);

    move(dPos);
    resize(dSize);

    if (m_outAnimation->state() != QPropertyAnimation::Running) {
        const QRect normalGeo( dPos, dSize );
        QRect outGeo( normalGeo.right(), normalGeo.y(), 0, normalGeo.height());

        m_outAnimation->setStartValue(normalGeo);
        m_outAnimation->setEndValue(outGeo);
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

void Bubble::mousePressEvent(QMouseEvent *)
{
    if (!m_defaultAction.isEmpty()) {
        Q_EMIT actionInvoked(m_entity->id().toUInt(), m_defaultAction);
        m_defaultAction.clear();
    } else {
        Q_EMIT dismissed(m_entity->id().toInt());
    }

    m_outTimer->stop();
}

void Bubble::showEvent(QShowEvent *event)
{
    DBlurEffectWidget::showEvent(event);

    QTimer::singleShot(1, this, [=] {
        raise();
    });

    m_quitTimer->start();
}

void Bubble::hideEvent(QHideEvent *event)
{
    DBlurEffectWidget::hideEvent(event);

    m_quitTimer->start();
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

    m_outTimer->stop();
    Q_EMIT actionInvoked(m_entity->id().toUInt(), actionId);
}

void Bubble::onOutTimerTimeout()
{
    if (containsMouse()) {
        m_outTimer->stop();
        m_outTimer->start();
    } else {
        m_outAnimation->start();
    }
}

void Bubble::onOutAnimFinished()
{
    Q_EMIT expired(m_entity->id().toInt());
}

void Bubble::updateContent()
{
    m_body->setTitle(m_entity->summary());
    m_body->setText(m_entity->body());

    processIconData();
    processActions();
}

void Bubble::initUI()
{
    resize(BubbleWidth, BubbleHeight);

    m_icon->setFixedSize(48, 48);
    m_icon->move(11, 11);

    m_body->setObjectName("Body");
    m_body->move(70, 0);

    m_actionButton->move(BubbleWidth - m_actionButton->width(), 0);

    setStyleSheet(BubbleStyleSheet);

    connect(m_actionButton, &ActionButton::buttonClicked, this, &Bubble::onActionButtonClicked);
}

void Bubble::initAnimations()
{
    m_outAnimation = new QPropertyAnimation(this, "geometry", this);
    m_outAnimation->setDuration(300);
    m_outAnimation->setEasingCurve(QEasingCurve::OutCubic);

    connect(m_outAnimation, &QPropertyAnimation::finished, this, &Bubble::onOutAnimFinished);

    m_moveAnimation = new QPropertyAnimation(this, "pos", this);
    m_moveAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

void Bubble::initTimers()
{
    m_outTimer = new QTimer(this);
    m_outTimer->setInterval(5000);
    m_outTimer->setSingleShot(true);
    connect(m_outTimer, &QTimer::timeout, this, &Bubble::onOutTimerTimeout);
}

bool Bubble::containsMouse() const
{
    QRect rectToGlobal = QRect(mapToGlobal(rect().topLeft()),
                               mapToGlobal(rect().bottomRight()));
    return rectToGlobal.contains(QCursor::pos());
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

    if (m_actionButton->isEmpty()) {
        m_actionButton->hide();
        m_body->setFixedSize(220, BubbleHeight);
    } else {
        m_actionButton->show();
        m_body->setFixedSize(150, BubbleHeight);
    }

}

void Bubble::processIconData()
{
    const QString imagePath = m_entity->hints().contains("image-path") ? m_entity->hints()["image-path"].toString() : "";

    if (imagePath.isEmpty()) {
        if (m_entity->hints()["image-data"].canConvert<QDBusArgument>()){
            QDBusArgument argument = m_entity->hints()["image-data"].value<QDBusArgument>();
            m_icon->setPixmap(converToPixmap(argument));
        } else if (m_entity->hints()["icon_data"].canConvert<QDBusArgument>()) {
            QDBusArgument argument = m_entity->hints()["icon_data"].value<QDBusArgument>();
            m_icon->setPixmap(converToPixmap(argument));
        } else {
            m_icon->setIcon(m_entity->appIcon());
        }
    } else {
        m_icon->setIcon(imagePath);
    }
}

void Bubble::saveImg(const QImage &image)
{
    QDir dir;
    dir.mkdir(CachePath);

    image.save(CachePath + m_entity->id() + ".png");
}

inline void copyLineRGB32(QRgb* dst, const char* src, int width)
{
    const char* end = src + width * 3;
    for (; src != end; ++dst, src+=3) {
        *dst = qRgb(src[0], src[1], src[2]);
    }
}

inline void copyLineARGB32(QRgb* dst, const char* src, int width)
{
    const char* end = src + width * 4;
    for (; src != end; ++dst, src+=4) {
        *dst = qRgba(src[0], src[1], src[2], src[3]);
    }
}

static QImage decodeNotificationSpecImageHint(const QDBusArgument& arg)
{
    int width, height, rowStride, hasAlpha, bitsPerSample, channels;
    QByteArray pixels;
    char* ptr;
    char* end;

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
    void (*fcn)(QRgb*, const char*, int) = 0;
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
    for (int y=0; y<height; ++y, ptr += rowStride) {
        if (ptr + channels * width > end) {
            qWarning() << "Image data is incomplete. y:" << y << "height:" << height;
            break;
        }
        fcn((QRgb*)image.scanLine(y), ptr, width);
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

void Bubble::onDelayQuit()
{
    const QGSettings gsettings("com.deepin.dde.notification", "/com/deepin/dde/notification/");
    if (gsettings.keys().contains("autoExit") && gsettings.get("auto-exit").toBool()) {
        qWarning() << "Killer Timeout, now quiiting...";
        qApp->quit();
    }
}

void Bubble::resetMoveAnim(const QRect &rect)
{
    if (isVisible() && m_outAnimation->state() != QPropertyAnimation::Running) {
        const QPoint &endPoint = QPoint(rect.x() - Padding - BubbleWidth, y());
        m_moveAnimation->setStartValue(QPoint(x(), y()));
        m_moveAnimation->setEndValue(endPoint);

        const QRect &startRect = QRect(endPoint, QSize(BubbleWidth, BubbleHeight));
        m_outAnimation->setStartValue(startRect);
        m_outAnimation->setEndValue(QRect(startRect.right(), startRect.y(), 0, BubbleHeight));

        m_moveAnimation->start();
    }
}
