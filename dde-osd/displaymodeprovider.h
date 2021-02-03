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

#ifndef DISPLAYMODEPROVIDER_H
#define DISPLAYMODEPROVIDER_H

#include "abstractosdprovider.h"

#include <com_deepin_daemon_display.h>
#include <com_deepin_daemon_appearance.h>

using namespace com::deepin::daemon;
/*!
 * \~chinese \class DisplayModeProvider
 * \~chinese \brief 切换屏幕模式时的通知类
 */

static const int ItemWidth = 370;
static const int ItemHeight = 72;
static const int IconSize = 54;

#define MERGE_MODE 1
#define EXTEND_MODE 2
#define SINGLE_MODE 3

#define CHECK_ICON_SIZE        16
#define ICON_HSPACE            22
#define CHECK_ICON_HSPACE      10

class DisplayModeProvider : public AbstractOSDProvider
{
    Q_OBJECT
public:
    explicit DisplayModeProvider(QObject *parent = 0);

    bool checkConditions() const Q_DECL_OVERRIDE;
    QMargins contentMargins() const Q_DECL_OVERRIDE; // 中心窗口距离边缘距离
    QSize contentSize() const Q_DECL_OVERRIDE; // 中心窗口大小

    QListView::Flow flow() const Q_DECL_OVERRIDE;

    void highlightCurrent() Q_DECL_OVERRIDE;
    void highlightNext() Q_DECL_OVERRIDE;

    void sync() Q_DECL_OVERRIDE;
    void sync(const QModelIndex &index) Q_DECL_OVERRIDE;

    int currentRow() Q_DECL_OVERRIDE;

    // model
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    // delegate
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QList<QPair<uchar,QString>> m_planItems;
    QPair<uchar,QString> m_currentPlan;

    uchar m_displayMode;
    QStringList m_outputNames;
    QString m_primaryScreen;

    Display *m_displayInter;
    Appearance *m_appearanceInter;

    void updateOutputNames();
    void updatePlanItems();

    QString getPlanItemName(QPair<uchar,QString> &plan) const;
    QString getPlanItemIcon(const int index) const;

private slots:
    void displayModeChanged(const uchar &mode);
    void primaryChanged(const QString &primary);
};

#endif // DISPLAYMODEPROVIDER_H
