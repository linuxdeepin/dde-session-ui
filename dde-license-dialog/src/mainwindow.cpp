// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindow.h"
#include "content.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>
#include <QIcon>

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

    setAccessibleName("MainWindow");
    m_title->setObjectName("TitleLabel");
    m_title->setAccessibleName("TitleLabel");
    QWidget *widget = new QWidget(this);
    widget->setAccessibleName("MainWidget");
    widget->setFixedSize(500, 40);

    btnclose = new DIconButton(QStyle::SP_TitleBarCloseButton, this);
    btnclose->setAccessibleName("CloseBtn");
    btnclose->setVisible(false);
    btnclose->setFlat(true);
    btnclose->setIconSize(QSize(40, 40));
    btnclose->setFocusPolicy(Qt::NoFocus);

    QPalette pa = m_title->palette();
    pa.setColor(QPalette::Button,pa.window().color());
    btnclose->setPalette(pa);

    QHBoxLayout *titlelayout = new QHBoxLayout;
    titlelayout->setContentsMargins(0, 0, 0, 0);
    titlelayout->addWidget(m_title);
    widget->setLayout(titlelayout);
    //m_title->setAlignment(Qt::AlignCenter);
    m_title->setStyleSheet(QString("qproperty-alignment: 'AlignBottom | AlignCenter'"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    //layout->addSpacing(10);
    layout->addWidget(widget, 0, Qt::AlignHCenter);
    layout->addWidget(m_content);
    m_content->setAccessibleName("MainContent");

    setLayout(layout);
    setFixedWidth(windowFixedWidth);

    m_leftIconBtn = new DIconButton(this);
    m_leftIconBtn->setAccessibleName("TopLeftIconBtn");
    m_leftIconBtn->setFlat(true);
    m_leftIconBtn->setFocusPolicy(Qt::NoFocus);
    m_leftIconBtn->setFixedSize(30, 30);
    m_leftIconBtn->setIconSize(QSize(30, 30));
    m_leftIconBtn->move(10, 10);
    m_leftIconBtn->setVisible(false);
    m_leftIconBtn->raise();

    connect(m_content, &Content::sourceChanged, this, [ = ](bool isCn) {
        if (isCn) {
            m_title->setText(m_cnTitle);
        } else {
            m_title->setText(m_enTitle);
        }

         int newWidth = m_content->calWidgetWidth();
        // 根据计算后的宽度调整主窗口
        if (newWidth > windowFixedWidth)
            setFixedWidth(newWidth);
    });

    DFontSizeManager::instance()->bind(m_title, DFontSizeManager::SizeType::T5, 70);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    DAbstractDialog::resizeEvent(event);
    if (btnclose) {
        const QSize sz = btnclose->sizeHint();
        btnclose->move(width() - sz.width(), 0);
        btnclose->raise();
    }
    if (m_leftIconBtn) {
        m_leftIconBtn->raise();
    }
}

void MainWindow::setTitlebarIcon(const QString &iconName)
{
    if (!iconName.isEmpty()) {
        m_leftIconBtn->setIcon(QIcon::fromTheme(iconName));
        m_leftIconBtn->setVisible(true);
        btnclose->setVisible(true);
        connect(btnclose, &DIconButton::clicked, this, [ = ] {
            qApp->exit(-1);
        });
    }
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
