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

#include <DConfig>

#include <com_deepin_daemon_display.h>
#include <com_deepin_daemon_appearance.h>

using namespace com::deepin::daemon;
DCORE_USE_NAMESPACE

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
    explicit DisplayModeProvider(QObject *parent = nullptr);

    bool checkConditions() const override;
    QMargins contentMargins() const override; // 中心窗口距离边缘距离
    QSize contentSize() const override; // 中心窗口大小

    QListView::Flow flow() const override;

    void highlightCurrent() override;
    void highlightNext() override;

    void sync() override;
    void sync(const QModelIndex &index) override;

    int currentRow() override;

    // model
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    // delegate
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool match(const QString &param) override;

private:
    void updateOutputNames();
    void updatePlanItems();

    QString getPlanItemName(QPair<uchar,QString> &plan) const;
    QString getPlanItemIconName(const int index) const;

private slots:
    void displayModeChanged(const uchar &mode);
    void primaryChanged(const QString &primary);

private:
    QList<QPair<uchar,QString>> m_planItems;
    QPair<uchar,QString> m_currentPlan;

    uchar m_displayMode;
    QStringList m_outputNames;
    QString m_primaryScreen;

    Display *m_displayInter;
    uint m_state;
};

#endif // DISPLAYMODEPROVIDER_H
