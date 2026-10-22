// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PRELOGINWINDOW_H
#define PRELOGINWINDOW_H

#include "textBrowser.h"

#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>

class PreLoginWindow : public QFrame
{
    Q_OBJECT

private:
    explicit PreLoginWindow(QWidget *parent = nullptr);

public:
    static PreLoginWindow *instance();
    void updateBrowserSize(int width);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void initUi();
    void initConnect();
    void acknowledge();

    TextBrowser *m_contentLabel;
    QPushButton *m_commitButton;
    QVBoxLayout *m_layout;
};

#endif // PRELOGINWINDOW_H
