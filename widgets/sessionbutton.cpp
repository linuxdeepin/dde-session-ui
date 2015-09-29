#include <QtCore/QObject>
#include "sessionbutton.h"

SessionButton::SessionButton(QString text, QString buttonId, QWidget *parent)
    : QFrame(parent)
{
    setObjectName("SessionButton");
    setFixedSize(QSize(140, 140));
//    this->setStyleSheet("background-color: red;");
    m_contentButton = new QPushButton;
//    m_contentButton->setStyleSheet("background-color: green;");
    m_contentButton->setFixedSize(QSize(100, 100));
    m_buttonId = buttonId;
    m_contentButton->setObjectName(buttonId);
    m_contentButton->setFocusPolicy(Qt::NoFocus);
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
    m_ButtonLayout->addWidget(m_contentButton);
    m_ButtonLayout->addStretch();

    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->addLayout(m_ButtonLayout);
    m_Layout->addSpacing(8);
    m_Layout->addWidget(m_contentTextLabel);
    m_Layout->addStretch(0);

    setLayout(m_Layout);
    initConnect();


}

SessionButton::~SessionButton()
{
}
void SessionButton::sendSignal() {
    qDebug() << "button clicked" << m_buttonId;
    emit  buttonAction(m_buttonId);
    qDebug() << "emit !";
}


bool SessionButton::isChecked() const{
    return m_checked;
}
QString SessionButton::buttonId() {
    return m_contentButton->objectName();
}
void SessionButton::setChecked(bool checked){
    if (m_checked != checked){
        m_checked = checked;
        update();
    }
}
void SessionButton::setButtonMutex(QString buttonName) {
    if (buttonName != this->objectName()) {
        qDebug() << buttonName << false;
        setChecked(false);
    } else {
        setChecked(true);
    }
}
void SessionButton::paintEvent(QPaintEvent *event){
     if (m_checked){
         QPainter painter(this);
         painter.setPen(QPen(QColor(255, 255, 255, 51), 2));
         painter.setBrush(QColor(0, 0 , 0, 76));
         painter.setRenderHint(QPainter::Antialiasing, true);
         painter.drawRoundedRect(QRect(2, 2, 136, 136), 10, 10, Qt::RelativeSize);
     }
     QFrame::paintEvent(event);
}
void SessionButton::initConnect() {
     connect(m_contentButton, SIGNAL(clicked()), this, SLOT(sendSignal()));
}
