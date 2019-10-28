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

#ifndef BUBBLE_H
#define BUBBLE_H

#include <DBlurEffectWidget>
#include <DIconButton>
#include <DLabel>

#include <QFrame>
#include <QStandardPaths>
#include <QDir>
#include <DPlatformWindowHandle>
#include <DWindowManagerHelper>
#include <QDBusArgument>

#include <memory>

#include "define.h"

DWIDGET_USE_NAMESPACE

static const int Padding = 46;
static const int BubbleMargin = 20;
static const int BubbleWidth = 320;
static const int BubbleHeight = 80;
static const int BubbleTimeout = 5000;//msec
static const QStringList Directory = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
static const QString CachePath = Directory.first() + "/.cache/deepin/deepin-notifications/";

typedef enum {
    NORMAL,
    LEVEL0,
    LEVEL1
} ShowLevel;

typedef QMap<ShowLevel, QSize> SizeMap;

class QLabel;
class AppIcon;
class QPropertyAnimation;
class QParallelAnimationGroup;
class NotificationEntity;
class ActionButton;
class Button;
class AppBody;
class QGraphicsDropShadowEffect;
class BubbleWidget_Bg;
class Bubble : public DBlurEffectWidget
{
    Q_OBJECT
public:
    Bubble(std::shared_ptr<NotificationEntity> entity = nullptr, OSD::ShowStyle style = OSD::ShowStyle::BUBBLEWINDOW);

    std::shared_ptr<NotificationEntity> entity() const;
    void setBasePosition(int, int, QRect = QRect());
    void setEntity(std::shared_ptr<NotificationEntity> entity);

    QPoint postion() { return dPos; }
    void setPostion(const QPoint &point);

Q_SIGNALS:
    void expired(Bubble *);
    void dismissed(Bubble *);
    void replacedByOther(Bubble *);
    void actionInvoked(Bubble *, QString);
    void havorStateChanged(bool);

public Q_SLOTS:
    void compositeChanged();
    void onDelayQuit();
    void resetMoveAnim(const QPoint &rect);
    void onHavorStateChanged(bool hover);

protected:
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void moveEvent(QMoveEvent *) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onActionButtonClicked(const QString &actionId);
    void onOutTimerTimeout();
    void onOutAnimFinished();
    void onDismissAnimFinished();

private:
    void initUI();
    void initConnections();
    void initAnimations();
    void initTimers();
    void updateContent();
    void processActions();
    void processIconData();
    bool containsMouse() const;

    const QString &appName() {return m_appName;}
    void setAppName(const QString &name);

    const QString &appTime() {return m_appTime;}
    void setAppTime(const QString &time);

    void saveImg(const QImage &image);
    const QPixmap converToPixmap(const QDBusArgument &value);
    QString CreateTimeString(const QString &time);

private:
    std::shared_ptr<NotificationEntity> m_entity;

    BubbleWidget_Bg *m_titleWidget = nullptr;
    BubbleWidget_Bg *m_bodyWidget = nullptr;
    DLabel *m_appNameLabel = nullptr;
    DLabel *m_appTimeLabel = nullptr;

    AppIcon *m_icon = nullptr;
    AppBody *m_body = nullptr;
    ActionButton *m_actionButton = nullptr;
    Button *m_closeButton = nullptr;

    QPropertyAnimation *m_inAnimation = nullptr;
    QPropertyAnimation *m_outAnimation = nullptr;
    QPropertyAnimation *m_dismissAnimation = nullptr;
    QPropertyAnimation *m_moveAnimation = nullptr;
    QTimer *m_outTimer = nullptr;
    QTimer *m_quitTimer;
    DPlatformWindowHandle *m_handle;
    DWindowManagerHelper *m_wmHelper;

    QRect m_screenGeometry;
    QString m_defaultAction;
    QPoint dPos;

    bool m_offScreen = true;
    bool m_canClose = false;
    bool m_havor = false;

    OSD::ShowStyle m_showStyle;
    QString m_appName;
    QString m_appTime;
};

class BubbleTemplate : public DBlurEffectWidget
{
    Q_OBJECT
public:
    BubbleTemplate();

    ShowLevel level() const {return m_level;}
    void setLevel(ShowLevel level) ;

private:
    ShowLevel m_level = NORMAL;
    SizeMap m_sizeMap;

    void initUI();
    void initSizeMap(SizeMap &map);
};

class BubbleWidget_Bg : public DWidget
{
    friend class Bubble;
private:
    BubbleWidget_Bg(QWidget *parent = nullptr);

    int hoverAlpha() {return m_hoverAlpha;}
    void setHoverAlpha(int alpha) {m_hoverAlpha = alpha; update();}

    int unHoverAlpha() {return m_unHoverAlpha;}
    void setUnHoverAlpha(int alpha) {m_unHoverAlpha = alpha; update();}

private:
    int m_hoverAlpha = 0;
    int m_unHoverAlpha = 0;
    bool m_hover = false;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
};

#endif // BUBBLE_H
