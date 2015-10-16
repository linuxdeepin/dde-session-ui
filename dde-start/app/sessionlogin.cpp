#include <QtCore/QObject>
#include <QApplication>
#include <QtCore/QFile>
#include <QDesktopWidget>
#include <QDebug>

#include "sessionlogin.h"

SessionLogin::SessionLogin(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("SessionLoginTool");
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    initUI();
}
void SessionLogin::initUI() {
    resize(qApp->desktop()->screenGeometry().size());
    m_backgroundLabel = new BackgroundLabel(true, this);


    m_logoWidget = new LogoWidget(this);
    m_switchFrame = new SwitchFrame(this);
    m_userWidget = new UserWidget(this);
    m_logoWidget->move(0, height()-m_logoWidget->height()-20);
    m_switchFrame->move(width()-m_switchFrame->width(), height() - m_switchFrame->height());
    m_userWidget->move((qApp->desktop()->screenGeometry().width() - m_userWidget->width())/2,
                       (qApp->desktop()->screenGeometry().height() - m_userWidget->height())/2-85);

    m_userWidget->setObjectName("UserWidget");
    m_passWdEdit = new PassWdEdit("LoginIcon", this);


    m_passWdEditLayout = new QHBoxLayout;
    m_passWdEditLayout->setMargin(0);
    m_passWdEditLayout->setSpacing(0);
    m_passWdEditLayout->addStretch();
    m_passWdEditLayout->addWidget(m_passWdEdit);
    m_passWdEditLayout->addStretch();

    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addStretch();
    m_Layout->addLayout(m_passWdEditLayout);
    m_Layout->addStretch();

    setLayout(m_Layout);
    updateStyle(":/skin/login.qss", this);
    showFullScreen();

    initConnection();
}
SessionLogin::~SessionLogin()
{}
void SessionLogin::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape) {
        qApp->exit();
    }
}
void SessionLogin::Loging() {
    qDebug() << "Loging!";
}
void SessionLogin::testing(QString id) {
    qDebug() << "switch User!" << id;
    if (id == "SwitchUser") {
        m_userWidget->expandWidget();
    }
}
void SessionLogin::moveUserWidget() {


}
void SessionLogin::initConnection() {
    connect(m_passWdEdit, SIGNAL(jumpTo()), this, SLOT(Loging()));
    connect(m_switchFrame, SIGNAL(switchItem(QString)), this, SLOT(testing(QString)));
    connect(m_userWidget, SIGNAL(sizeUpdate()), this, SLOT(moveUserWidget()));
}
