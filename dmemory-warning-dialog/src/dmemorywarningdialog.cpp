#include "dmemorywarningdialog.h"
#include "processinfotable.h"
#include "processinfomodel.h"
#include "processinfodelegate.h"
#include "buttondelegate.h"

#include <QKeyEvent>
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

#include <DTitlebar>

DMemoryWarningDialog::DMemoryWarningDialog(QWidget *parent)
    : DMainWindow(parent)

    , m_infoModel(new ProcessInfoModel)

    , m_icon(new QLabel)
    , m_appName(new QLabel)
    , m_tips(new QLabel)

    , m_cancelButton(new QPushButton)
    , m_continueButton(new QPushButton)
{
    titlebar()->setTitle(QString());

    m_cancelButton->setText(tr("Cancel"));
    m_continueButton->setText(tr("Continue"));
    m_icon->setPixmap(QIcon::fromTheme("dde").pixmap(32, 32));
    m_appName->setText("appname");
    m_tips->setText("need 100M memory");

    ProcessInfoTable *table = new ProcessInfoTable;
    table->setModel(m_infoModel);
    table->setItemDelegate(new ProcessInfoDelegate);
    table->setItemDelegateForColumn(3, new ButtonDelegate);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(m_cancelButton);
    btnsLayout->addWidget(m_continueButton);

    QLabel *icon = new QLabel;
    icon->setAlignment(Qt::AlignCenter);
    icon->setPixmap(QIcon::fromTheme("messagebox_warning").pixmap(64, 64));

    QLabel *label = new QLabel;
    label->setText(tr("Please free some memory to start new process"));
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    QFont f = label->font();
    f.setPointSize(12);
    label->setFont(f);

    QHBoxLayout *tipsLayout = new QHBoxLayout;
    tipsLayout->addStretch();
    tipsLayout->addWidget(m_icon);
    tipsLayout->addWidget(m_appName);
    tipsLayout->addWidget(m_tips);
    tipsLayout->addStretch();

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(icon);
    vLayout->addWidget(label);
    vLayout->addWidget(table);
    vLayout->addSpacing(20);
    vLayout->addLayout(tipsLayout);
    vLayout->addSpacing(20);
    vLayout->addLayout(btnsLayout);

    QWidget *w = new QWidget;
    w->setLayout(vLayout);

    setCentralWidget(w);
    setFixedSize(400, 600);
    move(qApp->primaryScreen()->geometry().center() - rect().center());
}

void DMemoryWarningDialog::updateAppInfo(const QString &appInfo)
{
    qDebug() << appInfo;
}

void DMemoryWarningDialog::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef QT_DEBUG
    case Qt::Key_Escape:    qApp->quit();   break;
#endif
    default:;
    }
}

void DMemoryWarningDialog::showEvent(QShowEvent *e)
{
    DMainWindow::showEvent(e);

    m_infoModel->startRefreshData();
}

void DMemoryWarningDialog::hideEvent(QHideEvent *e)
{
    DMainWindow::hideEvent(e);

    m_infoModel->stopRefreshData();
}
