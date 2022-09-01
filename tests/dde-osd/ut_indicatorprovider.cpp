// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#define private public
#include "indicatorprovider.h"
#undef private

#include <QPainter>
#include <QWidget>
#include <QTest>

#include <gtest/gtest.h>

class UT_IndicatorProvider : public testing::Test
{

};

class PaintWidget : public QWidget
{

public:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE
    {
        Q_UNUSED(event)
        QPainter painter(this);
        QStyleOptionViewItem option;
        option.rect = QRect(0, 0, 100, 100);
        option.state |= QStyle::State_Selected;
        m_provider->paint(&painter, option, QModelIndex());
    }

    void setProvider(IndicatorProvider *provider)
    {
        m_provider = provider;
    }

public:
    IndicatorProvider *m_provider;
};


TEST_F(UT_IndicatorProvider, coverageTest)
{
    IndicatorProvider provider;
    provider.rowCount(QModelIndex());
    EXPECT_TRUE(provider.match("CapsLockOn"));
    provider.data(QModelIndex(), Qt::DisplayRole);

    PaintWidget w;
    w.setProvider(&provider);
    w.show();
    QTest::qWait(100);
}
