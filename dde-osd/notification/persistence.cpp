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

#include "persistence.h"

#include <QStandardPaths>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "notificationentity.h"

static const QString TableName = "notifications";
static const QString TableName_v2 = "notifications2";
static const QString ColumnId = "ID";
static const QString ColumnIcon = "Icon";
static const QString ColumnSummary = "Summary";
static const QString ColumnBody = "Body";
static const QString ColumnAppName = "AppName";
static const QString ColumnCTime = "CTime";
static const QString ColumnReplacesId = "ReplacesId";
static const QString ColumnTimeout = "Timeout";

Persistence::Persistence(QObject *parent)
    : QObject(parent)
{
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir(dataDir);
    if (!dir.exists()) {
        dir.mkpath(dataDir);
    }

    m_dbConnection = QSqlDatabase::addDatabase("QSQLITE", "QSQLITE");
    m_dbConnection.setDatabaseName(dataDir + "/" + "data.db");
    if (!m_dbConnection.open()) {
        qWarning() << "open database error" << m_dbConnection.lastError().text();
    } else {
#ifdef QT_DEBUG
        qDebug() << "database open";
#endif
    }

    m_query = QSqlQuery(m_dbConnection);
    m_query.setForwardOnly(true);

    attemptCreateTable();
}

void Persistence::addOne(NotificationEntity *entity)
{
    m_query.prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8)"
                            "VALUES (:icon, :summary, :body, :appname, :ctime, :replacesid, :timeout)") \
                  .arg(TableName_v2, ColumnIcon, ColumnSummary, ColumnBody,
                       ColumnAppName, ColumnCTime, ColumnReplacesId, ColumnTimeout));
    m_query.bindValue(":icon", entity->appIcon());
    m_query.bindValue(":summary", entity->summary());
    m_query.bindValue(":body", entity->body());
    m_query.bindValue(":appname", entity->appName());
    m_query.bindValue(":ctime", entity->ctime());
    m_query.bindValue(":replacesid", entity->replacesId());
    m_query.bindValue(":timeout", entity->timeout());

    if (!m_query.exec()) {
        qWarning() << "insert value to database failed: " << m_query.lastError().text() << entity->id() << entity->ctime();
        return;
    } else {
#ifdef QT_DEBUG
        qDebug() << "insert value done, time is:" << entity->ctime();
#endif
    }

    // to get entity's id in database
    if (!m_query.exec(QString("SELECT last_insert_rowid() FROM %1;").arg(TableName_v2))) {
        qWarning() << "get entity's id failed: " << m_query.lastError().text() << entity->id() << entity->ctime();
        return;
    } else {
        m_query.next();
        entity->setId(m_query.value(0).toString());
#ifdef QT_DEBUG
        qDebug() << "get entity's id done:" << entity->id();
#endif
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
    m_query.prepare(QString("DELETE FROM %1 WHERE ID = (:id)").arg(TableName_v2));
    m_query.bindValue(":id", id);

    if (!m_query.exec()) {
        qWarning() << "remove value:" << id << "from database failed: " << m_query.lastError().text();
        return;
    } else {
#ifdef QT_DEBUG
        qDebug() << "remove value:" << id;
#endif
    }
}

void Persistence::removeAll()
{
    m_query.prepare(QString("DELETE FROM %1").arg(TableName_v2));

    if (!m_query.exec()) {
        qWarning() << "remove all from database failed: " << m_query.lastError().text();
        return;
    } else {
#ifdef QT_DEBUG
        qDebug() << "remove all done";
#endif
    }

    // Remove the unused space
    if (!m_query.exec("VACUUM")) {
        qWarning() << "remove the unused space failed: " << m_query.lastError().text();
        return;
    } else {
#ifdef QT_DEBUG
        qDebug() << "remove the unused space done";
#endif
    }
}

QString Persistence::getAll()
{
    m_query.prepare(QString("SELECT %1, %2, %3, %4, %5, %6 FROM %7")
               .arg(ColumnId, ColumnIcon, ColumnSummary, ColumnBody, ColumnAppName,
                    ColumnCTime, TableName_v2));

    if (!m_query.exec()) {
        qWarning() << "get all from database failed: " << m_query.lastError().text();
        return QString();
    } else {
#ifdef QT_DEBUG
        qDebug() << "get all done";
#endif
    }

    QJsonArray array1;
    while (m_query.next())
    {
        QJsonObject obj
        {
            {"id", m_query.value(0).toString()},
            {"icon", m_query.value(1).toString()},
            {"summary", m_query.value(2).toString()},
            {"body", m_query.value(3).toString()},
            {"name", m_query.value(4).toString()},
            {"time", m_query.value(5).toString()}
        };
        array1.append(obj);
    }
    return QJsonDocument(array1).toJson();
}

