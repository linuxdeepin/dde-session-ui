#include "inhibitwarnview.h"

#include <QHBoxLayout>

InhibitWarnView::InhibitWarnView(QWidget *parent)
    : QWidget(parent)
{
    m_acceptBtn = new DImageButton;
    m_cancelBtn = new DImageButton;
    m_reasonLbl = new QLabel;
    m_acceptLbl = new QLabel;
    m_acceptLbl->setAlignment(Qt::AlignCenter);

    m_cancelBtn->setNormalPic(":/img/cancel_normal.png");
    m_cancelBtn->setHoverPic(":/img/cancel_hover.png");
    m_cancelBtn->setPressPic(":/img/cancel_press.png");

    m_reasonLbl->setText("The reason of inhibit.");
    m_reasonLbl->setAlignment(Qt::AlignCenter);
    m_reasonLbl->setStyleSheet("color:white;"
                               "font-size:14px;");

    QLabel *cancenLbl = new QLabel;
    cancenLbl->setAlignment(Qt::AlignCenter);
    cancenLbl->setText(tr("Cancel"));
    cancenLbl->setStyleSheet("color:white;"
                             "font-size:14px;");

    m_acceptLbl->setStyleSheet("color:white;"
                               "font-size:14px;");

    QVBoxLayout *cancelLayout = new QVBoxLayout;
    cancelLayout->addWidget(m_cancelBtn);
    cancelLayout->addWidget(cancenLbl);

    QVBoxLayout *acceptLayout = new QVBoxLayout;
    acceptLayout->addWidget(m_acceptBtn);
    acceptLayout->addWidget(m_acceptLbl);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addLayout(cancelLayout);
    btnsLayout->addSpacing(20);
    btnsLayout->addLayout(acceptLayout);
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

void InhibitWarnView::setAcceptReason(const QString &reason)
{
    m_acceptLbl->setText(reason);
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
    case Actions::Logout:
        m_acceptBtn->setNormalPic(":/img/logout_warning_normal.png");
        m_acceptBtn->setHoverPic(":/img/logout_warning_hover.png");
        m_acceptBtn->setPressPic(":/img/logout_warning_press.png");
        break;
    default:
        m_acceptBtn->setNormalPic(":/img/reboot_warning_normal.png");
        m_acceptBtn->setHoverPic(":/img/reboot_warning_hover.png");
        m_acceptBtn->setPressPic(":/img/reboot_warning_press.png");
        break;
    }
}

void InhibitWarnView::setAcceptVisible(const bool acceptable)
{
    m_acceptBtn->setVisible(acceptable);
    m_acceptLbl->setVisible(acceptable);
}
