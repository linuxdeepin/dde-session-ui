/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "logmanager.h"
#include <Logger.h>
#include <ConsoleAppender.h>
#include <RollingFileAppender.h>

LogManager::LogManager()
{
    m_format = "%{time}{dd-MM-yyyy, HH:mm:ss.zzz} [%{type:-7}] [%{file:-25} %{line}] %{message}\n";
}

void LogManager::initConsoleAppender(){
    m_consoleAppender = new ConsoleAppender;
    m_consoleAppender->setFormat(m_format);
    logger->registerAppender(m_consoleAppender);
}

void LogManager::initRollingFileAppender(){
    //change the log storge to .cache
    QString cachePath = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).at(0);
    if (!QDir(cachePath).exists()){
        QDir(cachePath).mkpath(cachePath);
    }
    // FIXME:

    m_logPath = joinPath(cachePath, QString("%1.log").arg(qApp->applicationName()));
    m_rollingFileAppender = new RollingFileAppender(m_logPath);
    m_rollingFileAppender->setFormat(m_format);
    m_rollingFileAppender->setLogFilesLimit(5);
    m_rollingFileAppender->setDatePattern(RollingFileAppender::DailyRollover);
    logger->registerAppender(m_rollingFileAppender);
}

void LogManager::debug_log_console_on(){
    #if !defined(QT_NO_DEBUG)
    LogManager::instance()->initConsoleAppender();
    #endif
    LogManager::instance()->initRollingFileAppender();
}


QString LogManager::joinPath(const QString &path, const QString &fileName){
    QString separator(QDir::separator());
    return QString("%1%2%3").arg(path, separator, fileName);
}


QString LogManager::getlogFilePath(){
    return m_logPath;
}

LogManager::~LogManager()
{

}
