#include "model.h"

#include <QDebug>

#include "osdprovider.h"

Model::Model(QObject *parent)
    : QAbstractListModel(parent),
      m_provider(nullptr)
{

}

int Model::rowCount(const QModelIndex &index) const
{
    if (!m_provider) return 0;

    return m_provider->rowCount(index);
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!m_provider) return QVariant();

    return m_provider->data(index, role);
}

void Model::setProvider(AbstractOSDProvider *provider)
{
    beginResetModel();
    m_provider = provider;
    endResetModel();
}
