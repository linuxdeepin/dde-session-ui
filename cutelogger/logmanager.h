/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QtCore>

class ConsoleAppender;
class RollingFileAppender;

class LogManager
{
public:
    void initConsoleAppender();
    void initRollingFileAppender();

    inline static LogManager* instance(){
        static LogManager instance;
        return &instance;
    }

    void debug_log_console_on();
    QString joinPath(const QString& path, const QString& fileName);
    QString getlogFilePath();

signals:

public slots:

private:
    QString m_format;
    QString m_logPath;
    ConsoleAppender* m_consoleAppender;
    RollingFileAppender* m_rollingFileAppender;


    explicit LogManager();
    ~LogManager();
    LogManager(const LogManager &);
    LogManager & operator = (const LogManager &);
};

#endif // LOGMANAGER_H
