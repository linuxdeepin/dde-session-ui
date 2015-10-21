#include <QtCore/QObject>

#include "rounditembutton.h"

RoundItemButton::RoundItemButton(QString text, QString buttonId, QWidget* parent)
    : QPushButton(parent)
{
    m_buttonId = buttonId;
    m_buttonText = text;
    initUI();
    initConnect();
}
void RoundItemButton::initConnect() {
}

void RoundItemButton::initUI() {
    setFixedSize(QSize(120, 120));
    m_iconLabel = new QLabel;
    m_iconLabel->setFixedSize(QSize(75, 75));

    m_iconLabel->setObjectName(m_buttonId);
    m_iconLabel->setFocusPolicy(Qt::NoFocus);
    m_contentTextLabel = new QLabel;

    m_contentTextLabel->setStyleSheet("color: rgba(255, 255, 255, 255); "
                                      "font-size:16px; text-align:center;");
    m_contentTextLabel->setText(m_buttonText);
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
}

bool RoundItemButton::isChecked() const{
    return (m_checked);
}
void RoundItemButton::setChecked(bool checked){
    if (m_checked != checked){
        m_checked = checked;
        update();
    }
}
void RoundItemButton::setButtonMutex(QString buttonName) {

    m_iconLabel->setProperty("Hover", false);
    emit signalManager->buttonStyleChanged();
    if (buttonName != this->objectName()) {
        setChecked(false);
    } else {
        setChecked(true);
    }
}
void RoundItemButton::setButtonHoverMutex(QString buttonName) {

    if (buttonName != m_iconLabel->objectName()) {
        m_iconLabel->setProperty("Hover", false);
        emit signalManager->buttonStyleChanged();
    }
}
void RoundItemButton::setHover(bool isHover) {
    m_iconLabel->setProperty("Hover", isHover);
    emit signalManager->buttonStyleChanged();
}

void RoundItemButton::enterEvent(QEvent* event) {
    Q_UNUSED(event);
    m_iconLabel->setProperty("Hover", true);
    emit signalManager->setButtonHover(m_buttonId);
}
void RoundItemButton::leaveEvent(QEvent* event) {
    Q_UNUSED(event);
    m_iconLabel->setProperty("Hover", false);
    emit signalManager->buttonStyleChanged();
}
void RoundItemButton::mousePressEvent(QMouseEvent* e) {
    Q_UNUSED(e);
    m_iconLabel->setProperty("Checked", true);
    emit signalManager->buttonStyleChanged();
}
void RoundItemButton::mouseReleaseEvent(QMouseEvent* e) {
    Q_UNUSED(e);
    m_iconLabel->setProperty("Checked", false);
    emit signalManager->buttonStyleChanged();
    emit  buttonAction(m_buttonId);
}
void RoundItemButton::paintEvent(QPaintEvent* event){
     if (m_checked){
         QPainter painter(this);
         painter.setPen(QPen(QColor(255, 255, 255, 51), 2));
         painter.setBrush(QColor(0, 0 , 0, 76));
         painter.setRenderHint(QPainter::Antialiasing, true);
         painter.drawRoundedRect(QRect(2, 2, 116, 116), 10, 10, Qt::RelativeSize);
     }
     QPushButton::paintEvent(event);
}
void RoundItemButton::addTextShadow() {
    QGraphicsDropShadowEffect *nameShadow = new QGraphicsDropShadowEffect;
    nameShadow->setBlurRadius(16);
    nameShadow->setColor(QColor(0, 0, 0, 85));
    nameShadow->setOffset(0, 4);
    m_contentTextLabel->setGraphicsEffect(nameShadow);
}
RoundItemButton::~RoundItemButton()
{
}
