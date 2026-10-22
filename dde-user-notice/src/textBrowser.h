// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TEXTBROWSER_H
#define TEXTBROWSER_H

#include <QTextBrowser>

class TextBrowser : public QTextBrowser
{
public:
    explicit TextBrowser(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *e) override;
};

#endif // TEXTBROWSER_H
