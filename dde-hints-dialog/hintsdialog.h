/*
 * Copyright (C) 2020 ~ 2022 Uniontech Technology Co., Ltd.
 *
 * Author:     chenjun <chenjun@uniontech.com>
 *
 * Maintainer: chenjun <chenjun@uniontech.com>
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

#ifndef HINTSDIALOG_H
#define HINTSDIALOG_H

#include <dabstractdialog.h>
#include <dtiplabel.h>
#include <ddialogclosebutton.h>

#include <QWidget>

DWIDGET_USE_NAMESPACE

class QLabel;

class HintsDialog : public DAbstractDialog
{
    Q_OBJECT

public:
    explicit HintsDialog(QWidget *parent = nullptr);
    ~HintsDialog() override;
    void setHintTitle(const QString &title);
    void setHintContent(const QString &message);

protected:
    void showEvent(QShowEvent *event) override;

private:
    const int windowFixedWidth = 400;
    QLabel *m_title;
    DTipLabel *m_content;
    DDialogCloseButton *m_closeButton;
};

#endif // HINTSDIALOG_H
