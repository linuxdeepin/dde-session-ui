#include "controlwidget.h"

#include <QHBoxLayout>
#include <dimagebutton.h>
#include <QEvent>
#include <QWheelEvent>
#include <QGraphicsDropShadowEffect>

DWIDGET_USE_NAMESPACE

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    initConnect();
}

void ControlWidget::initUI()
{
    m_mediaWidget = nullptr;

    m_mainLayout = new QHBoxLayout;

    m_switchUserBtn = new DImageButton;
    m_switchUserBtn->setNormalPic(":/img/bottom_actions/userswitch_normal.png");
    m_switchUserBtn->setHoverPic(":/img/bottom_actions/userswitch_hover.png");
    m_switchUserBtn->setPressPic(":/img/bottom_actions/userswitch_press.png");

    m_powerBtn = new DImageButton;
    m_powerBtn->setNormalPic(":/img/bottom_actions/shutdown_normal.png");
    m_powerBtn->setHoverPic(":/img/bottom_actions/shutdown_hover.png");
    m_powerBtn->setPressPic(":/img/bottom_actions/shutdown_press.png");

    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(26);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_switchUserBtn);
    m_mainLayout->setAlignment(m_switchUserBtn, Qt::AlignBottom);
    m_mainLayout->addWidget(m_powerBtn);
    m_mainLayout->setAlignment(m_powerBtn, Qt::AlignBottom);
    m_mainLayout->addSpacing(28);

    setLayout(m_mainLayout);
    setFixedSize(500, 150);
}

void ControlWidget::initConnect()
{
    connect(m_switchUserBtn, &DImageButton::clicked, this, &ControlWidget::requestSwitchUser);
    connect(m_powerBtn, &DImageButton::clicked, this, &ControlWidget::requestShutdown);
}

void ControlWidget::showTips()
{
#ifndef SHENWEI_PLATFORM
    m_tipsAni->setStartValue(QPoint(m_tipWidget->width(), 0));
    m_tipsAni->setEndValue(QPoint());
    m_tipsAni->start();
#else
    m_tipsLabel->move(0, 0);
#endif
}

void ControlWidget::hideTips()
{
#ifndef SHENWEI_PLATFORM
    m_tipsAni->setEndValue(QPoint(m_tipWidget->width(), 0));
    m_tipsAni->setStartValue(QPoint());
    m_tipsAni->start();
#else
    m_tipsLabel->move(m_tipWidget->width(), 0);
#endif
}

void ControlWidget::setMPRISEnable(const bool state)
{
    if (m_mediaWidget) {
        m_mediaWidget->setVisible(state);
    } else {
        m_mediaWidget = new MediaWidget;
        m_mediaWidget->initMediaPlayer();
        m_mainLayout->insertWidget(0, m_mediaWidget);
        m_mainLayout->insertStretch(0);
    }
}

void ControlWidget::setUserSwitchEnable(const bool visible)
{
    m_switchUserBtn->setVisible(visible);
}

