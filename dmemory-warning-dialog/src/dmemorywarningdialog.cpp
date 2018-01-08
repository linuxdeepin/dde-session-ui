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

DMemoryWarningDialog::DMemoryWarningDialog(QWidget *parent)
    : QMainWindow(parent)
{
    ProcessInfoTable *table = new ProcessInfoTable;
    table->setModel(new ProcessInfoModel);
    table->setItemDelegate(new ProcessInfoDelegate);
    table->setItemDelegateForColumn(3, new ButtonDelegate);
    table->setFixedHeight(300);

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

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(icon);
    vLayout->addWidget(label);
    vLayout->addWidget(table);
    vLayout->addStretch();

    QWidget *w = new QWidget;
    w->setLayout(vLayout);

    setCentralWidget(w);
    setFixedSize(400, 500);
    move(qApp->primaryScreen()->geometry().center() - rect().center());
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
