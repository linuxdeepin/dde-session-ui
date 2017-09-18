/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
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
#include <QFileSystemWatcher>

class FullscreenBackground : public QWidget
{
    Q_OBJECT

public:
    explicit FullscreenBackground(QWidget *parent = 0);

public slots:
    void setBackground(const QString &file);
    void setBackground(const QPixmap &pixmap);

protected:
    void setContent(QWidget * const w);

private slots:
    void adjustGeometry();
    const QString getBlurImagePath(const QString &path);

private:
    bool eventFilter(QObject *watched, QEvent *event);
    void showEvent(QShowEvent *e);
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    QString m_bgPath;
    QPixmap m_background;
    QPointer<QWidget> m_content;
    QFileSystemWatcher m_blurWatcher;
    QTimer *m_adjustTimer;
};

#endif // FULLSCREENBACKGROUND_H
