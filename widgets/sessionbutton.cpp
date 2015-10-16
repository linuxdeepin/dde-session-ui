#include <QtCore/QObject>
#include "sessionbutton.h"

SessionButton::SessionButton(QString text, QString buttonId, QWidget *parent)
    : QPushButton(parent)
{
    setFixedSize(QSize(120, 120));
    m_iconLabel = new QLabel;
    m_iconLabel->setFixedSize(QSize(75, 75));
    m_buttonId = buttonId;
    m_iconLabel->setObjectName(buttonId);
    m_iconLabel->setFocusPolicy(Qt::NoFocus);
    m_contentTextLabel = new QLabel;

    m_contentTextLabel->setStyleSheet("color: rgba(255, 255, 255, 255); "
                                      "font-size:16px; text-align:center;");
    m_contentTextLabel->setText(text);
    m_contentTextLabel->setFixedSize(this->width(), 30);
    m_contentTextLabel->setAlignment(Qt::AlignHCenter);

    m_ButtonLayout = new QHBoxLayout;
    m_ButtonLayout->setMargin(0);
    m_ButtonLayout->setSpacing(0);
    m_ButtonLayout->addStretch();
    m_ButtonLayout->addWidget(m_iconLabel);
    m_ButtonLayout->addStretch();

    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->addSpacing(4);
    m_Layout->addLayout(m_ButtonLayout);
    m_Layout->setSpacing(2);
    m_Layout->addWidget(m_contentTextLabel);
    m_Layout->addStretch(0);
    addTextShadow();
    setLayout(m_Layout);

    initConnect();
}

SessionButton::~SessionButton()
{
}
void SessionButton::enterEvent(QEvent *event) {
    Q_UNUSED(event);
    m_iconLabel->setProperty("Hover", true);
    emit signalManager->setButtonHover(m_buttonId);
}
void SessionButton::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    m_iconLabel->setProperty("Hover", false);
    emit signalManager->buttonStyleChanged();
}
void SessionButton::mousePressEvent(QMouseEvent *e) {
    Q_UNUSED(e);
    m_iconLabel->setProperty("Checked", true);
    emit signalManager->buttonStyleChanged();
}
void SessionButton::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e);
    m_iconLabel->setProperty("Checked", false);
    emit signalManager->buttonStyleChanged();
    emit  buttonAction(m_buttonId);
}
bool SessionButton::isChecked() const{
    return (m_checked);
}
void SessionButton::setChecked(bool checked){
    if (m_checked != checked){
        m_checked = checked;
        update();
    }
}
void SessionButton::setButtonMutex(QString buttonName) {

    m_iconLabel->setProperty("Hover", false);
    emit signalManager->buttonStyleChanged();
    if (buttonName != this->objectName()) {
        setChecked(false);
    } else {
        setChecked(true);
    }
}
void SessionButton::setButtonHoverMutex(QString buttonName) {

    if (buttonName != m_iconLabel->objectName()) {
        m_iconLabel->setProperty("Hover", false);
        emit signalManager->buttonStyleChanged();
    }
}
void SessionButton::setHover(bool isHover) {
    m_iconLabel->setProperty("Hover", isHover);
    emit signalManager->buttonStyleChanged();
}
void SessionButton::paintEvent(QPaintEvent *event){
     if (m_checked){
         QPainter painter(this);
         painter.setPen(QPen(QColor(255, 255, 255, 51), 2));
         painter.setBrush(QColor(0, 0 , 0, 76));
         painter.setRenderHint(QPainter::Antialiasing, true);
         painter.drawRoundedRect(QRect(2, 2, 116, 116), 10, 10, Qt::RelativeSize);
     }
     QPushButton::paintEvent(event);
}
void SessionButton::addTextShadow() {
    QGraphicsDropShadowEffect *nameShadow = new QGraphicsDropShadowEffect;
    nameShadow->setBlurRadius(16);
    nameShadow->setColor(QColor(0, 0, 0, 85));
    nameShadow->setOffset(0, 4);
    m_contentTextLabel->setGraphicsEffect(nameShadow);
}
void SessionButton::initConnect() {
}
