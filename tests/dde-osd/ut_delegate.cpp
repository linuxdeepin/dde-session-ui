#define private public
#include "delegate.h"
#include "audioprovider.h"
#undef private

#include <QPainter>
#include <QWidget>

#include <gtest/gtest.h>

class UT_Delegate : public testing::Test
{

};

TEST_F(UT_Delegate, coverageTest)
{
    Delegate delegate;
    AudioProvider provider;
    delegate.setProvider(&provider);

    QWidget w;
    QPainter painter(&w);
    QStyleOptionViewItem option;
    option.rect = QRect(0, 0, 100, 100);
    option.state |= QStyle::State_Selected;
    delegate.paint(&painter, option, QModelIndex(0, 0, nullptr, nullptr));

    delegate.sizeHint(option, QModelIndex());
}
