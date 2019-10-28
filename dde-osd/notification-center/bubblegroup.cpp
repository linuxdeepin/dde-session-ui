#include "bubblegroup.h"
#include "bubbleitem.h"
#include "notification/notificationentity.h"

#include <DLabel>
#include <QListWidget>

DWIDGET_USE_NAMESPACE

BubbleGroup::BubbleGroup(QWidget *parent, std::shared_ptr<NotificationEntity> entity)
    : QWidget(parent)
{
    m_titleWidget = new QWidget();
    m_titleWidget->setFixedSize(BubbleItemWidth, GroupTitleHeight);

    QFont font;
    font.setPointSize(16);
    font.setBold(true);

    DLabel *group_title = new DLabel;
    group_title->setText(entity->appName());
    group_title->setFont(font);

    title_close = new DIconButton(DStyle::SP_CloseButton);
    title_close->setFlat(true);
    title_close->setIconSize(QSize(GroupButtonSize, GroupButtonSize));
    title_close->setFixedSize(GroupButtonSize, GroupButtonSize);
    title_close->setVisible(false);
    connect(title_close, &DIconButton::clicked, this, &BubbleGroup::closeGroup);

    QHBoxLayout *head_Layout = new QHBoxLayout;
    head_Layout->setContentsMargins(10, 0, 4, 0);
    head_Layout->setSpacing(20);
    head_Layout->addWidget(group_title, Qt::AlignLeft);
    head_Layout->addStretch();
    head_Layout->addWidget(title_close, Qt::AlignRight);
    m_titleWidget->setLayout(head_Layout);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_titleWidget);
    mainLayout->setSpacing(10);
    mainLayout->setMargin(0);

    setLayout(mainLayout);
}

void BubbleGroup::pushBubble(std::shared_ptr<NotificationEntity> entity)
{
    BubbleItem *bubble = new BubbleItem(entity);
    m_bubbleList.push_front(bubble);
    mainLayout->addWidget(bubble, Qt::AlignTop | Qt::AlignVCenter);
}

void BubbleGroup::popBubble()
{

}

void BubbleGroup::closeGroup()
{
    setVisible(false);
    deleteLater();
    qDeleteAll(m_bubbleList);

    //TODO: database content must clear
}

void BubbleGroup::enterEvent(QEvent*)
{
    title_close->setVisible(true);
}

void BubbleGroup::leaveEvent(QEvent*)
{
    title_close->setVisible(false);
}
