#include "mainwindow.h"
#include "content.h"

#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : DAbstractDialog(parent)
    , m_title(new QLabel)
    , m_content(new Content)
{
    m_title->setObjectName("TitleLabel");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addSpacing(8);
    layout->addWidget(m_title, 0, Qt::AlignHCenter);
    layout->addWidget(m_content, 0, Qt::AlignHCenter);

    setLayout(layout);

    m_title->setStyleSheet("#TitleLabel {"
        "font-size: 12px;"
        "font-weight: bold;"
        "color: #303030;"
        "}");
}

MainWindow::~MainWindow()
{

}

void MainWindow::setTitle(const QString &title)
{
    m_title->setText(title);
}

void MainWindow::setSource(const QString &source)
{
    m_content->setSource(source);
}

void MainWindow::setAllowCheckBoxText(const QString &text)
{
    m_content->setAllowCheckBoxText(text);
}
