// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NOTIFICATIONSWIDGET_H
#define NOTIFICATIONSWIDGET_H

#include <constants.h>

#include <QWidget>
#include <QIcon>

class NotificationsWidget: public QWidget
{
    Q_OBJECT

public:
    explicit NotificationsWidget(QWidget *parent = nullptr);
    void setDisturb(bool OK);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    const QPixmap loadSvg(const QString &iconName, const QString &localPath, const int size, const qreal ratio);
    bool containCursorPos();

private:
    bool m_disturb; //提供勿扰图标修改
    bool m_hover;
    bool m_pressed;
};

#endif // NOTIFICATIONSWIDGET_H
