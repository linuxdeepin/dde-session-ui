#include "bubblegroup.h"
#include "bubbleitem.h"
#include "notifymodel.h"
#include "bubbledelegate.h"
#include "notifycommon.h"
#include "notification/notificationentity.h"

#include <QListView>
#include <QBoxLayout>

DWIDGET_USE_NAMESPACE

BubbleGroup::BubbleGroup(QWidget *parent, std::shared_ptr<NotifyModel> model)
    : QWidget(parent)
{
    m_titleWidget = new QWidget();
    m_titleWidget->setFixedSize(Notify::BubbleItemWidth, Notify::GroupTitleHeight);

    QFont font;
    font.setPointSize(16);
    font.setBold(true);

    group_title = new DLabel;
    group_title->setFont(font);

    title_close = new DIconButton(DStyle::SP_CloseButton);
    title_close->setFlat(true);
    title_close->setIconSize(QSize(Notify::GroupButtonSize, Notify::GroupButtonSize));
    title_close->setFixedSize(Notify::GroupButtonSize, Notify::GroupButtonSize);
    title_close->setVisible(false);

    QHBoxLayout *head_Layout = new QHBoxLayout;
    head_Layout->setContentsMargins(10, 0, 10, 0);
    head_Layout->addWidget(group_title, Qt::AlignLeft);
    head_Layout->addStretch();
    head_Layout->addWidget(title_close, Qt::AlignRight);
    m_titleWidget->setLayout(head_Layout);

    m_notifyView = new QListView(this);
    m_notifyDelegate = new BubbleDelegate(this);
    model->setView(m_notifyView);
    m_notifyView->setModel(model.get());
    m_notifyView->setItemDelegate(m_notifyDelegate);

    m_notifyView->setFrameStyle(QFrame::NoFrame);
    m_notifyView->setMouseTracking(true);
    m_notifyView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_notifyView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_notifyView->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_notifyView->setSpacing(0);
    m_notifyView->setContentsMargins(0, 0, 0, 0);
    m_notifyView->setUpdatesEnabled(true);
    m_notifyView->setContentsMargins(0, 0, 0, 0);
    m_notifyView->setUpdatesEnabled(true);
    m_notifyView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_notifyView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(10);
    mainLayout->setMargin(0);
    mainLayout->addWidget(m_titleWidget, 0, Qt::AlignTop);
    mainLayout->addWidget(m_notifyView, 1);

    setLayout(mainLayout);
}

void BubbleGroup::enterEvent(QEvent *)
{
    title_close->setVisible(true);
}

void BubbleGroup::leaveEvent(QEvent *)
{
    title_close->setVisible(false);
}
