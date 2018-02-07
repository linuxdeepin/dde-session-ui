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

#ifndef KBLAYOUTINDICATOR_H
#define KBLAYOUTINDICATOR_H

#include <QSystemTrayIcon>
#include <QMenu>

#include <com_deepin_daemon_inputdevice_keyboard.h>

using KeyboardInterface = com::deepin::daemon::inputdevice::Keyboard;

class DataWrapper : public QObject
{
    Q_OBJECT
public:
    explicit DataWrapper(KeyboardInterface *kinter, QObject *parent = nullptr);
    virtual ~DataWrapper() {}

    QString currentLayout() const;
    void setCurrentLayout(const QString &currentLayout);

    QStringList layoutList() const;
    void setLayoutList(const QStringList &layoutList);

    QString localizedNameOfLayout(const QString &layout) const;

    QStringList userLayoutList() const;

signals:
    void layoutListChanged(const QStringList &list);
    void currentLayoutChanged(const QString &layout);

private:
    KeyboardInterface *m_keyboardInter;

    QString m_currentLayout;
    QStringList m_layoutList;
    KeyboardLayoutList m_layoutListAll;

    void initLayoutListAll();
};

class KBLayoutIndicator : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit KBLayoutIndicator(QWidget *parent = nullptr);

private:
    KeyboardInterface *m_keyboardInter;

    DataWrapper *m_data;

    QMenu *m_menu;
    QAction *m_addLayoutAction;

    void updateMenu();
    void updateIcon();

    QString duplicateCheck(const QString &kb);
    QPixmap generateIconPixmap(const QString &name, qreal dpr = 1.0);

    void switchToNextLayout();

    void handleDataChanged();

    void handleActionTriggered(QAction *action);
    void handleActivated(QSystemTrayIcon::ActivationReason reason);
};

#endif // KBLAYOUTINDICATOR_H
