// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef RECOGNIZEDIALOG_H
#define RECOGNIZEDIALOG_H

#include <com_deepin_daemon_display_monitor.h>

#include <DBlurEffectWidget>

#include <QBrush>
#include <QPen>

DWIDGET_USE_NAMESPACE
using namespace com::deepin::daemon::display;

class RecognizeDialog : public DBlurEffectWidget
{
    Q_OBJECT

public:
    explicit RecognizeDialog(Monitor *monitor, QWidget *parent = nullptr);
    void setText(const QString text);

protected:
    void paintEvent(QPaintEvent *);

public Q_SLOTS:
    void onScreenRectChanged();

private:
    Monitor *m_monitor;
    QRect m_rect;
    QString m_text;
};

#endif // !RECOGNIZEDIALOG_H
