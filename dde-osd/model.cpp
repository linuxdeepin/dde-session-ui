#include "model.h"

#include <QDebug>

Model::Model(QObject *parent)
    : QAbstractListModel(parent)
{

}

int Model::rowCount(const QModelIndex &) const
{
    return m_data.length();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    QPair<QString, QString> item = m_data.at(index.row());
    if (role == Qt::DecorationRole) {
        return item.first;
    }
    return item.second;
}

void Model::setData(const QList<QPair<QString, QString> > &data)
{
    beginResetModel();
    m_data = data;
    endResetModel();
}
