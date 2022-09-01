// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INDICATORPROVIDER_H
#define INDICATORPROVIDER_H

#include "abstractosdprovider.h"
/*!
 * \~chinese \class IndicatorProvider
 * \~chinese \brief 外设模式切换时的通知类
 */
class IndicatorProvider : public AbstractOSDProvider
{
    Q_OBJECT
public:
    explicit IndicatorProvider(QObject *parent = nullptr);

    // model
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    // delegate
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool match(const QString &param) override;

private:
    QString m_param;
    QMap<QString, QString> m_dats;
};

#endif // INDICATORPROVIDER_H
