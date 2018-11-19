#include "dbuslockagent.h"

#include "sessionbasemodel.h"

DBusLockAgent::DBusLockAgent(QObject *parent) : QObject(parent)
{

}

void DBusLockAgent::setModel(SessionBaseModel * const model)
{
    m_model = model;
}

void DBusLockAgent::Show()
{
    emit m_model->show();
}

void DBusLockAgent::ShowUserList()
{
    emit m_model->showUserList();
}
