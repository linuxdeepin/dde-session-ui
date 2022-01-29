/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
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

#ifndef DMEMORYWARNINGDIALOG_H
#define DMEMORYWARNINGDIALOG_H

#include "processinfomodel.h"

#include <QDialog>
#include <QPushButton>
#include <QLabel>

#include <dabstractdialog.h>

class ProcessInfoModel;

DWIDGET_USE_NAMESPACE

bool confirm(const QPixmap &icon);

class DMemoryWarningDialog : public DAbstractDialog
{
    Q_OBJECT

public:
    explicit DMemoryWarningDialog(QWidget *parent = nullptr);
    ~DMemoryWarningDialog();

    enum TipsType
    {
        LaunchApp,
        OpenChromeTab,
        ExecuteCommand,
    };

public slots:
    void updateAppInfo(const QString &appInfo);

private:
    void keyPressEvent(QKeyEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private slots:
    void onNeedMemChanged(const qulonglong needed);
    void onContinueClicked();
    void onCancelClicked();
    void updateTips();

private:
    int m_needed;
    TipsType m_tipsType;
    QString m_appName;
    QPointer<StartManagerInter> m_startManagerInter;
    ProcessInfoModel *m_infoModel;
    QLabel *m_icon;
    QLabel *m_memNeeded;
    QPushButton *m_cancelButton;
    QPushButton *m_continueButton;
};

#endif // DMEMORYWARNINGDIALOG_H
