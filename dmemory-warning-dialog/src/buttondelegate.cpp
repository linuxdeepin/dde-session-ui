#include "buttondelegate.h"
#include "processinfomodel.h"

#include <QApplication>
#include <QProcess>
#include <QDebug>

void terminate(const QStringList &pidList)
{
    for (const auto &pid : pidList)
        QProcess::startDetached("kill", QStringList() << pid);
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
        terminate(index.data(ProcessInfoModel::PidListRole).value<QStringList>());
        break;
    default:;
    }

    return true;
}
