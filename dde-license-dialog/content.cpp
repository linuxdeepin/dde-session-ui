#include "content.h"

#include <DSuggestButton>
#include <DCommandLinkButton>
#include <DFontSizeManager>

#include <QScrollArea>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QStandardPaths>
DWIDGET_USE_NAMESPACE

Content::Content(QWidget *parent)
    : QWidget(parent)
    , m_scrollArea(new QScrollArea)
    , m_acceptCheck(new QCheckBox)
    , m_cancelBtn(new QPushButton)
    , m_acceptBtn(new DSuggestButton)
    , m_source(new QLabel)
    , m_languageBtn(new DButtonBox)
    , m_isCn(false)
    , m_hasCn(false)
    , m_hasEn(false)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);

    DButtonBoxButton *btnChinese = new DButtonBoxButton("中文");
    DButtonBoxButton *btnEnginsh = new DButtonBoxButton("English");
    QList<DButtonBoxButton *> btnlist;
    btnlist.append(btnChinese);
    btnlist.append(btnEnginsh);
    m_languageBtn->setButtonList(btnlist, true);
    // btnChinese->setChecked(true);//FIX:未显示之前setChecked无效
    m_languageBtn->setId(btnChinese, 1);
    m_languageBtn->setId(btnEnginsh, 0);

    layout->addWidget(m_languageBtn, 0, Qt::AlignHCenter);

    m_scrollArea->setMinimumSize(468, 300);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    m_scrollArea->setContentsMargins(0, 0, 0, 0);

    QWidget *sourceWidget = new QWidget(this);
    sourceWidget->setFixedWidth(468);
    QVBoxLayout *sourceLayout = new QVBoxLayout;
    sourceWidget->setLayout(sourceLayout);
    sourceLayout->addWidget(m_source);
    // sourceLayout->addStretch();//FIX:英文协议内容过长，会导致label高度显示不正确，未找到原因，去掉这一行正常
    m_source->setTextFormat(Qt::RichText);
    m_source->setWordWrap(true);
    m_source->setOpenExternalLinks(true);
    // 左右边距20
    m_source->setContentsMargins(20, 0, 20, 0);

    m_scrollArea->setWidget(sourceWidget);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
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

    m_bottom = new QWidget(this);
    m_bottom->setFixedHeight(65);
    m_bottom->setLayout(bottomLayout);

    layout->addWidget(m_scrollArea);
    layout->addSpacing(20);
    layout->addWidget(m_bottom);

    m_acceptBtn->setDisabled(true);

    connect(m_cancelBtn, &QPushButton::clicked, this, [ = ] {
        qApp->exit(-1);
    });

    connect(m_acceptBtn, &QPushButton::clicked, this, [ = ] {
        qApp->exit(96);
    });

    connect(m_acceptCheck, &QCheckBox::toggled, m_acceptBtn, &QPushButton::setEnabled);

    connect(m_languageBtn, &DButtonBox::buttonClicked, [this](QAbstractButton * value) {
        m_isCn = m_languageBtn->id(value);
        updateContent();
    });

    m_isCn = QLocale::system().language() == QLocale::Chinese;
    updateContent();
    updateLanguageBtn();

    DFontSizeManager::instance()->bind(m_acceptCheck, DFontSizeManager::SizeType::T8, 70);

    // 中文和英文按钮一样大
    QTimer::singleShot(0, this, [ = ] {
        int width = qMax(btnChinese->width(), btnEnginsh->width());
        m_languageBtn->setFixedSize(width * 2, m_languageBtn->height());
        btnChinese->setFixedSize(width, btnChinese->height());
        btnEnginsh->setFixedSize(width, btnChinese->height());
    });
}

void Content::setHideBottom(const bool &status)
{
    m_bottom->setVisible(!status);
}

void Content::setSource(const QString &source)
{
    if (source.isEmpty())
        return;
    // pandoc将md转换成html
    static QMap<QString, QString> sourceMap;

    if (sourceMap[source].isEmpty()) {
        QProcess process;
        QString para;
        QString tempPath=QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first();
        tempPath.append("/license_temp.html");

        if (QFile::exists(tempPath))
            QFile::remove(tempPath);

        para = QString("pandoc %1 --output %2").arg(source).arg(tempPath);
        QStringList args;
        args << "-c";
        args << para;
        process.start("sh", args);
        process.waitForFinished();
        process.waitForReadyRead();
        QFile file(tempPath);
        if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
            qDebug() << file.errorString();
            return;
        }
        sourceMap.insert(source, file.readAll());
        file.close();
    }

    m_source->setText(sourceMap[source]);

    updateWindowHeight();
}

void Content::setAllowCheckBoxText(const QString &text)
{
    m_allow = text;
}

void Content::setEnAllowCheckBoxText(const QString &text)
{
    m_enallow = text;
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
    if (!m_hasCn && !m_hasEn)
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
        m_acceptCheck->setText(m_allow);
        m_acceptCheck->setVisible(!m_allow.isEmpty());
        m_cancelBtn->setText(tr("Cancel"));
        m_acceptBtn->setText(tr("Confirm"));
        m_acceptBtn->setEnabled(m_allow.isEmpty() || (!m_allow.isEmpty() && m_acceptCheck->isChecked()));
    } else {
        setSource(m_en);
        m_acceptCheck->setText(m_enallow);
        m_acceptCheck->setVisible(!m_enallow.isEmpty());
        m_cancelBtn->setText("Cancel");
        m_acceptBtn->setText("Confirm");
        m_acceptBtn->setEnabled(m_allow.isEmpty() || (!m_allow.isEmpty() && m_acceptCheck->isChecked()));
    }
    Q_EMIT sourceChanged(m_isCn);
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
