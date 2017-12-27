/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "persistence.h"

#include <QStandardPaths>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QDir>

#include "notificationentity.h"

static const QString TableName = "notifications";
static const QString ColumnId = "ID";
static const QString ColumnIcon = "Icon";
static const QString ColumnSummary = "Summary";
static const QString ColumnBody = "Body";
static const QString ColumnAppName = "AppName";
static const QString ColumnCTime = "CTime";

Persistence::Persistence(QObject *parent)
    : QObject(parent)
{
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir(dataDir);
    if (!dir.exists())
        dir.mkdir(dataDir);

    m_dbConnection = QSqlDatabase::addDatabase("QSQLITE", "QSQLITE");
    m_dbConnection.setDatabaseName(dataDir + "/" + "data.db");
    if (!m_dbConnection.open()) {
        qWarning() << "open database error" << m_dbConnection.lastError().text();
    } else {
        qDebug() << "database open";
    }

    QSqlQuery query(m_dbConnection);
    query.prepare(QString("CREATE TABLE IF NOT EXISTS %1"
                          "("
                          "%2 TEXT PRIMARY KEY,"
                          "%3 TEXT,"
                          "%4 TEXT,"
                          "%5 TEXT,"
                          "%6 TEXT,"
                          "%7 TEXT"
                          ");").arg(TableName, ColumnId, ColumnIcon, ColumnSummary, ColumnBody, ColumnAppName, ColumnCTime));

    if (!query.exec()) {
        qWarning() << "create table failed" << query.lastError().text();
    }
}

void Persistence::addOne(NotificationEntity *entity)
{
    QSqlQuery query(m_dbConnection);
    query.prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6, %7) VALUES (:id, :icon, :summary, :body, :appname, :ctime)") \
                  .arg(TableName, ColumnId, ColumnIcon, ColumnSummary, ColumnBody, ColumnAppName, ColumnCTime));
    query.bindValue(":id", entity->id());
    query.bindValue(":icon", entity->appIcon());
    query.bindValue(":summary", entity->summary());
    query.bindValue(":body", entity->body());
    query.bindValue(":appname", entity->appName());
    query.bindValue(":ctime", entity->id());

    if (!query.exec()) {
        qWarning() << "insert value to database failed: " << query.lastError().text();
    } else {
        qDebug() << "insert value " << entity->id();
        emit RecordAdded(entity);
    }
}

void Persistence::addAll(QList<NotificationEntity *> entities)
{
    for (NotificationEntity *entity : entities) {
        addOne(entity);
    }
}

void Persistence::removeOne(const QString &id)
{
    QSqlQuery query(m_dbConnection);
    query.prepare(QString("DELETE FROM %1 WHERE id = (:id)").arg(TableName));
    query.bindValue(":id", id);
    query.exec();
}

void Persistence::removeAll()
{
    QSqlQuery query(m_dbConnection);
    query.prepare(QString("DELETE FROM %1").arg(TableName));
    query.exec();
}

QList<NotificationEntity> Persistence::getAll()
{
    QList<NotificationEntity> ret;

    QSqlQuery query(QString("SELECT * FROM %1").arg(TableName), m_dbConnection);

    const int idName = query.record().indexOf(ColumnId);
    const int iconName = query.record().indexOf(ColumnIcon);
    const int summaryName = query.record().indexOf(ColumnSummary);
    const int bodyName = query.record().indexOf(ColumnBody);
    const int appNameName = query.record().indexOf(ColumnAppName);
//    const int ctimeName = query.record().indexOf(ColumnCTime);

    while (query.next())
    {
        const QString appName = query.value(appNameName).toString();
        const QString id = query.value(idName).toString();
        const QString icon = query.value(iconName).toString();
        const QString summary = query.value(summaryName).toString();
        const QString body = query.value(bodyName).toString();

        // ID is the same as ctime at the current stage.
        // const QString ctime = query.value(ctimeName).toString();

        NotificationEntity ent(appName, id, icon, summary, body, QStringList(), QVariantMap());
        ret << ent;
    }

    return ret;
}
