/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "inhibitwarnview.h"
#include "framedatabind.h"

#include <QHBoxLayout>

InhibitorRow::InhibitorRow(QString who, QString why, const QIcon &icon, QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;
    QLabel *whoLabel = new QLabel(who);
    QLabel *whyLabel = new QLabel("-" + why);
    whoLabel->setStyleSheet("color: white; font: bold 12px;");
    whyLabel->setStyleSheet("color: white;");

    layout->addStretch();

    if (!icon.isNull()) {
        QLabel *iconLabel = new QLabel(this);
        QPixmap pixmap = icon.pixmap(topLevelWidget()->windowHandle(), QSize(48, 48));
        iconLabel->setPixmap(pixmap);
        iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        layout->addWidget(iconLabel);
    }

    layout->addWidget(whoLabel);
    layout->addWidget(whyLabel);
    layout->addStretch();
    this->setLayout(layout);
}

InhibitorRow::~InhibitorRow()
{

}

InhibitWarnView::InhibitWarnView(Actions inhibitType, QWidget *parent)
    : WarningView(parent)
    , m_inhibitType(inhibitType)
{
    m_acceptBtn = new RoundItemButton(QString());
    m_cancelBtn = new RoundItemButton(tr("Cancel"));
    m_acceptBtn->setObjectName("AcceptButton");
    m_cancelBtn->setObjectName("CancelButton");
    m_confirmTextLabel = new QLabel;

    m_inhibitorListLayout = new QVBoxLayout;

    std::function<void (QVariant)> buttonChanged = std::bind(&InhibitWarnView::onOtherPageDataChanged, this, std::placeholders::_1);
    m_dataBindIndex = FrameDataBind::Instance()->registerFunction("InhibitWarnView", buttonChanged);

    m_cancelBtn->setNormalPic(":/img/cancel_normal.svg");
    m_cancelBtn->setHoverPic(":/img/cancel_hover.svg");
    m_cancelBtn->setPressPic(":/img/cancel_press.svg");

    m_confirmTextLabel->setText("The reason of inhibit.");
    m_confirmTextLabel->setAlignment(Qt::AlignCenter);
    m_confirmTextLabel->setStyleSheet("color:white;");

    QVBoxLayout *cancelLayout = new QVBoxLayout;
    cancelLayout->addWidget(m_cancelBtn);

    QVBoxLayout *acceptLayout = new QVBoxLayout;
    acceptLayout->addWidget(m_acceptBtn);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->setSpacing(20);
    btnsLayout->setObjectName("ButtonLayout");
    btnsLayout->addStretch();
    btnsLayout->addLayout(cancelLayout);
    btnsLayout->addLayout(acceptLayout);
    btnsLayout->addStretch();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addStretch();
    centralLayout->addLayout(m_inhibitorListLayout);
    centralLayout->addSpacing(20);
    centralLayout->addWidget(m_confirmTextLabel);
    centralLayout->addSpacing(20);
    centralLayout->addLayout(btnsLayout);
    centralLayout->addStretch();

    setLayout(centralLayout);

    m_acceptBtn->setChecked(true);
    m_currentBtn = m_acceptBtn;

    connect(m_cancelBtn, &RoundItemButton::clicked, this, &InhibitWarnView::cancelled);
    connect(m_acceptBtn, &RoundItemButton::clicked, [this] {emit actionInvoked(m_action);});
}

InhibitWarnView::~InhibitWarnView()
{
    FrameDataBind::Instance()->unRegisterFunction("InhibitWarnView", m_dataBindIndex);
}

void InhibitWarnView::setInhibitorList(const QList<InhibitorData> &list)
{
    for (QWidget* widget : m_inhibitorPtrList) {
        m_inhibitorListLayout->removeWidget(widget);
        widget->deleteLater();
    };
    m_inhibitorPtrList.clear();

    for (const InhibitorData &inhibitor : list) {
        QIcon icon;

        if (inhibitor.pid) {
            QFileInfo executable_info(QFile::readLink(QString("/proc/%1/exe").arg(inhibitor.pid)));

            if (executable_info.exists()) {
                icon = QIcon::fromTheme(executable_info.fileName());
            }
        }

        if (icon.isNull()) {
            icon = QIcon::fromTheme("application-x-desktop");
        }

        QWidget * inhibitorWidget = new InhibitorRow(inhibitor.who, inhibitor.why, icon, this);
        m_inhibitorPtrList.append(inhibitorWidget);
        m_inhibitorListLayout->addWidget(inhibitorWidget);
    }
}

void InhibitWarnView::setInhibitConfirmMessage(const QString &text)
{
    m_confirmTextLabel->setText(text);
}

void InhibitWarnView::setAcceptReason(const QString &reason)
{
    m_acceptBtn->setText(reason);
}

void InhibitWarnView::setAction(const Actions action)
{
    m_action = action;

    switch (action) {
    case Actions::Shutdown:
        m_acceptBtn->setNormalPic(":/img/poweroff_warning_normal.svg");
        m_acceptBtn->setHoverPic(":/img/poweroff_warning_hover.svg");
        m_acceptBtn->setPressPic(":/img/poweroff_warning_press.svg");
        break;
    case Actions::Logout:
        m_acceptBtn->setNormalPic(":/img/logout_warning_normal.svg");
        m_acceptBtn->setHoverPic(":/img/logout_warning_hover.svg");
        m_acceptBtn->setPressPic(":/img/logout_warning_press.svg");
        break;
    default:
        m_acceptBtn->setNormalPic(":/img/reboot_warning_normal.svg");
        m_acceptBtn->setHoverPic(":/img/reboot_warning_hover.svg");
        m_acceptBtn->setPressPic(":/img/reboot_warning_press.svg");
        break;
    }
}

void InhibitWarnView::setAcceptVisible(const bool acceptable)
{
    m_acceptBtn->setVisible(acceptable);
}

void InhibitWarnView::toggleButtonState()
{
    if (m_acceptBtn->isChecked()) {
        m_acceptBtn->setChecked(false);
        m_cancelBtn->setChecked(true);
        m_currentBtn = m_cancelBtn;
    } else {
        m_cancelBtn->setChecked(false);
        m_acceptBtn->setChecked(true);
        m_currentBtn = m_acceptBtn;
    }

    FrameDataBind::Instance()->updateValue("InhibitWarnView", m_currentBtn->objectName());
}

void InhibitWarnView::buttonClickHandle()
{
    emit m_currentBtn->clicked();
}

Actions InhibitWarnView::inhibitType() const
{
    return m_inhibitType;
}

void InhibitWarnView::onOtherPageDataChanged(const QVariant &value)
{
    const QString objectName { value.toString() };

    if (objectName == "AcceptButton") {
        m_cancelBtn->setChecked(false);
        m_acceptBtn->setChecked(true);
        m_currentBtn = m_acceptBtn;
    }
    else {
        m_acceptBtn->setChecked(false);
        m_cancelBtn->setChecked(true);
        m_currentBtn = m_cancelBtn;
    }
}
