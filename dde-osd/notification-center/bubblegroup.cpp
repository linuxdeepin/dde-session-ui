#include "bubblegroup.h"
#include "bubbleitem.h"
#include "notification/notificationentity.h"

#include <DLabel>
#include <QListWidget>

DWIDGET_USE_NAMESPACE

BubbleGroup::BubbleGroup(QWidget* parent, std::shared_ptr<NotificationEntity> entity)
    : QWidget (parent)
{
    m_titleWidget = new QWidget;
    DLabel *group_title = new DLabel;
    group_title->setText(entity->appName());
    DIconButton *title_close = new DIconButton(DStyle::SP_CloseButton);
    connect(title_close, &DIconButton::clicked, this, &BubbleGroup::closeGroup);

    QHBoxLayout *head_Layout = new QHBoxLayout;
    head_Layout->setMargin(10);
    head_Layout->addWidget(group_title, Qt::AlignLeft);
    head_Layout->addStretch();
    head_Layout->addWidget(title_close, Qt::AlignRight);
    m_titleWidget->setLayout(head_Layout);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_titleWidget);
    mainLayout->setMargin(10);

    setLayout(mainLayout);
}

void BubbleGroup::pushBubble(std::shared_ptr<NotificationEntity> entity)
{
    BubbleItem* bubble = new BubbleItem(entity);
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
