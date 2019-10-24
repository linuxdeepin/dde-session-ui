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

#include <QFrame>
#include <QStandardPaths>
#include <QDir>
#include <DPlatformWindowHandle>
#include <DWindowManagerHelper>
#include <QDBusArgument>
#include <memory>

DWIDGET_USE_NAMESPACE

class QLabel;
class AppIcon;
class QPropertyAnimation;
class QParallelAnimationGroup;
class NotificationEntity;
class ActionButton;
class AppBody;
class QGraphicsDropShadowEffect;

static const int Padding = 46;
static const int BubbleMargin = 20;
static const int BubbleWidth = 320;
static const int BubbleHeight = 80;
static const int BubbleTimeout = 5000;//msec

static const QStringList Directory = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
static const QString CachePath = Directory.first() + "/.cache/deepin/deepin-notifications/";

class Bubble : public DBlurEffectWidget
{
    Q_OBJECT
public:
    Bubble(std::shared_ptr<NotificationEntity> entity = nullptr);

    std::shared_ptr<NotificationEntity> entity() const;
    void setBasePosition(int, int, QRect = QRect());
    void setEntity(std::shared_ptr<NotificationEntity> entity);

    QPoint postion() { return dPos; }
    void setPostion(const QPoint &point)
    {
        dPos = point;
        move(dPos);
    }

Q_SIGNALS:
    void expired(Bubble *);
    void dismissed(Bubble *);
    void replacedByOther(Bubble *);
    void actionInvoked(Bubble *, QString);
    void focusChanged(bool);

public Q_SLOTS:
    void compositeChanged();
    void onDelayQuit();
    void resetMoveAnim(const QPoint &rect);

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

    void saveImg(const QImage &image);
    const QPixmap converToPixmap(const QDBusArgument &value);

private:
    std::shared_ptr<NotificationEntity> m_entity;

    AppIcon *m_icon = nullptr;
    AppBody *m_body = nullptr;
    ActionButton *m_actionButton = nullptr;

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
};

#endif // BUBBLE_H
