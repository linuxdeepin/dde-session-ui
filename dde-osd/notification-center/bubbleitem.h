/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zorowk <near.kingzero@gmail.com>
 *
 * Maintainer: zorowk <near.kingzero@gmail.com>
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

#ifndef BUBBLEITEM_H
#define BUBBLEITEM_H

#include <DWidget>
#include <DLabel>
#include <DDialogCloseButton>
#include <memory>
#include <QDBusArgument>

class NotificationEntity;
class NotifyModel;
class AppIcon;
class AppBody;
class Button;
class ActionButton;

DWIDGET_USE_NAMESPACE

class AlphaWidget : public DWidget
{
public:
    AlphaWidget(QWidget *parent = nullptr);

    void setAlpha(int alpha) {m_hoverAlpha = alpha; m_unHoverAlpha = alpha; update();}

    int hoverAlpha() {return m_hoverAlpha;}
    void setHoverAlpha(int alpha) {m_hoverAlpha = alpha; update();}
    void setSelectedStatus(bool status) {m_selected = status;}
    void setSelectedAlpha(int alpha) {m_selectedAlpha = alpha; update();}

    int unHoverAlpha() {return m_unHoverAlpha;}
    void setUnHoverAlpha(int alpha) {m_unHoverAlpha = alpha; update();}

private:
    int m_selectedAlpha = 0;
    int m_hoverAlpha = 0;
    int m_unHoverAlpha = 0;
    bool m_hover = false;
    bool m_selected = false;

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
};

class BubbleItem : public QWidget
{
    Q_OBJECT
public:
    BubbleItem(QWidget *parent = nullptr, std::shared_ptr<NotificationEntity> entity = nullptr);
    void setModel(NotifyModel *model);
    const QPixmap converToPixmap(const QDBusArgument &value);
    void setShowContent(bool visible);
    void setAlpha(int alpha);

Q_SIGNALS:
    void havorStateChanged(bool);

public Q_SLOTS:
    void onHavorStateChanged(bool hover);
    void onCloseBubble();

protected:
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initContent();
    void onRefreshTime();
    void refreshTheme();

private:
    std::shared_ptr<NotificationEntity> m_entity;
    NotifyModel *m_notifyModel = nullptr;
    QTimer *m_refreshTimer = nullptr;

    //controls
    AlphaWidget *m_bgWidget = nullptr;
    AlphaWidget *m_titleWidget = nullptr;
    AlphaWidget *m_bodyWidget = nullptr;
    DLabel *m_appNameLabel = nullptr;
    DLabel *m_appTimeLabel = nullptr;

    AppIcon *m_icon = nullptr;
    AppBody *m_body = nullptr;
    ActionButton *m_actionButton = nullptr;
    DDialogCloseButton *m_closeButton = nullptr;

    bool m_showContent = true;
    QString m_defaultAction;
};

#endif // BUBBLEITEM_H
