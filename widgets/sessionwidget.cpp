#include "sessionwidget.h"
#include "constants.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QLabel>
#include <QFile>
#include <QButtonGroup>
#include <QSettings>
#include <QPropertyAnimation>

SessionWidget::SessionWidget(QWidget *parent)
    : QFrame(parent),
      m_sessionBtns(new QList<RoundItemButton *>),
      m_sessionModel(new QLightDM::SessionsModel(this))
{
    // add sessions button
    const int count = m_sessionModel->rowCount(QModelIndex());
    for (int i(0); i != count; ++i)
    {
        const QString &session = m_sessionModel->data(m_sessionModel->index(i), Qt::DisplayRole).toString();

        RoundItemButton *sbtn = new RoundItemButton(session, this);
        sbtn->setFixedSize(160, 160);
        sbtn->hide();
        sbtn->setAutoExclusive(true);

        const QString normalIcon = QString(":/img/sessions_icon/%1_normal.png").arg(session);
        const QString hoverIcon = QString(":/img/sessions_icon/%1_hover.png").arg(session);
        const QString checkedIcon = QString(":/img/sessions_icon/%1_press.png").arg(session);

        if (QFile(normalIcon).exists() && QFile(hoverIcon).exists() && QFile(checkedIcon).exists()) {
            sbtn->setProperty("normalIcon", normalIcon);
            sbtn->setProperty("hoverIcon", hoverIcon);
            sbtn->setProperty("checkedIcon", checkedIcon);
        } else {
            sbtn->setProperty("normalIcon", ":/img/sessions_icon/unknow_normal.png");
            sbtn->setProperty("hoverIcon", ":/img/sessions_icon/unknow_hover.png");
            sbtn->setProperty("checkedIcon", ":/img/sessions_icon/unknow_press.png");
        }

        connect(sbtn, &RoundItemButton::clicked, [this, session] {switchSession(session);});

        m_sessionBtns->append(sbtn);
    }

    setFixedSize(qApp->desktop()->width(), 200);
//    setStyleSheet("background-color:red;");
}

SessionWidget::~SessionWidget()
{
    for (RoundItemButton *sbtn : *m_sessionBtns)
        delete sbtn;
    delete m_sessionBtns;
}

const QString SessionWidget::currentSessionName() const
{
    return m_sessionModel->data(m_sessionModel->index(m_currentSessionIndex), Qt::DisplayRole).toString();
}

const QString SessionWidget::currentSessionKey() const
{
    return m_sessionModel->data(m_sessionModel->index(m_currentSessionIndex), QLightDM::SessionsModel::KeyRole).toString();
}

void SessionWidget::show()
{
    if (isVisible())
        return;

    const int itemPadding = 20;
    const int itemWidth = m_sessionBtns->first()->width();
    const int itemTotal = itemPadding + itemWidth;
    const int count = m_sessionBtns->count();
    const int totalWidth = count * itemTotal - itemPadding; // sub itempadding for first item
    const int startLeft = (width() - totalWidth) / 2;

    for (int i(0); i != count; ++i) {
        QPropertyAnimation *ani = new QPropertyAnimation(m_sessionBtns->at(i), "pos");
        ani->setStartValue(QPoint(width(), 0));
        ani->setEndValue(QPoint(startLeft + i * itemTotal, 0));
        ani->start(QAbstractAnimation::DeleteWhenStopped);

        m_sessionBtns->at(i)->show();
    }

    // checked default session button
    m_sessionBtns->at(m_currentSessionIndex)->setChecked(true);

    QWidget::show();
}

void SessionWidget::switchToUser(const QString &userName)
{
    qDebug() << "session switch to user: " << userName;

    QSettings settings(DDESESSIONCC::CONFIG_FILE, QSettings::IniFormat);
    const QString &sessionName = settings.value(userName + "/last-session").toString();
    switchSession(sessionName);

    qDebug() << userName << "default session is: " << sessionName << m_currentSessionIndex;
}

void SessionWidget::saveUserLastSession(const QString &userName)
{
    qDebug() << "save user session: " << userName << currentSessionName();

    QSettings settings(DDESESSIONCC::CONFIG_FILE, QSettings::IniFormat);
    settings.setValue(userName + "/last-session", currentSessionName());
}

void SessionWidget::switchSession(const QString &sessionName)
{
    qDebug() << "switch to " << sessionName;
    m_currentSessionIndex = getSessionIndex(sessionName);
    m_sessionBtns->at(m_currentSessionIndex)->setChecked(true);

    // TODO: emit after hide animation finished
    emit sessionChanged(sessionName);
}

int SessionWidget::getSessionIndex(const QString &sessionName)
{
    const int count = m_sessionModel->rowCount(QModelIndex());
    for (int i(0); i != count; ++i)
        if (m_sessionModel->data(m_sessionModel->index(i), Qt::DisplayRole) == sessionName)
            return i;

    // if not match any item, choose first
    return 0;
}

