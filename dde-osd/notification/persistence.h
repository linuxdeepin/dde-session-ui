/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "constants.h"

#define ACTION_SEGMENT ("|")
#define HINT_SEGMENT ("|")
#define KEYVALUE_SEGMENT ("!!!")

class NotificationEntity;
class Persistence : public QObject
{
    Q_OBJECT
public:
    explicit Persistence(QObject *parent = nullptr);

    void addOne(EntityPtr entity);              //向数据库添加一条通知数据
    void addAll(QList<EntityPtr> entities);     //向数据库添加多条通知数据
    void removeOne(const QString &id);          //根据通知的ID,从数据库删除一条通知.
    void removeApp(const QString &app_name);    //根据App名称从数据库删除App组的通知
    void removeAll();                           //从数据库删除所有通知

    QList<EntityPtr> getAllNotify();            //获取所有通知
    QString getAll();                           //将所有通知转为Json格式的字符串返回
    QString getById(const QString &id);         //根据ID获取通知信息

    // the result starts with offset + 1
    // If rowcount is - 1, it is obtained from offset + 1 to the last.
    QString getFrom(int rowCount, const QString &offsetId);

    int getRecordCount();                       //获取通知记录有多少条

signals:
    void RecordAdded(EntityPtr entity);

private:
    void attemptCreateTable();  //在数据库中尝试创建一个表,记录通知信息
    QString ConvertMapToString(const QVariantMap &map); //将QVariantMap类型转换为QString类型
    QVariantMap ConvertStringToMap(const QString &text); //将QString类型转换为QVariantMap类型

    //判断数据库表中的属性名称是否有效,有效返回true,无效返回false
    bool IsAttributeValid(const QString &tableName, const QString &attributeName);
    //添加一个属性到数据库表中,成功返回true,失败返回false
    bool AddAttributeToTable(const QString &tableName, const QString &attributeName);

private:
    QSqlDatabase m_dbConnection;
    QSqlQuery m_query;
};

#endif // PERSISTENCE_H
