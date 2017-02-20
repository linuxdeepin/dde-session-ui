/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef OSD_H
#define OSD_H

#include "switchmonitor.h"
#include "switchlayout.h"
#include "switchnormal.h"
#include <QWidget>
#include <QPaintEvent>
#include <QTimer>
#include <QList>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <DBlurEffectWidget>

DWIDGET_USE_NAMESPACE

class OsdOption: public QCommandLineOption
{
public:
    OsdOption(const QString &name,
              const QString &imageName = QString(),
              const QString &text = QString()) :
        QCommandLineOption(name, "\tOSD " + name), m_imageName(imageName), m_text(text)
    {

    }

    QString imageName() const {return m_imageName;}
    void setText(const QString &text) {m_text = text;}
    QString text() const {return m_text;}

private:
    QString m_imageName;
    QString m_text;
};

class Osd;

class Osd : public DBlurEffectWidget
{
    Q_OBJECT

public:
    Osd(QWidget *parent = 0);
    ~Osd();

    void showOSD();
    void dbusShowOSD();

public:
    void loadBasicNormal(QString whichImage);
    void loadAdditionalNormal(QString imageName);
    void loadSwitchLayout();
    void highlightNextLayout();
    void highlightCurrentLayout();
    void loadSwitchMonitors();
    void highlightNextMonitor();
    void highlightCurrentMonitor();
    void tailInWork();
    int lastAppMode();
    bool getLayoutStatus();
    bool getMonitorStatus();
    void cancelNormalText();
    void showNormalText(QString text);

    void processParser();
    void processParser(const QStringList &args);

    void delayAction();

private:
    void initGlobalVars();
    void initBasicOperation();
    void initBasicCommandLine();
    void initConnect();

    void setTimer();
    void moveToCenter();

    void paintEvent(QPaintEvent *e);

    SwitchNormal *m_SwitchNormal;
    SwitchLayout *m_SwitchLayout;
    SwitchMonitor *m_SwitchMonitor;
    QCommandLineParser m_Parser;
    QRect m_MouseInScreen;
    QTimer *m_Timer;

    QList<QCommandLineOption> cmdOptionList;
    QList<QString> imageNameList;

    QList<QCommandLineOption> cmdOptionWithTextList;
    QList<QString> imageNameWithTextList;
    QList<QString> textList;

    enum ActionMode {
        Unknow,
        SwitchLayoutEnum,
        SwitchMonitorEnum,
        NoProgressBar,
        AudioProgressBar,
        BrightnessProgressBar
    } actionMode = Unknow;

    bool layoutHasBeenInited = false;
    bool monitorHasBeenInited = false;

    const int BASE_SIZE = 140;
    const int DEADLINE_TIME = 2000;
};


class OsdDBus: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.osd")

public:
    explicit OsdDBus(Osd *parent = 0): QObject(parent)
    {
        m_Osd = parent;
    }

public slots:
    Q_SCRIPTABLE void ShowOSD(QString osd)
    {
        QStringList fakeArgs;
        fakeArgs << qApp->arguments().first();
        fakeArgs << "--" + osd;
        qDebug() << "DBus Call ShowOSD" << osd << fakeArgs;
        m_Osd->processParser(fakeArgs);
        m_Osd->showOSD();
    }

private:
    Osd *m_Osd;
};

#endif // OSD_H
