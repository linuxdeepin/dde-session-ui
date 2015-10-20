#include "sessionwidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QDebug>
#include <QLabel>

SessionWidget::SessionWidget(QWidget *parent)
    : QWidget(parent),
      m_sessionModel(new QLightDM::SessionsModel(this))
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    const int count = m_sessionModel->rowCount(QModelIndex());

    mainLayout->addStretch();
    for (int i(0); i != count; ++i)
    {
        mainLayout->addWidget(new QLabel(m_sessionModel->data(m_sessionModel->index(i), Qt::DisplayRole).toString()));
    }
    mainLayout->addStretch();


    setLayout(mainLayout);
    setFixedSize(qApp->desktop()->width(), 200);
}

const QString SessionWidget::currentSessionName() const
{
    // TODO:
    return m_sessionModel->data(m_sessionModel->index(0), Qt::DisplayRole).toString();
}

const QString SessionWidget::currentSessionKey() const
{
    // TODO:
    return m_sessionModel->data(m_sessionModel->index(0), QLightDM::SessionsModel::KeyRole).toString();
}

