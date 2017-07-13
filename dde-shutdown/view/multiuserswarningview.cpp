#include "multiuserswarningview.h"

#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>

#include "accountsutils.h"

const static QSize ViewSize = QSize(500, 500);

const static QSize UserAvatarSize = QSize(64, 64);
const static QSize UserListItemSize = QSize(180, 80);

MultiUsersWarningView::MultiUsersWarningView(QWidget *parent) :
    QFrame(parent),
    m_vLayout(new QVBoxLayout(this)),
    m_userList(new QListWidget),
    m_warningTip(new QLabel),
    m_cancelBtn(new DImageButton),
    m_actionBtn(new DImageButton)
{
    setFixedSize(ViewSize);

    m_userList->setAttribute(Qt::WA_TranslucentBackground);
//    m_userList->setSelectionRectVisible(false);
    m_userList->setSelectionMode(QListView::NoSelection);
    m_userList->setEditTriggers(QListView::NoEditTriggers);
    m_userList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_userList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    m_userList->viewport()->setAttribute(Qt::WA_TranslucentBackground);
    m_userList->setFrameStyle(QFrame::NoFrame);
    m_userList->setGridSize(UserListItemSize);
    m_userList->setFocusPolicy(Qt::NoFocus);
    m_userList->setStyleSheet("background-color:transparent;");

    m_warningTip->setStyleSheet("color: white;"
                                "font-size: 14px;");
    m_warningTip->setWordWrap(true);
    m_warningTip->setFixedWidth(500);
    m_warningTip->setAlignment(Qt::AlignHCenter);

    m_cancelBtn->setNormalPic(":/img/cancel_normal.png");
    m_cancelBtn->setHoverPic(":/img/cancel_hover.png");
    m_cancelBtn->setPressPic(":/img/cancel_press.png");

    QHBoxLayout * btnLayout = new QHBoxLayout;
    btnLayout->addStretch(1);
    btnLayout->addWidget(m_cancelBtn);
    btnLayout->addWidget(m_actionBtn);
    btnLayout->addStretch(1);

    m_vLayout->addWidget(m_userList, 0, Qt::AlignHCenter);
    m_vLayout->addSpacing(40);
    m_vLayout->addWidget(m_warningTip, 0, Qt::AlignHCenter);
    m_vLayout->addSpacing(40);
    m_vLayout->addLayout(btnLayout);
    m_vLayout->addStretch();

    connect(m_cancelBtn, &DImageButton::clicked, this, &MultiUsersWarningView::cancelled);
    connect(m_actionBtn, &DImageButton::clicked, this, &MultiUsersWarningView::actionInvoked);
}

void MultiUsersWarningView::setUsers(QStringList &users)
{
    m_userList->setFixedSize(UserListItemSize.width(),
                             UserListItemSize.height() * qMin(users.length(), 4));

    for (QString user : users) {
        QListWidgetItem * item = new QListWidgetItem;
        m_userList->addItem(item);

        QString icon = AccountsUtils::GetUserAvatar(user);
        m_userList->setItemWidget(item, new UserListItem(icon, user));
    }
}

Actions MultiUsersWarningView::action() const
{
    return m_action;
}

void MultiUsersWarningView::setAction(const Actions action)
{
    switch (action) {
    case Actions::Shutdown:
        m_actionBtn->setNormalPic(":/img/poweroff_warning_normal.png");
        m_actionBtn->setHoverPic(":/img/poweroff_warning_hover.png");
        m_actionBtn->setPressPic(":/img/poweroff_warning_press.png");
        m_warningTip->setText(tr("The above users still keep logged in and the data will be lost due to shutdown, are you sure to shut down? "));
        break;
    default:
        m_actionBtn->setNormalPic(":/img/reboot_warning_normal.png");
        m_actionBtn->setHoverPic(":/img/reboot_warning_hover.png");
        m_actionBtn->setPressPic(":/img/reboot_warning_press.png");
        m_warningTip->setText(tr("The above users still keep logged in and the data will be lost due to reboot, are you sure to reboot? "));
        break;
    }
}

UserListItem::UserListItem(QString &icon, QString &name) :
    QFrame(),
    m_icon(new QLabel(this)),
    m_name(new QLabel(name, this))
{
    setFixedSize(UserListItemSize);

    m_icon->setFixedSize(UserAvatarSize);
    m_icon->setScaledContents(true);
    m_icon->setPixmap(getRoundPixmap(icon));

    m_name->setStyleSheet("color: white; font-size: 14px");
    m_name->move(80, 20);
}

QPixmap UserListItem::getRoundPixmap(QString &path)
{
    QPixmap source(path);
    QPixmap result(source.size());
    result.fill(Qt::transparent);

    QPainter p(&result);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath pp;
    pp.addEllipse(result.rect());
    p.setClipPath(pp);
    p.drawPixmap(result.rect(), source);
    p.end();

    return result;
}