void ControlWidget::setSessionSwitchEnable(const bool visible)
{
    if (!m_sessionBtn && !m_tipWidget && !m_sessionTip && !m_tipsAni && !visible)
        return;

    if (!m_sessionBtn) {
        m_sessionBtn = new DImageButton;
        m_sessionBtn->setFixedSize(40, 40);
#ifndef SHENWEI_PLATFORM
        m_sessionBtn->installEventFilter(this);
#else
        m_sessionBtn->setProperty("normalIcon", ":/img/sessions/unknow_indicator_normal.png");
        m_sessionBtn->setProperty("hoverIcon", ":/img/sessions/unknow_indicator_hover.png");
        m_sessionBtn->setProperty("checkedIcon", ":/img/sessions/unknow_indicator_press.png");

#endif

        m_mainLayout->insertWidget(0, m_sessionBtn);
        m_mainLayout->setAlignment(m_sessionBtn, Qt::AlignBottom);

        connect(m_sessionBtn, &DImageButton::clicked, this, &ControlWidget::requestSwitchSession);
    }

    if (!m_tipWidget) {
        m_tipWidget = new QWidget;
        m_tipWidget->setFixedSize(80, 40);

        m_mainLayout->insertWidget(0, m_tipWidget);
        m_mainLayout->setAlignment(m_tipWidget, Qt::AlignBottom);
        m_mainLayout->insertStretch(0);
    }

    if (!m_sessionTip) {
        m_sessionTip = new QLabel(m_tipWidget);
        m_sessionTip->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_sessionTip->setFixedSize(80, 40);

#ifndef SHENWEI_PLATFORM
        m_sessionTip->setStyleSheet("color:white;"
                                    "font-size:12px;");
#else
        QPalette pe;
        pe.setColor(QPalette::WindowText,Qt::white);
        m_sessionTip->setPalette(pe);
#endif

        QGraphicsDropShadowEffect *tipsShadow = new QGraphicsDropShadowEffect(m_sessionTip);
        tipsShadow->setColor(Qt::white);
        tipsShadow->setBlurRadius(4);
        tipsShadow->setOffset(0, 0);
        m_sessionTip->setGraphicsEffect(tipsShadow);

        m_sessionTip->move(m_tipWidget->width(), 0);
    }

    if (!m_tipsAni) {
        m_tipsAni = new QPropertyAnimation(m_sessionTip, "pos", this);
    }

    m_sessionBtn->setVisible(visible);
}

void ControlWidget::chooseToSession(const QString &session)
{
    if (m_sessionBtn && m_sessionTip) {
        qDebug() << "choosed session: " << session;
        if (session.isEmpty())
            return;

        m_sessionTip->setText(session);

        const QString sessionId = session.toLower();
        const QString normalIcon = QString(":/img/sessions/%1_indicator_normal.png").arg(sessionId);
        const QString hoverIcon = QString(":/img/sessions/%1_indicator_hover.png").arg(sessionId);
        const QString checkedIcon = QString(":/img/sessions/%1_indicator_press.png").arg(sessionId);

        if (QFile(normalIcon).exists() && QFile(hoverIcon).exists() && QFile(checkedIcon).exists()) {
    #ifndef SHENWEI_PLATFORM
            m_sessionBtn->setNormalPic(normalIcon);
            m_sessionBtn->setHoverPic(hoverIcon);
            m_sessionBtn->setPressPic(checkedIcon);
    #else
            m_sessionBtn->setProperty("normalIcon", normalIcon);
            m_sessionBtn->setProperty("hoverIcon", hoverIcon);
            m_sessionBtn->setProperty("checkedIcon", checkedIcon);
    #endif
        } else {
    #ifndef SHENWEI_PLATFORM
            m_sessionBtn->setNormalPic(":/img/sessions/unknow_indicator_normal.png");
            m_sessionBtn->setHoverPic(":/img/sessions/unknow_indicator_hover.png");
            m_sessionBtn->setPressPic(":/img/sessions/unknow_indicator_press.png");
    #else
            m_sessionBtn->setProperty("normalIcon", ":/img/sessions/unknow_indicator_normal.png");
            m_sessionBtn->setProperty("hoverIcon", ":/img/sessions/unknow_indicator_hover.png");
            m_sessionBtn->setProperty("checkedIcon", ":/img/sessions/unknow_indicator_press.png");
    #endif
        }
    }
}

bool ControlWidget::eventFilter(QObject *watched, QEvent *event)
{
#ifndef SHENWEI_PLATFORM
    if (watched == m_sessionBtn) {
        if (event->type() == QEvent::Enter)
            showTips();
        else if (event->type() == QEvent::Leave)
            hideTips();
    }
#else
    Q_UNUSED(watched);
    Q_UNUSED(event);
#endif
    return false;
}
