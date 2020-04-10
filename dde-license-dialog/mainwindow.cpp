#include "mainwindow.h"
#include "content.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>

#include <DFontSizeManager>
DTK_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : DAbstractDialog(false, parent)
    , m_title(new QLabel)
    , m_content(new Content)
{
    auto envType = qEnvironmentVariable("XDG_SESSION_TYPE");
    bool bWayland = envType.contains("wayland");
    if (bWayland) {
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }

    m_title->setObjectName("TitleLabel");
    QWidget *widget = new QWidget(this);
    widget->setFixedSize(500, 40);

    btnclose = new DIconButton(QStyle::SP_TitleBarCloseButton, this);
    btnclose->setVisible(false);
    btnclose->setFlat(true);
    btnclose->setIconSize(QSize(40, 40));
    btnclose->setFocusPolicy(Qt::NoFocus);

    QPalette pa = m_title->palette();
    pa.setColor(QPalette::Button,pa.background().color());
    btnclose->setPalette(pa);

    QHBoxLayout *titlelayout = new QHBoxLayout;
    titlelayout->setMargin(0);
    titlelayout->addWidget(m_title);
    titlelayout->addWidget(btnclose, Qt::AlignRight);
    widget->setLayout(titlelayout);
    //m_title->setAlignment(Qt::AlignCenter);
    m_title->setStyleSheet(QString("qproperty-alignment: 'AlignBottom | AlignCenter'"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    //layout->addSpacing(10);
    layout->addWidget(widget, 0, Qt::AlignHCenter);
    layout->addWidget(m_content);

    setLayout(layout);
    setFixedWidth(500);

    connect(m_content, &Content::sourceChanged, this, [ = ](bool isCn) {
        if (isCn) {
            m_title->setText(m_cnTitle);
        } else {
            m_title->setText(m_enTitle);
        }
    });

    connect(m_content, &Content::sourceChanged, this, [ = ](bool isCn) {
        if (isCn) {
            m_title->setText(m_cnTitle);
        } else {
            m_title->setText(m_enTitle);
        }
    });

    connect(m_content, &Content::sourceChanged, this, [ = ](bool isCn) {
        if (isCn) {
            m_title->setText(m_cnTitle);
        } else {
            m_title->setText(m_enTitle);
        }
    });

    DFontSizeManager::instance()->bind(m_title, DFontSizeManager::SizeType::T5, 70);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setTitle(const QString &title)
{
    m_cnTitle = title;
}

void MainWindow::setEnTitle(const QString &title)
{
    m_enTitle = title;
}

void MainWindow::setCnSource(const QString &source)
{
    m_content->setCnSource(source);
}

void MainWindow::setEnSource(const QString &source)
{
    m_content->setEnSource(source);
}

void MainWindow::updateLocaleSource()
{
    m_content->updateLocaleSource();
}

void MainWindow::setAllowCheckBoxText(const QString &text)
{
    m_content->setAllowCheckBoxText(text);
}

void MainWindow::setEnAllowCheckBoxText(const QString &text)
{
    m_content->setEnAllowCheckBoxText(text);
}

void MainWindow::setHideBottom(const QString& param)
{
    bool status = false;
    if(param == QStringLiteral("yes"))
    {
        status = true;
        btnclose->setVisible(true);
        connect(btnclose, &DIconButton::clicked, this, [ = ] {
            qApp->exit(0);
        });
    }
    m_content->setHideBottom(status);
}

