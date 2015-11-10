#include <QtGui/QPainter>
#include <QDebug>
#include <QTimer>

#include "userbutton.h"

UserButton::UserButton(QString iconUrl, QString idName, QWidget *parent)
    : QPushButton(parent),
      m_moveAni(new QPropertyAnimation(this, "pos")),
      m_opacity(0),
      m_showAnimation(new QPropertyAnimation(this, "opacity")),
      m_hideAnimation(new QPropertyAnimation(this, "opacity"))
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

    m_textLabel = new QLabel;
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

    setLayout(m_Layout);

    m_opacityEffect = new QGraphicsOpacityEffect;

    connect(this, &UserButton::opacityChanged, &UserButton::setCustomEffect);
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

void UserButton::showButton()
{
    m_showAnimation->setStartValue(0);
    m_showAnimation->setEndValue(1);
    m_showAnimation->setDuration(800);

    m_showAnimation->start();

    connect(m_showAnimation, &QPropertyAnimation::finished, [=](){
        qDebug() << "xxxx";

        QTimer::singleShot(500, [=]{
            m_opacityEffect->setEnabled(false);
            addTextShadow(true);
        });
    });
}

void UserButton::hide(const int duration)
{
    Q_UNUSED(duration);
    m_hideAnimation->setStartValue(1);
    m_hideAnimation->setEndValue(0);
    m_hideAnimation->start();

       addTextShadow(false);
       m_opacityEffect->setEnabled(true);

       connect(m_hideAnimation, &QPropertyAnimation::finished, [=](){
           QPushButton::hide();
       });
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

void UserButton::addTextShadow(bool isEffective) {
    QGraphicsDropShadowEffect *nameShadow = new QGraphicsDropShadowEffect;
    nameShadow->setBlurRadius(16);
    nameShadow->setColor(QColor(0, 0, 0, 85));
    nameShadow->setOffset(0, 4);
    nameShadow->setEnabled(isEffective);
    m_textLabel->setGraphicsEffect(nameShadow);
}

void UserButton::stopAnimation()
{
    m_moveAni->stop();
}

double UserButton::opacity() {
    return m_opacity;
}

void UserButton::setOpacity(double opa) {
    if (m_opacity != opa) {
        m_opacity = opa;
        emit opacityChanged();
    }
}

void UserButton::setCustomEffect() {
    m_opacityEffect->setOpacity(m_opacity);
    setGraphicsEffect(m_opacityEffect);
}

UserButton::~UserButton()
{}
