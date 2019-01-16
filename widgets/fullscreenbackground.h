/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
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

#ifndef FULLSCREENBACKGROUND_H
#define FULLSCREENBACKGROUND_H

#include <QWidget>
#include <QPointer>
#include <QTimer>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QVariantAnimation>

class FullscreenBackground : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool contentVisible READ contentVisible WRITE setContentVisible NOTIFY contentVisibleChanged)

public:
    explicit FullscreenBackground(QWidget *parent = nullptr);

    bool contentVisible() const;

public slots:
    void updateBackground(const QPixmap &background);
    void updateBackground(const QString &file);
    void setScreen(QScreen *screen);
    void setContentVisible(bool contentVisible);

signals:
    void contentVisibleChanged(bool contentVisible);

protected:
    void setContent(QWidget * const w);
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) override;

private:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    const QPixmap pixmapHandle(const QPixmap &pixmap);

private:
    void updateScreen(QScreen *screen);
    void updateGeometry();
    using QWidget::setGeometry;
    using QWidget::resize;
    using QWidget::move;

    QString m_bgPath;
    QPixmap m_background;
    QPixmap m_fakeBackground;
    QPixmap m_backgroundCache;
    QPixmap m_fakeBackgroundCache;
    QPointer<QWidget> m_content;
    QVariantAnimation *m_fadeOutAni;
    QScreen *m_screen = nullptr;
};

#endif // FULLSCREENBACKGROUND_H
