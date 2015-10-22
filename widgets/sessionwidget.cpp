#include "sessionwidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QLabel>
#include <QFile>
#include <QButtonGroup>

SessionWidget::SessionWidget(QWidget *parent)
    : QFrame(parent),
      m_sessionBtns(new QList<RoundItemButton *>),
      m_sessionModel(new QLightDM::SessionsModel(this))
{
    QButtonGroup *g = new QButtonGroup(this);
    g->setExclusive(true);

    // add sessions button
    const int count = m_sessionModel->rowCount(QModelIndex());
    const QString &currentSession = currentSessionName();
    for (int i(0); i != count; ++i)
    {
        const QString &session = m_sessionModel->data(m_sessionModel->index(i), Qt::DisplayRole).toString();

        RoundItemButton *sbtn = new RoundItemButton(session, this);
        sbtn->hide();
        sbtn->setAutoExclusive(true);
        if (session == currentSession)
            sbtn->setChecked(true);
        g->addButton(sbtn);

        const QString normalIcon = QString(":/session/img/sessions/%1_normal.png").arg(session);
        const QString hoverIcon = QString(":/session/img/sessions/%1_hover.png").arg(session);
        const QString checkedIcon = QString(":/session/img/sessions/%1_press.png").arg(session);

        if (QFile(normalIcon).exists()) {
            sbtn->setProperty("normalIcon", normalIcon);
            sbtn->setProperty("hoverIcon", hoverIcon);
            sbtn->setProperty("checkedIcon", checkedIcon);
        } else {
            sbtn->setProperty("normalIcon", ":/session/img/sessions/unknow_normal.png");
            sbtn->setProperty("hoverIcon", ":/session/img/sessions/unknow_hover.png");
            sbtn->setProperty("checkedIcon", ":/session/img/sessions/unknow_press.png");
        }

        connect(sbtn, &RoundItemButton::clicked, [this, session] {switchSession(session);});

        m_sessionBtns->append(sbtn);
    }

    setFixedSize(qApp->desktop()->width(), 150);
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
    // TODO: index
    return m_sessionModel->data(m_sessionModel->index(0), Qt::DisplayRole).toString();
}

const QString SessionWidget::currentSessionKey() const
{
    // TODO: index
    return m_sessionModel->data(m_sessionModel->index(0), QLightDM::SessionsModel::KeyRole).toString();
}

void SessionWidget::show()
{
    const int itemPadding = 20;
    const int itemWidth = m_sessionBtns->first()->width();
    const int itemTotal = itemPadding + itemWidth;
    const int count = m_sessionBtns->count();
    const int totalWidth = count * itemTotal - itemPadding; // sub itempadding for first item
    const int startLeft = (width() - totalWidth) / 2;

    for (int i(0); i != count; ++i) {
        // TODO: animation
        m_sessionBtns->at(i)->move(startLeft + i * itemTotal, 0);
        m_sessionBtns->at(i)->show();
    }
//        m_sessionBtns->at(i)->show(QPoint(i * 500, 0));
    QWidget::show();
}

void SessionWidget::switchSession(const QString &sessionName)
{
    qDebug() << "switch to " << sessionName;
    emit sessionChanged(sessionName);
}

