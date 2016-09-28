#include "inhibitwarnview.h"

#include <QHBoxLayout>

InhibitWarnView::InhibitWarnView(QWidget *parent)
    : QWidget(parent)
{
    m_acceptBtn = new DImageButton;
    m_cancelBtn = new DImageButton;
    m_reasonLbl = new QLabel;

    m_cancelBtn->setNormalPic(":/img/cancel_normal.png");
    m_cancelBtn->setHoverPic(":/img/cancel_hover.png");
    m_cancelBtn->setPressPic(":/img/cancel_press.png");

    m_reasonLbl->setText("The reason of inhibit.");
    m_reasonLbl->setStyleSheet("color:white;");

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addWidget(m_acceptBtn);
    btnsLayout->addSpacing(20);
    btnsLayout->addWidget(m_cancelBtn);
    btnsLayout->addStretch();

    QVBoxLayout *centeralLayout = new QVBoxLayout;
    centeralLayout->addWidget(m_reasonLbl);
    centeralLayout->addLayout(btnsLayout);

    setLayout(centeralLayout);
    setFixedSize(500, 300);

    connect(m_cancelBtn, &DImageButton::clicked, this, &InhibitWarnView::cancelled);
    connect(m_acceptBtn, &DImageButton::clicked, [this] {emit actionInvoked(m_action);});
}

void InhibitWarnView::setInhibitReason(const QString &reason)
{
    m_reasonLbl->setText(reason);
}

void InhibitWarnView::setAction(const Actions action)
{
    m_action = action;

    switch (action) {
    case Actions::Shutdown:
        m_acceptBtn->setNormalPic(":/img/poweroff_warning_normal.png");
        m_acceptBtn->setHoverPic(":/img/poweroff_warning_hover.png");
        m_acceptBtn->setPressPic(":/img/poweroff_warning_press.png");
        break;
    default:
        m_acceptBtn->setNormalPic(":/img/reboot_warning_normal.png");
        m_acceptBtn->setHoverPic(":/img/reboot_warning_hover.png");
        m_acceptBtn->setPressPic(":/img/reboot_warning_press.png");
        break;
    }
}
