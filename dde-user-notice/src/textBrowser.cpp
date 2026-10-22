// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "textBrowser.h"

#include <QEvent>

#include <DFontSizeManager>
#include <DPaletteHelper>

DWIDGET_USE_NAMESPACE

TextBrowser::TextBrowser(QWidget *parent)
    : QTextBrowser(parent)
{
    setAlignment(Qt::AlignTop);
    setFrameShape(QFrame::Shape::NoFrame);
    setReadOnly(true);
    setLineWrapMode(QTextEdit::LineWrapMode::WidgetWidth);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setAttribute(Qt::WA_TranslucentBackground, true);

    DFontSizeManager::instance()->bind(this, DFontSizeManager::T6);

    DPalette pa = DPaletteHelper::instance()->palette(this);
    pa.setColor(DPalette::ColorRole::Base, Qt::transparent);
    // 须知界面固定使用深色背景，正文需固定为白色，避免跟随系统主题变成深色文字
    pa.setColor(DPalette::ColorRole::Text, Qt::white);
    DPaletteHelper::instance()->setPalette(this, pa);
}

void TextBrowser::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
