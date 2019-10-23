/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     fanpengcheng <fanpengcheng_cm@deepin.com>
 *
 * Maintainer: fanpengcheng <fanpengcheng_cm@deepin.com>
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
#ifndef BUTTON_H
#define BUTTON_H
#include <QWidget>
#include <QIcon>

//#define MAX(a,b) ((a)<(b)?(b):(a))
#define MIN(a,b) ((a)>(b)?(b):(a))

class QMouseEvent;
class QMenu;
class QPaintEvent;
class QContextMenuEvent;
class QAction;
class Button : public QWidget
{
    Q_OBJECT
public:
    Button(QWidget *parent = nullptr);

    const QPixmap &pixmap() {return m_pixmap;}
    void setPixmap(const QPixmap &pixmap);

    const QString &text() {return m_text;}
    void setText(const QString &text);

    const QString &id() {return m_id;}
    void setId(const QString &id);

    const Qt::Alignment textAlignment() {return m_align;}
    void setTextAlignment(Qt::Alignment align);

    int radius() {return m_radius;}
    void setRadius(int radius);

    void addAction(QAction *action);
    void clear();

public slots:
    void hideMenu();

signals:
    void clicked();
    void toggled(const QString &id);

private:
    QPixmap m_pixmap;
    QString m_text;
    QString m_id;
    int m_radius;
    bool m_in;
    bool m_hasMenu;
    Qt::Alignment m_align;
    QMenu *m_menu = nullptr;

    QRectF m_contentRect;
    QRectF m_menuRect;

protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
};
#endif // BUTTON_H
