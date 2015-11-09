#include <QtGui/QPainter>
#include <QDebug>
#include <QTimer>

#include "userbutton.h"

UserButton::UserButton(QString iconUrl, QString idName, QWidget *parent)
    : QPushButton(parent),
      m_moveAni(new QPropertyAnimation(this, "pos"))
{
    m_iconUrl = iconUrl;
    m_buttonId = idName;

    initUI();
    initConnect();
}

void UserButton::initConnect() {
    connect(this, SIGNAL(clicked()), this, SLOT(sendClicked()));
    connect(m_userAvatar, SIGNAL(userAvatarClicked()), this, SLOT(sendClicked()));
}

void UserButton::initUI() {
    setFixedSize(120, 150);
    m_userAvatar = new UserAvatar;
    m_userAvatar->setAvatarSize(UserAvatar::AvatarLargeSize);
    m_userAvatar->setFixedSize(120, 120);
    m_userAvatar->setIcon(m_iconUrl);
    m_userAvatar->setObjectName(m_buttonId);

    m_textLabel = new UserBreathingLabel;
    m_textLabel->setStyleSheet("text-align:center;color: rgba(255, 255, 255, 255);\
                                          font-size:16px; ");
    m_textLabel->setText(m_buttonId);
    m_textLabel->setFixedSize(this->width(), 30);
    m_textLabel->setAlignment(Qt::AlignHCenter);

    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->setMargin(0);
    m_buttonLayout->setSpacing(0);
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_userAvatar);
    m_buttonLayout->addStretch();

    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addLayout(m_buttonLayout);
    m_Layout->addWidget(m_textLabel);
    m_Layout->addStretch();
    setStyleSheet("border: none;");
    addTextShadow();
    setLayout(m_Layout);
}

void UserButton::sendClicked() {
    emit imageClicked(m_buttonId);
}

void UserButton::setImageSize(const AvatarSize &avatarsize) {
    if (avatarsize==AvatarLargerSize) {
        m_userAvatar->setAvatarSize(m_userAvatar->AvatarLargeSize);
    } else {
        m_userAvatar->setAvatarSize(m_userAvatar->AvatarSmallSize);
    }

    m_avatarsize = avatarsize;
    update();
}

void UserButton::show()
{
    m_textLabel->showLabel();

    if (!m_userAvatar->isVisible()) {
        QTimer::singleShot(100, this, SLOT(show()));
        m_userAvatar->showButton();
    }

    QPushButton::show();
    update();
}

void UserButton::hide(const int duration)
{
    if (!duration) {
        m_userAvatar->hideButton();
        m_textLabel->hideLabel();

        QPushButton::hide();
    } else
        QTimer::singleShot(duration, this, SLOT(hide()));
}

void UserButton::move(const QPoint &position, int duration)
{
    if (!duration)
        return QPushButton::move(position);

    m_moveAni->stop();
    m_moveAni->setDuration(duration);
    m_moveAni->setStartValue(pos());
    m_moveAni->setEndValue(position);

    m_moveAni->start();
}

void UserButton::addTextShadow() {
    QGraphicsDropShadowEffect *nameShadow = new QGraphicsDropShadowEffect;
    nameShadow->setBlurRadius(16);
    nameShadow->setColor(QColor(0, 0, 0, 85));
    nameShadow->setOffset(0, 4);
    m_textLabel->setGraphicsEffect(nameShadow);
}
//void UserButton::showButton() {
//    m_textLabel->showLabel();
//    if (!isVisible()) {
//        QTimer::singleShot(100, this, SLOT(show()));
//        m_userAvatar->showButton();
//    }
//    update();
//}
//void UserButton::hideButton() {
//    m_userAvatar->hideButton();
//    m_textLabel->hideLabel();
//}

void UserButton::stopAnimation()
{
    m_moveAni->stop();
}
UserButton::~UserButton()
{}
