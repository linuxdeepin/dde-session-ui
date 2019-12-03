#include "content.h"

#include <DSuggestButton>
#include <DCommandLinkButton>

#include <QScrollArea>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QFile>

DWIDGET_USE_NAMESPACE

Content::Content(QWidget *parent)
    : QWidget(parent)
    , m_scrollArea(new QScrollArea)
    , m_acceptCheck(new QCheckBox)
    , m_cancelBtn(new QPushButton)
    , m_acceptBtn(new DSuggestButton)
    , m_source(new QLabel)
    , m_languageBtn(new DCommandLinkButton(""))
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);
    layout->addWidget(m_languageBtn, 0, Qt::AlignHCenter);

    m_cancelBtn->setText(tr("Cancel"));
    m_acceptBtn->setText(tr("Confirm"));

    m_scrollArea->setMinimumSize(468,491);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
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

//    QWidget* lineWidget = new QWidget(this);
//    lineWidget->setObjectName("LineWidget");
//    lineWidget->setFixedWidth(520);
//    lineWidget->setFixedHeight(2);
//    lineWidget->move(0, 490);

//    setStyleSheet("#LineWidget {"
//        "border: 1px solid rgba(0, 0, 0, 0.1);"
//        "}");

    m_acceptBtn->setDisabled(true);

    connect(m_cancelBtn, &QPushButton::clicked, this, [=] {
        qApp->exit(-1);
    });

    connect(m_acceptBtn, &QPushButton::clicked, this, [=] {
        qApp->exit(96);
    });

    connect(m_acceptCheck, &QCheckBox::toggled, m_acceptBtn, &QPushButton::setEnabled);

    connect(m_languageBtn, &QPushButton::clicked, this, [=] {
        m_isCn = !m_isCn;
        updateContent();
    });

    m_isCn = QLocale::system().language() == QLocale::Chinese;
    updateContent();
    updateLanguageBtn();
}

void Content::setSource(const QString &source)
{
    if (source.isEmpty())
        return;

    QFile file(source);
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        m_source->setText(file.readAll());
        file.close();
    }
}

void Content::setAllowCheckBoxText(const QString &text)
{
    m_acceptCheck->setText(text);
    m_acceptCheck->setVisible(!text.isEmpty());
    m_acceptBtn->setEnabled(text.isEmpty());
}

void Content::setCnSource(const QString &source)
{
    m_cn = source;
    if (m_isCn)
        setSource(source);
    updateLanguageBtn();
}

void Content::setEnSource(const QString &source)
{
    m_en = source;
    if (!m_isCn)
        setSource(source);
    updateLanguageBtn();
}

void Content::updateLanguageBtn()
{
    m_languageBtn->setVisible(QFile::exists(m_cn) && QFile::exists(m_en));
}

void Content::updateContent()
{
    if (m_isCn) {
        setSource(m_cn);
        m_languageBtn->setText(tr("View in English"));
    } else {
        setSource(m_en);
        m_languageBtn->setText(tr("View in Chinese"));
    }
}
