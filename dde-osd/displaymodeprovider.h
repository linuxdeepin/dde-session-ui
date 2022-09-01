// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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

class QGSettings;

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
    QList<QPair<uchar,QString>> m_planItems;
    QPair<uchar,QString> m_currentPlan;

    uchar m_displayMode;
    QStringList m_outputNames;
    QString m_primaryScreen;

    Display *m_displayInter;
    Appearance *m_appearanceInter;
    QGSettings *m_setting;
    uint m_state;

    void updateOutputNames();
    void updatePlanItems();

    QString getPlanItemName(QPair<uchar,QString> &plan) const;
    QString getPlanItemIcon(const int index) const;

private slots:
    void displayModeChanged(const uchar &mode);
    void primaryChanged(const QString &primary);
};

#endif // DISPLAYMODEPROVIDER_H
