#include "updatecontent.h"
#include <QHBoxLayout>
#include <QEvent>
#include <QLabel>
#include <QApplication>

UpdateContent::UpdateContent(const QString &version, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    QLabel *successIcon = new QLabel;
    successIcon->setPixmap(QPixmap(":/resources/success.svg"));

    QLabel *successTip = new QLabel(tr("Welcome, system updated successfully"));
    QLabel *currentVersion = new QLabel(tr("Current Edition:") + " " + version);
    m_enterBtn = new QPushButton(tr("Enter"), this);
    m_enterBtn->setMinimumWidth(200);

    mainLayout->addStretch();
    mainLayout->addWidget(successIcon, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(successTip, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(25);
    mainLayout->addWidget(currentVersion, 0, Qt::AlignHCenter);
    mainLayout->addStretch();

    m_enterBtn->setStyleSheet("QPushButton {"
                              "background: rgba(255, 255, 255, 0.2);"
                              "border-radius:5px;"
                              "outline: none;"
                              "color: white;"
                              "}"
                              "QPushButton:hover {"
                              "background: rgba(255, 255, 255, 0.3);"
                              "border-radius:5px;"
                              "outline: none;"
                              "color: white;"
                              "}"
                              "QPushButton:pressed {"
                              "background: rgba(255, 255, 255, 0.1);"
                              "border-radius:5px;"
                              "outline: none;"
                              "color: #2ca7f8;"
                              "}");

    successTip->setStyleSheet("font-size: 36px;"
                              "color: white;"
                              "font-weight: 50;");

    currentVersion->setStyleSheet("font-size: 20px;"
                                  "color: white;"
                                  "font-weight: 40;");

    setLayout(mainLayout);

    connect(m_enterBtn, &QPushButton::clicked, qApp, &QApplication::exit);
}

void UpdateContent::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    const QRect &r = rect();
    m_enterBtn->move((r.width() - m_enterBtn->width()) / 2, r.height() - 180);
}

