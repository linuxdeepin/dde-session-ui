#include <QtCore/QObject>
#include "sessionbutton.h"

SessionButton::SessionButton(QString text, QString buttonId, QWidget *parent)
    : QFrame(parent)
{
    setFixedSize(QSize(120, 150));
    m_contentButton = new QPushButton;
    m_contentButton->setFixedSize(QSize(120, 120));
    m_contentButton->setObjectName(buttonId);
    m_contentButton->setFocusPolicy(Qt::NoFocus);
    m_contentTextLabel = new QLabel;
    m_contentTextLabel->setStyleSheet("color: white; font-size:12pt;");
    m_contentTextLabel->setText(text);
    m_contentTextLabel->setFixedSize(QSize(m_contentButton->width(), 30));
    m_contentTextLabel->setAlignment(Qt::AlignCenter);

    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->addSpacing(2);
    m_Layout->addWidget(m_contentButton);
    m_Layout->addWidget(m_contentTextLabel);

    setLayout(m_Layout);

}

SessionButton::~SessionButton()
{

}
