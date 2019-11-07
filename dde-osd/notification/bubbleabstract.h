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
class BubbleWidget_Bg : public DWidget
{
public:
    BubbleWidget_Bg(QWidget *parent = nullptr);

    void setAlpha(int alpha) {m_hoverAlpha = alpha; m_unHoverAlpha = alpha; update();}

    int hoverAlpha() {return m_hoverAlpha;}
    void setHoverAlpha(int alpha) {m_hoverAlpha = alpha; update();}

    int unHoverAlpha() {return m_unHoverAlpha;}
    void setUnHoverAlpha(int alpha) {m_unHoverAlpha = alpha; update();}

private:
    int m_hoverAlpha = 0;
    int m_unHoverAlpha = 0;
    bool m_hover = false;

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
};

class BubbleAbStract : public DBlurEffectWidget
{
public:
    BubbleAbStract(QWidget *parent = nullptr, std::shared_ptr<NotificationEntity> entity = nullptr);
    void saveImg(const QImage &image);
    const QPixmap converToPixmap(const QDBusArgument &value);

public Q_SLOTS:
    void compositeChanged();

protected:
    void updateContent();
    void processActions();
    void processIconData();
    bool containsMouse() const;

protected:
    std::shared_ptr<NotificationEntity> m_entity;

    //controls
    BubbleWidget_Bg *m_bgWidget = nullptr;
    BubbleWidget_Bg *m_titleWidget = nullptr;
    BubbleWidget_Bg *m_bodyWidget = nullptr;
    DLabel *m_appNameLabel = nullptr;
    DLabel *m_appTimeLabel = nullptr;
    AppIcon *m_icon = nullptr;
    AppBody *m_body = nullptr;
    ActionButton *m_actionButton = nullptr;
    Button *m_closeButton = nullptr;

    DPlatformWindowHandle *m_handle = nullptr;
    DWindowManagerHelper *m_wmHelper = nullptr;

    QString m_defaultAction;
    bool m_canClose = false;

protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
};

#endif // BUBBLEABSTRACT_H
