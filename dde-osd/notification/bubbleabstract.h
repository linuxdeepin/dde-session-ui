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

#ifndef BUBBLEABSTRACT_H
#define BUBBLEABSTRACT_H

#include <DBlurEffectWidget>
#include <DWidget>
#include <memory>
#include <DPlatformWindowHandle>
#include <DWindowManagerHelper>
#include <QDBusArgument>

DWIDGET_USE_NAMESPACE

class NotificationEntity;
class AppIcon;
class AppBody;
class Button;
class ActionButton;

static const QStringList HintsOrder {
    "desktop-entry",
    "image-data",
    "image-path",
    "image_path",
    "icon_data"
};

class BubbleAbStract : public DBlurEffectWidget
{
    Q_OBJECT
public:
    BubbleAbStract(QWidget *parent = nullptr, std::shared_ptr<NotificationEntity> entity = nullptr);
    void saveImg(const QImage &image);
    const QPixmap converToPixmap(const QDBusArgument &value);
    static inline void copyLineRGB32(QRgb *dst, const char *src, int width);
    static inline void copyLineARGB32(QRgb *dst, const char *src, int width);
    static QImage decodeNotificationSpecImageHint(const QDBusArgument &arg);

public Q_SLOTS:
    void compositeChanged();
    void onActionButtonClicked(const QString &actionId);

protected:
    void updateContent();
    void processActions();
    void processIconData();
    bool containsMouse() const;

protected:
    std::shared_ptr<NotificationEntity> m_entity;

    //controls
    AppIcon *m_icon = nullptr;
    AppBody *m_body = nullptr;
    ActionButton *m_actionButton = nullptr;
    Button *m_closeButton = nullptr;

    DPlatformWindowHandle *m_handle = nullptr;
    DWindowManagerHelper *m_wmHelper = nullptr;

    QString m_defaultAction;
    bool m_canClose = false;
    bool m_enabled = true;

protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
};

#endif // BUBBLEABSTRACT_H
