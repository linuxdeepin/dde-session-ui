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
#include <com_deepin_daemon_imageblur.h>

using ImageBlur = com::deepin::daemon::ImageBlur;

class FadeoutBackground : public QFrame {
    Q_OBJECT
public:
    explicit FadeoutBackground(QWidget *parent = 0);

    void showFadeOut(const QPixmap &oldPixmap);

signals:
    void fadeoutFinished(const QPixmap &oldPixmap);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QPropertyAnimation *m_backgroundAnimation;
    QPixmap m_fadeoutPixmap;
};

class FullscreenBackground : public QWidget
{
    Q_OBJECT
public:
    explicit FullscreenBackground(QWidget *parent = 0);

public slots:
    void updateBackground(const QPixmap &background);
    void updateBackground(const QString &file);

protected:
    void setContent(QWidget * const w);
    void keyPressEvent(QKeyEvent *e);

private slots:
    void adjustGeometry();
    void onBlurFinished(const QString &source, const QString &blur, bool status);

private:
    bool eventFilter(QObject *watched, QEvent *event);
    void showEvent(QShowEvent *e);
    void paintEvent(QPaintEvent *e);

    void setGeometry(const QRect &rect);
    const QScreen *screenForGeometry(const QRect &rect) const;

private:
    QString m_bgPath;
    QPixmap m_background;
    QPointer<QWidget> m_content;
    QTimer *m_adjustTimer;
    ImageBlur *m_blurImageInter;
    FadeoutBackground *m_fakeBackground;
};

#endif // FULLSCREENBACKGROUND_H
