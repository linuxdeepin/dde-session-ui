/**
 * Copyright (C) 2014 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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

Bubble::Bubble(NotificationEntity *entity):
    DBlurEffectWidget(),
    m_entity(entity),
    m_icon(new AppIcon(this)),
    m_title(new QLabel(this)),
    m_body(new AppBody(this)),
    m_actionButton(new ActionButton(this))
{
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
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
    m_aboutToOutTimer->stop();

    updateContent();

    show();

    if (m_offScreen) {
        m_offScreen = false;
        m_inAnimation->start();
    }

    m_aboutToOutTimer->start();
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

    m_inAnimation->setStartValue(QPoint(x - width(), y - height()));
    m_inAnimation->setEndValue(QPoint(x - width(), y));

    const QRect normalGeo( dPos, dSize );
    QRect outGeo( normalGeo.right(), normalGeo.y(), 0, normalGeo.height());

    m_outAnimation->setStartValue(normalGeo);
    m_outAnimation->setEndValue(outGeo);

    if (!rect.isEmpty())
        m_screenGeometry = rect;
}

QPoint Bubble::getCursorPos()
{
    return QCursor::pos();
}

void Bubble::setMask(int, int, int, int)
{

}

void Bubble::closeButtonClicked()
{
    emit dismissed(m_entity->id().toInt());

    m_outTimer->stop();
    m_aboutToOutTimer->stop();
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
    // TODO: default action support
    emit dismissed(m_entity->id().toInt());

    m_outTimer->stop();
    m_aboutToOutTimer->stop();
}

void Bubble::moveEvent(QMoveEvent *event)
{
    // don't show this bubble on unrelated screens while sliding in.
    if (m_inAnimation->state() == QPropertyAnimation::Running) {
        const bool visible = m_screenGeometry.contains(event->pos());
        resize(visible ? QSize(BubbleWidth, BubbleHeight) : QSize(1, 1));
    }

    DBlurEffectWidget::moveEvent(event);
}

void Bubble::updateContent()
{
    QJsonArray actions;
    foreach (QString action, m_entity->actions()) {
        actions.append(action);
    }

    m_title->setText(m_entity->summary());
    m_body->setText(m_entity->body());

    if (m_entity->body().isEmpty()) {
        m_title->move(70, 16);
    } else {
        m_title->move(70, 6);
    }

    processIconData();
    processActions();
}

void Bubble::initUI()
{
    m_icon->setFixedSize(48, 48);
    m_icon->move(11, 11);

    m_title->setTextFormat(Qt::RichText);
    m_title->setObjectName("Title");
    m_title->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_title->move(70, 6);

    m_body->setObjectName("Body");
    m_body->move(70, 22);

    m_actionButton->move(BubbleWidth - m_actionButton->width(), 0);

    setStyleSheet(BubbleStyleSheet);

    connect(m_actionButton, &ActionButton::buttonClicked, [this](QString actionId){
        QMap<QString, QVariant> hints = m_entity->hints();
        QMap<QString, QVariant>::const_iterator i = hints.constBegin();
        while (i != hints.constEnd()) {
            QStringList args = i.value().toString().split(",");
            if (args.length()) {
                QString cmd = args.first();
                args.removeFirst();
                if (i.key() == "x-deepin-action-" + actionId) {
                    QProcess::startDetached(cmd, args);
                }
            }
            ++i;
        }
        emit actionInvoked(m_entity->id().toInt(), actionId);
    });
}

void Bubble::initAnimations()
{
    m_inAnimation = new QPropertyAnimation(this, "pos", this);
    m_inAnimation->setDuration(300);
    m_inAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_outAnimation = new QPropertyAnimation(this, "geometry", this);
    m_outAnimation->setDuration(300);
    m_outAnimation->setEasingCurve(QEasingCurve::OutCubic);

    connect(m_outAnimation, &QPropertyAnimation::finished, [this]{
        emit expired(m_entity->id().toInt());
    });
}

void Bubble::initTimers()
{
    m_outTimer = new QTimer(this);
    m_outTimer->setInterval(5000);
    m_outTimer->setSingleShot(true);
    connect(m_outTimer, &QTimer::timeout, [this]{
        if (containsMouse()) {
            m_outTimer->stop();
            m_outTimer->start();
        } else {
            m_offScreen = true;
            m_outAnimation->start();
        }
    });

    m_aboutToOutTimer = new QTimer(this);
    m_aboutToOutTimer->setInterval(m_outTimer->interval() - 1000);
    m_aboutToOutTimer->setSingleShot(true);
    connect(m_aboutToOutTimer, &QTimer::timeout, this, &Bubble::aboutToQuit);
}

bool Bubble::containsMouse() const
{
    QRect rectToGlobal = QRect(mapToGlobal(rect().topLeft()),
                               mapToGlobal(rect().bottomRight()));
    return rectToGlobal.contains(QCursor::pos());
}

void Bubble::processActions()
{
    m_actionButton->clear();

    QString id;
    QString text;
    QStringList actions = m_entity->actions();
    for (int i = 0; i < actions.length(); i++) {
        if (i % 2 == 0) {
            id = actions.at(i);
            if (id == "default") {
                i++; continue;
            }
        } else {
            text = actions.at(i);
            m_actionButton->addButton(id, text);
        }
    }

    if (m_actionButton->isEmpty()) {
        m_title->setFixedSize(220, 20);
        m_body->setFixedSize(220, 40);
    } else {
        m_title->setFixedSize(150, 20);
        m_body->setFixedSize(150, 40);
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
