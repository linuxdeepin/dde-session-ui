#include "sessionbutton.h"

#include <QVBoxLayout>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

SessionButton::SessionButton(QWidget *parent)
    : QWidget(parent),
      m_showAnimation(new QPropertyAnimation(this, "pos")),
      m_hideAnimation(new QPropertyAnimation(this, "pos"))
{
    m_sessionIcon = new DUI::DImageButton(this);
    m_sessionName = new QLabel(this);
    m_sessionName->setAlignment(Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_sessionIcon);
    mainLayout->setAlignment(m_sessionIcon, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(m_sessionName);
    mainLayout->setAlignment(m_sessionName, Qt::AlignHCenter);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    setLayout(mainLayout);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void SessionButton::setName(const QString &name)
{
    qDebug() << name;
    m_sessionName->setText(name);

    // set icon
    QString iconNormalPath = QString(":/sessions/img/sessions/%1_normal.png").arg(name);
    QString iconHoverPath = QString(":/sessions/img/sessions/%1.png").arg(name);

    if (!QFile(iconNormalPath).exists())
        iconNormalPath = ":/sessions/img/sessions/unkown.png";
    if (!QFile(iconHoverPath).exists())
        iconHoverPath = ":/sessions/img/sessions/unkown_normal.png";

    m_sessionIcon->setNormalPic(iconNormalPath);
    m_sessionIcon->setHoverPic(iconHoverPath);
}

void SessionButton::show(const QPoint &position)
{
    if (isVisible() ||
        m_showAnimation->state() == QPropertyAnimation::Running ||
        m_hideAnimation->state() == QPropertyAnimation::Running)
        return;

    QWidget::show();

    m_showAnimation->setStartValue(QPoint(qApp->desktop()->width(), position.y()));
    m_showAnimation->setEndValue(position);
    m_showAnimation->start();
}
