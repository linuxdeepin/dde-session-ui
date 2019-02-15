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
    m_model->setIsShow(true);

    emit m_model->visibleChanged(true);
}

void DBusLockAgent::ShowUserList()
{
    emit m_model->showUserList();
}
