#include "content.h"

#include <QScrollArea>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>

Content::Content(QWidget *parent)
    : QWidget(parent)
    , m_scrollArea(new QScrollArea)
    , m_acceptCheck(new QCheckBox)
    , m_cancelBtn(new QPushButton)
    , m_acceptBtn(new DSuggestButton)
    , m_source(new QLabel)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);

    m_cancelBtn->setText(tr("Cancel"));
    m_acceptBtn->setText(tr("Confirm"));

    m_scrollArea->setFixedSize(468, 491);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    m_scrollArea->setContentsMargins(0, 0, 0, 0);

    QWidget* sourceWidget = new QWidget;
    sourceWidget->setFixedWidth(468);
    QVBoxLayout* sourceLayout = new QVBoxLayout;
    sourceWidget->setLayout(sourceLayout);
    sourceLayout->addWidget(m_source);
    sourceLayout->addStretch();
    m_source->setWordWrap(true);

    m_scrollArea->setWidget(sourceWidget);

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->setMargin(0);
    bottomLayout->setSpacing(0);
    bottomLayout->addSpacing(10);
    bottomLayout->addWidget(m_acceptCheck, 0, Qt::AlignVCenter);
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_cancelBtn, 0, Qt::AlignVCenter);
    bottomLayout->addSpacing(10);
    bottomLayout->addWidget(m_acceptBtn, 0, Qt::AlignVCenter);
    bottomLayout->addSpacing(10);

    m_cancelBtn->setFixedSize(80, 36);
    m_acceptBtn->setFixedSize(80, 36);

    QWidget* bottom = new QWidget;
    bottom->setFixedHeight(65);
    bottom->setLayout(bottomLayout);

    layout->addWidget(m_scrollArea);
    layout->addWidget(bottom);

    QWidget* lineWidget = new QWidget(this);
    lineWidget->setObjectName("LineWidget");
    lineWidget->setFixedWidth(520);
    lineWidget->setFixedHeight(2);
    lineWidget->move(0, 490);

    setStyleSheet("#LineWidget {"
        "border: 1px solid rgba(0, 0, 0, 0.1);"
        "}");

    m_acceptBtn->setDisabled(true);

    connect(m_cancelBtn, &QPushButton::clicked, this, [=] {
        qApp->exit(-1);
    });

    connect(m_acceptBtn, &QPushButton::clicked, this, [=] {
        qApp->exit(96);
    });

    connect(m_acceptCheck, &QCheckBox::toggled, m_acceptBtn, &QPushButton::setEnabled);
}

void Content::setSource(const QString &source)
{
    m_source->setText(source);
}

void Content::setAllowCheckBoxText(const QString &text)
{
    m_acceptCheck->setText(text);
    m_acceptCheck->setVisible(!text.isEmpty());
    m_acceptBtn->setEnabled(text.isEmpty());
}
