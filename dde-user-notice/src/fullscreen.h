// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FULLSCREEN_H
#define FULLSCREEN_H

#include "abstractfullbackgroundinterface.h"

#include <QImage>
#include <QPixmap>
#include <QWidget>

class QPaintEvent;
class QCloseEvent;
class QScreen;

class FullScreen : public QWidget, public AbstractFullBackgroundInterface
{
    Q_OBJECT

public:
    explicit FullScreen(QWidget *content, QWidget *parent = nullptr);

    void setScreen(QPointer<QScreen> screen, bool isVisible = true) override;

public Q_SLOTS:
    void onAuthFinished();

private:
    static QImage getBackgroundImage(const QSize &size, qreal scale);
    static QImage getCachedBackgroundImage(const QSize &size, qreal scale);
    void showContentToCutScreen();

protected:
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QWidget *m_content;

    bool m_showBlack;
    QPixmap m_backgroundPixmap;
    QRect m_newGeometry;
    quint64 m_backgroundRequest = 0;
};

#endif // FULLSCREEN_H
