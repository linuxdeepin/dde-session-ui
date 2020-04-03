#include "content.h"
#include "sourcelabel.h"

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
#include <QTimer>

DWIDGET_USE_NAMESPACE

Content::Content(QWidget *parent)
    : QWidget(parent)
    , m_scrollArea(new QScrollArea)
    , m_acceptCheck(new QCheckBox)
    , m_cancelBtn(new QPushButton)
    , m_acceptBtn(new DSuggestButton)
    , m_source(new SourceLabel)
    , m_languageBtn(new DButtonBox)
    , m_hasCn(false)
    , m_hasEn(false)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);

    DButtonBoxButton *btnChinese = new DButtonBoxButton("中文");
    DButtonBoxButton *btnEnginsh = new DButtonBoxButton("English");
    QList<DButtonBoxButton *> btnlist;
    btnlist.append(btnChinese);
    btnlist.append(btnEnginsh);
    m_languageBtn->setButtonList(btnlist, true);
//    btnChinese->setChecked(true);//FIX:未显示之前setChecked无效
    m_languageBtn->setId(btnChinese, 1);
    m_languageBtn->setId(btnEnginsh, 0);

    layout->addWidget(m_languageBtn, 0, Qt::AlignHCenter);

    m_cancelBtn->setText(tr("Cancel"));
    m_acceptBtn->setText(tr("Confirm"));

    m_scrollArea->setMinimumSize(468,300);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    m_scrollArea->setContentsMargins(0, 0, 0, 0);

    QWidget* sourceWidget = new QWidget;
    QVBoxLayout* sourceLayout = new QVBoxLayout;
    sourceWidget->setLayout(sourceLayout);
    sourceLayout->addWidget(m_source);
    // 左右边距20
    m_source->setContentsMargins(20, 0, 20, 0);

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

    m_acceptBtn->setDisabled(true);

    connect(m_cancelBtn, &QPushButton::clicked, this, [=] {
        qApp->exit(-1);
    });

    connect(m_acceptBtn, &QPushButton::clicked, this, [=] {
        qApp->exit(96);
    });

    connect(m_acceptCheck, &QCheckBox::toggled, m_acceptBtn, &QPushButton::setEnabled);

    connect(m_source,&SourceLabel::loadFinished,this,&Content::loadFinished);
    connect(m_languageBtn, &DButtonBox::buttonClicked, [this](QAbstractButton *value) {
        m_isCn = m_languageBtn->id(value);
        updateContent();
    });

    m_isCn = QLocale::system().language() == QLocale::Chinese;
    updateContent();
    updateLanguageBtn();

    connect(m_source,&SourceLabel::loadFinished,this,[=]{
        // 中文和英文按钮一样大
        QTimer::singleShot(0,this,[=]{
            int width = qMax(btnChinese->width(),btnEnginsh->width());
            btnChinese->resize(width,btnChinese->height());
            btnEnginsh->resize(width,btnChinese->height());
            m_languageBtn->resize(width*2,m_languageBtn->height());
        });
    });
}

void Content::setSource(const QString &source)
{
    if (source.isEmpty())
        return;

    QFile file(source);
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        m_source->setText(file.readAll());
        file.close();

        updateWindowHeight();
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
    setSource(source);
    updateLanguageBtn();
    m_languageBtn->button(1)->setChecked(true);
}

void Content::setEnSource(const QString &source)
{
    m_en = source;
    setSource(source);
    updateLanguageBtn();
    m_languageBtn->button(0)->setChecked(true);
}

// 中英文都存在,选中本地语言
void Content::updateLocaleSource()
{
    if (!m_hasCn || !m_hasEn)
        return;

    if (QLocale::system().language() == QLocale::Chinese) {
        m_isCn = true;
        m_languageBtn->button(1)->setChecked(true);
    } else {
        m_isCn = false;
        m_languageBtn->button(0)->setChecked(true);
    }

    updateContent();
}

void Content::updateLanguageBtn()
{
    m_hasCn = QFile::exists(m_cn);
    m_hasEn = QFile::exists(m_en);
    m_languageBtn->setVisible(m_hasCn && m_hasEn);
}

void Content::updateContent()
{
    if (m_isCn) {
        setSource(m_cn);
    } else {
        setSource(m_en);
    }
}

// 根据文本多少，调整窗口大小
void Content::updateWindowHeight()
{
    QRect rc = m_source->rect();
    rc.setWidth(rc.width() - 40);
    QSize sz = m_source->fontMetrics().boundingRect(rc, Qt::TextWordWrap, m_source->text()).size();

    int minHeight = qBound(300, sz.height(), 491);
    m_scrollArea->setMinimumHeight(minHeight);
}
