#include "bubblegroup.h"
#include "bubbleitem.h"
#include "notifymodel.h"
#include "bubbledelegate.h"
#include "notification/notificationentity.h"
#include "notification/constants.h"

#include <QListView>
#include <QBoxLayout>
#include <DGuiApplicationHelper>

DWIDGET_USE_NAMESPACE

BubbleGroup::BubbleGroup(QWidget *parent, std::shared_ptr<NotifyModel> model)
    : QWidget(parent)
    , m_notifyModel(model)
{
    m_titleWidget = new QWidget();
    m_titleWidget->setFixedSize(OSD::BubbleWidth(OSD::ShowStyle::BUBBLEWIDGET), Notify::GroupTitleHeight);

    group_title = new DLabel;
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

    m_groupList = new DListView(this);
    m_notifyDelegate = new BubbleDelegate(this);
    m_notifyModel->setView(m_groupList);
    m_groupList->setModel(m_notifyModel.get());
    m_groupList->setItemDelegate(m_notifyDelegate);

    m_groupList->setAutoFillBackground(false);
    m_groupList->viewport()->setAutoFillBackground(false);
    m_groupList->setFrameStyle(QFrame::NoFrame);
    m_groupList->setMouseTracking(true);
    m_groupList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_groupList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_groupList->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_groupList->setContentsMargins(0, 0, 0, 0);
    m_groupList->setUpdatesEnabled(true);
    m_groupList->setSelectionMode(QListView::NoSelection);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(10);
    mainLayout->setMargin(0);
    mainLayout->addWidget(m_titleWidget);
    mainLayout->addWidget(m_groupList);

    setLayout(mainLayout);

    connect(title_close, &DIconButton::clicked, this, &BubbleGroup::closeGroup);
    connect(m_notifyModel.get(), &NotifyModel::expandNotify, this, &BubbleGroup::expandAnimation);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &BubbleGroup::refreshTheme);
    refreshTheme();
}

void BubbleGroup::enterEvent(QEvent *)
{
    title_close->setVisible(true);
}

void BubbleGroup::leaveEvent(QEvent *)
{
    title_close->setVisible(false);
}

void BubbleGroup::expandAnimation()
{
    if (m_expandAnimation.isNull()) {
        m_expandAnimation = new ExpandAnimation(m_groupList);

        connect(m_expandAnimation, &ExpandAnimation::finished, [ = ]() {
            m_expandAnimation->hide();
            m_expandAnimation->deleteLater();
            m_notifyModel->refreshContent();
        });
    }

    int bubble_count = m_notifyModel->rowCount();
    int need_bubble =  parentWidget()->height() / OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET);
    need_bubble = bubble_count < need_bubble ? bubble_count : need_bubble;
    int expand_height = need_bubble * OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET);
    m_expandAnimation->setFixedSize(OSD::BubbleWidth(OSD::ShowStyle::BUBBLEWIDGET), expand_height);

    int y = BubbleEntities * OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + Notify::CenterMargin * BubbleEntities;
    m_expandAnimation->move(0, y);
    m_expandAnimation->raise();
    m_expandAnimation->show();

    auto notifications = m_notifyModel->allNotifys();

    need_bubble -= BubbleEntities;
    m_expandAnimation->addData(notifications.mid(BubbleEntities, need_bubble));
    m_expandAnimation->start();
}

void BubbleGroup::refreshTheme()
{
    QFont font;
    font.setBold(true);
    group_title->setFont(DFontSizeManager::instance()->t4(font));

    QPalette pa = group_title->palette();
    pa.setBrush(QPalette::WindowText, pa.brightText());
    group_title->setPalette(pa);
}
