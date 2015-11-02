#include "switchframe.h"

#include <QEvent>
#include <QDebug>
#include <QFile>
#include <QGraphicsDropShadowEffect>

SwitchFrame::SwitchFrame(QWidget* parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
}

SwitchFrame::~SwitchFrame()
{
}

void SwitchFrame::chooseToSession(const QString &session)
{
    m_tipsLabel->setText(session);

    const QString normalIcon = QString(":/sessions/sessions/%1_normal.png").arg(session);
    const QString hoverIcon = QString(":/sessions/sessions/%1_hover.png").arg(session);
    const QString checkedIcon = QString(":/sessions/sessions/%1_press.png").arg(session);

    if (QFile(normalIcon).exists() && QFile(hoverIcon).exists() && QFile(checkedIcon).exists()) {
        m_switchSessionButton->setNormalPic(normalIcon);
        m_switchSessionButton->setHoverPic(hoverIcon);
        m_switchSessionButton->setPressPic(checkedIcon);
    } else {
        m_switchSessionButton->setNormalPic(":/sessions/sessions/unknow_normal.png");
        m_switchSessionButton->setHoverPic(":/sessions/sessions/unknow_hover.png");
        m_switchSessionButton->setPressPic(":/sessions/sessions/unknow_press.png");
    }
}

void SwitchFrame::hideTips()
{
    m_tipsAni->setEndValue(QPoint(m_tipsWidget->width(), 0));
    m_tipsAni->setStartValue(QPoint());
    m_tipsAni->start();
}

void SwitchFrame::showTips()
{
    m_tipsAni->setStartValue(QPoint(m_tipsWidget->width(), 0));
    m_tipsAni->setEndValue(QPoint());
    m_tipsAni->start();
}

bool SwitchFrame::eventFilter(QObject *o, QEvent *e)
{
    if (o == m_switchSessionButton) {
        if (e->type() == QEvent::Enter)
            showTips();
        else if (e->type() == QEvent::Leave)
            hideTips();
    }

    return false;
}

void SwitchFrame::initConnect() {
    connect(m_switchUserButton, &QPushButton::clicked, this, &SwitchFrame::triggerSwitchUser);
    connect(m_powerButton, &QPushButton::clicked, this, &SwitchFrame::triggerPower);
    connect(m_switchSessionButton, &DImageButton::clicked, this, &SwitchFrame::triggerSwitchSession);
}

void SwitchFrame::initUI() {
    setFixedSize(350, 100);

    m_switchSessionButton = new DImageButton;
    m_switchSessionButton->setFixedSize(100, 100);
    m_switchSessionButton->installEventFilter(this);
    m_switchSessionButton->setObjectName("SwitchDEnvironment");
    m_switchUserButton = new QPushButton;
    m_switchUserButton->setFixedSize(100, 100);
    m_switchUserButton->setObjectName("SwitchUser");
    m_powerButton = new QPushButton;
    m_powerButton->setFixedSize(100, 100);
    m_powerButton->setObjectName("PowerMenu");
    m_tipsWidget = new QWidget;
    m_tipsWidget->setFixedSize(80, 20);
    m_tipsLabel = new QLabel(m_tipsWidget);
    m_tipsLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_tipsLabel->setStyleSheet("color:white;"
//                               "background-color:red;"
                               "font-size:12px;");
    m_tipsLabel->setFixedSize(m_tipsWidget->size());
    m_tipsLabel->move(m_tipsWidget->width(), 0);

    m_tipsAni = new QPropertyAnimation(m_tipsLabel, "pos", this);

    QGraphicsDropShadowEffect *tipsShadow = new QGraphicsDropShadowEffect(m_tipsLabel);
    tipsShadow->setColor(Qt::white);
    tipsShadow->setBlurRadius(4);
    tipsShadow->setOffset(0, 0);
    m_tipsLabel->setGraphicsEffect(tipsShadow);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(10);
    m_Layout->addStretch();
    m_Layout->addWidget(m_tipsWidget);
    m_Layout->addWidget(m_switchSessionButton);
    m_Layout->setAlignment(m_switchSessionButton, Qt::AlignBottom);
    m_Layout->addWidget(m_switchUserButton);
    m_Layout->setAlignment(m_switchUserButton, Qt::AlignBottom);
    m_Layout->addWidget(m_powerButton);
    m_Layout->setAlignment(m_powerButton, Qt::AlignBottom);
//    m_Layout->addStretch();
    setLayout(m_Layout);
//    setStyleSheet("background-color:red;");
}
