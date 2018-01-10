#include "buttondelegate.h"
#include "processinfomodel.h"

#include <QApplication>
#include <QProcess>
#include <QDebug>
#include <QPainter>

#include <ddialog.h>

DWIDGET_USE_NAMESPACE

bool confirm(const QPixmap &icon)
{
    const QStringList btns = QStringList() << QApplication::translate("ButtonDelegate", "Cancel")
//                                           << QApplication::translate("ButtonDelegate", "View")
                                           << QApplication::translate("ButtonDelegate", "Free");

    DDialog terminateDialog(nullptr);
    terminateDialog.setMessage(QApplication::translate("ButtonDelegate", "Are you sure to terminate this process? this operate can not restore."));
    terminateDialog.setIconPixmap(icon);
    terminateDialog.addButtons(btns);

    QObject::connect(&terminateDialog, &DDialog::buttonClicked, [&](const int index) {
        if (index == 1)
            terminateDialog.accept();
    });

    if (terminateDialog.exec() == DDialog::Accepted)
        return true;
    return false;
}

void terminate(const QStringList &pidList, const QPixmap &icon)
{
    if (confirm(icon))
        for (const auto &pid : pidList)
            QProcess::startDetached("kill", QStringList() << pid);
}

void close_tab(const int id)
{
    if (!confirm(QIcon::fromTheme("google-chrome").pixmap(32, 32)))
        return;

    QProcess::startDetached("qdbus com.deepin.chromeExtension.TabsLimit /com/deepin/chromeExtension/TabsLimit com.deepin.chromeExtension.TabsLimit.CloseTab " + QString::number(id));
}

ButtonDelegate::ButtonDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.row() % 2)
        painter->fillRect(option.rect, QColor(0, 0, 0, 255 * 0.05));

    QStyleOptionButton button;
    button.rect = option.rect.marginsRemoved(QMargins(6, 6, 6, 6));
    button.text = index.data().toString();
    button.state = QStyle::State_Enabled;
    if (index.data(ProcessInfoModel::StateRole).toBool())
        button.state |= QStyle::State_Sunken | QStyle::State_Active;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
}

bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(option);

    switch (event->type())
    {
    case QEvent::MouseButtonPress:
        model->setData(index, QVariant(), ProcessInfoModel::StateRole);
        break;
    case QEvent::MouseButtonRelease:
    {
        const auto &pidList = index.data(ProcessInfoModel::PidListRole).value<QStringList>();
        if (!pidList.isEmpty())
            terminate(pidList, index.data(ProcessInfoModel::IconRole).value<QPixmap>());
        else
            close_tab(index.data(ProcessInfoModel::TabIdRole).toInt());
        break;
    }
    default:;
    }

    return true;
}
