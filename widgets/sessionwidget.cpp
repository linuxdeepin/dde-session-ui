#include "sessionwidget.h"
#include "util_updateui.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QLabel>

SessionWidget::SessionWidget(QWidget *parent)
    : QFrame(parent),
      m_sessionBtns(new QList<RoundItemButton *>),
      m_sessionModel(new QLightDM::SessionsModel(this))
{
    const int count = m_sessionModel->rowCount(QModelIndex());

    for (int i(0); i != count; ++i)
    {
        const QString &session = m_sessionModel->data(m_sessionModel->index(i), Qt::DisplayRole).toString();

        RoundItemButton *sbtn = new RoundItemButton(session, "SessionButton", this);
        sbtn->hide();
        sbtn->setCheckable(true);
        sbtn->setObjectName("SessionFrame");

        connect(sbtn, &RoundItemButton::buttonAction, this, &SessionWidget::switchSession);

        m_sessionBtns->append(sbtn);
    }

    setFixedSize(qApp->desktop()->width(), 150);
    updateStyle(":/skin/sessions.qss", this);
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
    qDebug() << "switch to session: " << sessionName;
}

