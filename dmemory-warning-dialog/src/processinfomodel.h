#ifndef PROCESSINFOMODEL_H
#define PROCESSINFOMODEL_H

#include "processinfomanager.h"

#include <QAbstractTableModel>
#include <QPointer>

#define COLUMN_ICON     0
#define COLUMN_NAME     1
#define COLUMN_MEM      2
#define COLUMN_FREE_BTN 3
#define COLUMN_NUMS     4

class ProcessInfoModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ProcessInfoModel(QObject *parent = nullptr);

    enum ProcessInfoRole
    {
        IconRole = Qt::DecorationRole,
        DisplayRole = Qt::DisplayRole,

        StateRole = Qt::UserRole,
        PidListRole,
    };

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;

public slots:
    void clearPressed();

private:
    QPointer<ProcessInfoManager> m_processInfos;
    QModelIndex m_pressedIndex;
};

#endif // PROCESSINFOMODEL_H
