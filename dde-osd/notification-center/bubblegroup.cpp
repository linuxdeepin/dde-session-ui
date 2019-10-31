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
    group_title->setFocusPolicy(Qt::NoFocus);

    QPalette pa;
    pa.setBrush(QPalette::WindowText, pa.brightText());
    group_title->setPalette(pa);

    title_close = new DIconButton(DStyle::SP_CloseButton);
    title_close->setFlat(true);
    title_close->setIconSize(QSize(Notify::GroupButtonSize, Notify::GroupButtonSize));
    title_close->setFixedSize(Notify::GroupButtonSize, Notify::GroupButtonSize);
    title_close->setVisible(false);

    QHBoxLayout *head_Layout = new QHBoxLayout;
    head_Layout->setContentsMargins(10, 0, 0, 0);
    head_Layout->addWidget(group_title, Qt::AlignLeft);
    head_Layout->addStretch();
    head_Layout->addWidget(title_close, Qt::AlignRight);
    m_titleWidget->setLayout(head_Layout);

    m_groupList = new QListView(this);
    m_notifyDelegate = new BubbleDelegate(this);
    model->setView(m_groupList);
    m_groupList->setModel(model.get());
    m_groupList->setItemDelegate(m_notifyDelegate);

    m_groupList->setAutoFillBackground(false);
    m_groupList->viewport()->setAutoFillBackground(false);
    m_groupList->setFrameStyle(QFrame::NoFrame);
    m_groupList->setMouseTracking(true);
    m_groupList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_groupList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_groupList->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_groupList->setSpacing(5);
    m_groupList->setContentsMargins(0, 0, 0, 0);
    m_groupList->setUpdatesEnabled(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_titleWidget);
    mainLayout->addWidget(m_groupList);

    setLayout(mainLayout);

    connect(title_close, &DIconButton::clicked, this, &BubbleGroup::closeGroup);
}

void BubbleGroup::enterEvent(QEvent *)
{
    title_close->setVisible(true);
}

void BubbleGroup::leaveEvent(QEvent *)
{
    title_close->setVisible(false);
}
