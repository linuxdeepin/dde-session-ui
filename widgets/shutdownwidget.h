/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef SHUTDOWNWIDGET
#define SHUTDOWNWIDGET

#include <QFrame>

#include <functional>

#include "util_updateui.h"
#include "rounditembutton.h"
#include "sessionbasemodel.h"
#include "framedatabind.h"

class ShutdownWidget: public QFrame
{
    Q_OBJECT
public:
    ShutdownWidget(QWidget* parent = 0);
    ~ShutdownWidget();

    void setModel(SessionBaseModel * const model);

signals:
    void abortOperation();

public slots:
    void leftKeySwitch();
    void rightKeySwitch();
    void shutdownAction();

protected:
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *e) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initConnect();
    void updateTr(RoundItemButton * widget, const QString &tr);
    void onOtherPageChanged(const QVariant &value);

    SessionBaseModel *m_model;
    int m_index = 0;
    QHBoxLayout* m_Layout;
    QList<RoundItemButton*>* m_btnList;
    RoundItemButton* m_currentSelectedBtn = NULL;
    RoundItemButton* m_requireShutdownButton;
    RoundItemButton* m_requireRestartButton;
    RoundItemButton* m_requireSuspendButton;
    RoundItemButton* m_requireHibernateButton;
    QMap<RoundItemButton*, SessionBaseModel::PowerAction> m_actionMap;
    QList<std::pair<std::function<void (QString)>, QString>> m_trList;
    FrameDataBind *m_frameDataBind;
};
#endif // SHUTDOWNWIDGET