QString Persistence::getById(const QString &id)
{
    m_query.prepare(QString("SELECT %1, %2, %3, %4, %5, %6 FROM %7 WHERE ID = (:id)")
               .arg(ColumnId, ColumnIcon, ColumnSummary, ColumnBody, ColumnAppName,
                    ColumnCTime, TableName_v2));
    m_query.bindValue(":id", id);

    if (!m_query.exec()) {
        qWarning() << "get data by id:" << id << "failed: " << m_query.lastError().text();
        return QString();
    } else {
#ifdef QT_DEBUG
        qDebug() << "get data by id:" << id << "done";
#endif
    }

    QJsonArray array;
    while (m_query.next())
    {
        QJsonObject obj
        {
            {"id", m_query.value(0).toString()},
            {"icon", m_query.value(1).toString()},
            {"summary", m_query.value(2).toString()},
            {"body", m_query.value(3).toString()},
            {"name", m_query.value(4).toString()},
            {"time", m_query.value(5).toString()}
        };
        array.append(obj);
    }
    qDebug() << array;

    if (array.size() > 1) {
        qWarning() << "more than one data has been obtained by id:" << id;
    }

    return QJsonDocument(array).toJson();
}

QString Persistence::getFrom(int rowCount, const QString &offsetId)
{
    // gets the line number of the specified offset
    m_query.prepare(QString("SELECT count() FROM %1 WHERE ID <= (:offsetId)").arg(TableName_v2));
    m_query.bindValue(":offsetId", offsetId);

    if (!m_query.exec()) {
        qWarning() << "get line number failed: " << m_query.lastError().text();
        return QString();
    } else {
#ifdef QT_DEBUG
        qDebug() << "get line number done";
#endif
    }

    m_query.next();
    QString rowNum = m_query.value(0).toString();
    if (rowNum.isEmpty()) {
        qWarning() << "the line number is invalid: ";
        return QString();
    } else {
#ifdef QT_DEBUG
        qDebug() << "line number is valid";
#endif
    }

    // get data from rowNum+1
    m_query.prepare(QString("SELECT %1, %2, %3, %4, %5, %6 FROM %7 LIMIT (:rowCount) OFFSET (:offset)")
               .arg(ColumnId, ColumnIcon, ColumnSummary, ColumnBody, ColumnAppName,
                    ColumnCTime, TableName_v2));
    m_query.bindValue(":rowCount", rowCount);
    m_query.bindValue(":offset", rowNum);

    if (!m_query.exec()) {
        qWarning() << "get data from database failed: " << m_query.lastError().text();
        return QString();
    } else {
#ifdef QT_DEBUG
        qDebug() << "get data done";
#endif
    }

    QJsonArray array;
    while (m_query.next())
    {
        QJsonObject obj
        {
            {"id", m_query.value(0).toString()},
            {"icon", m_query.value(1).toString()},
            {"summary", m_query.value(2).toString()},
            {"body", m_query.value(3).toString()},
            {"name", m_query.value(4).toString()},
            {"time", m_query.value(5).toString()}
        };
        array.append(obj);
    }
    qDebug() << array;

    return QJsonDocument(array).toJson();
}

void Persistence::attemptCreateTable()
{

    m_query.prepare(QString("CREATE TABLE IF NOT EXISTS %1"
                          "("
                          "%2 INTEGER PRIMARY KEY   AUTOINCREMENT,"
                          "%3 TEXT,"
                          "%4 TEXT,"
                          "%5 TEXT,"
                          "%6 TEXT,"
                          "%7 TEXT,"
                          "%8 TEXT,"
                          "%9 TEXT"
                          ");").arg(TableName_v2,
                                ColumnId, ColumnIcon, ColumnSummary,
                                ColumnBody, ColumnAppName, ColumnCTime,
                                ColumnReplacesId, ColumnTimeout));

    if (!m_query.exec()) {
        qWarning() << "create table failed" << m_query.lastError().text();
    }
}
