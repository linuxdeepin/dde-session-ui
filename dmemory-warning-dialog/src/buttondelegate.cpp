#include "buttondelegate.h"
#include "processinfomodel.h"

#include <QApplication>
#include <QProcess>
#include <QDebug>

#include <ddialog.h>

DWIDGET_USE_NAMESPACE

void terminate(const QStringList &pidList, const QPixmap &icon)
{
    const QStringList btns = QStringList() << QApplication::translate("ButtonDelegate", "Cancel")
                                           << QApplication::translate("ButtonDelegate", "View")
                                           << QApplication::translate("ButtonDelegate", "Free");

    DDialog terminateDialog(nullptr);
    terminateDialog.setMessage(QApplication::translate("ButtonDelegate", "Are you sure to terminate this process?"));
    terminateDialog.setIconPixmap(icon);
    terminateDialog.addButtons(btns);

    QObject::connect(&terminateDialog, &DDialog::buttonClicked, [=](const int index) {
        if (index != 2)
            return;

        for (const auto &pid : pidList)
            QProcess::startDetached("kill", QStringList() << pid);
    });

    terminateDialog.exec();
}

ButtonDelegate::ButtonDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
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
        terminate(index.data(ProcessInfoModel::PidListRole).value<QStringList>(), index.data(ProcessInfoModel::IconRole).value<QPixmap>());
        break;
    default:;
    }

    return true;
}
