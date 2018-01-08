#ifndef PROCESSINFOTABLE_H
#define PROCESSINFOTABLE_H

#include <QTableView>

class ProcessInfoTable : public QTableView
{
    Q_OBJECT

public:
    explicit ProcessInfoTable(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model);

protected:
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // PROCESSINFOTABLE_H
