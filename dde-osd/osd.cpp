/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "osd.h"
#include <X11/extensions/shape.h>
#include <QtX11Extras/QX11Info>
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QBrush>

Osd::Osd(QWidget *parent)
    : QWidget(parent)
{
    initGlobalVars();

    initBasicOperation();

    initConnect();

    initBasicCommandLine();
}

Osd::~Osd()
{
    // set the monitor mode when the app quits
    if (actionMode == SwitchMonitorEnum) {
        m_SwitchMonitor->setMonitorMode();
    }

    // set the keyboard layout when the app quits
    if (actionMode == SwitchLayoutEnum && m_SwitchLayout->layoutCount() >= 1) {
        m_SwitchLayout->setKeyboard();
    }
}

void Osd::initGlobalVars()
{
    m_SwitchNormal = new SwitchNormal(this);

    m_SwitchLayout = new SwitchLayout(this);

    m_SwitchMonitor = new SwitchMonitor(this);

    // m_BackImageLabel is used for the white border
    m_BackImageLabel = new QLabel(this);

    // m_Timer is used to record time , to quit the app properly
    m_Timer = new QTimer(this);
}

void Osd::initBasicOperation()
{
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    // set mouse penetration
    XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput, 0, 0, NULL, 0, ShapeSet, YXBanded);
}

void Osd::initConnect()
{
    connect(m_Timer, &QTimer::timeout, qApp, &QCoreApplication::quit);
}

void Osd::initBasicCommandLine(){
    // command line parser
    m_Parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    m_Parser.addHelpOption();
    m_Parser.addOption(BrightnessUp);
    m_Parser.addOption(BrightnessDown);
    m_Parser.addOption(AudioMute);
    m_Parser.addOption(AudioDown);
    m_Parser.addOption(AudioUp);
    m_Parser.addOption(SwitchLayouts);
    m_Parser.addOption(SwitchMonitors);
}

void Osd::moveToCenter()
{
    // find out the screen that contains mouse.
    QDesktopWidget *desktop = QApplication::desktop();
    int primaryScreenKey = desktop->primaryScreen();
    for (int i = 0; i < desktop->screenCount(); i++) {
        QRect screen = desktop->screenGeometry(primaryScreenKey + i);
        if (screen.contains(QCursor::pos())) {
            m_MouseInScreen = screen;
            break;
        }
    }
    // move to corresponding screen
    this->move(m_MouseInScreen.x() + (m_MouseInScreen.width() - this->width()) / 2, m_MouseInScreen.y() + (m_MouseInScreen.height() - this->height()) / 2);
}

void Osd::processParser(){
    m_Parser.process(*qApp);
}

// when reaches deadline, we need to quit the app immediately
void Osd::setTimer()
{
    m_Timer->start(DEADLINE_TIME);
}

void Osd::tailInWork(){
    moveToCenter();
    setTimer();
    this->update();
}

void Osd::loadBasicNormal(QString whichImage){
    if(whichImage == "Brightness"){
        actionMode = BrightnessProgressBar;
    }else if(whichImage == "Audio"){
        actionMode = AudioProgressBar;
    }else{
        actionMode = NoProgressBar;
    }

    m_SwitchLayout->hideLayout();
    m_SwitchMonitor->hideMonitors();
    this->resize(BASE_SIZE, BASE_SIZE);
    m_BackImageLabel->resize(this->size());
    m_BackImageLabel->setStyleSheet(BACK_IMAGE_STYLE);

    m_SwitchNormal->loadBasicImage(whichImage);
}

bool Osd::handleBasicCmd(){
    if (QDBusConnection::sessionBus().registerService("com.deepin.dde.osd")) {
        QDBusConnection::sessionBus().registerObject("/com/deepin/dde/osd", this, QDBusConnection::ExportAllSlots);
        if (m_Parser.isSet(BrightnessUp) || m_Parser.isSet(BrightnessDown)) {
            loadBasicNormal("Brightness");
        } else if (m_Parser.isSet(AudioUp) || m_Parser.isSet(AudioDown)) {
            loadBasicNormal("Audio");
        } else if (m_Parser.isSet(AudioMute)) {
            loadBasicNormal("AudioMute");
        } else if (m_Parser.isSet(SwitchLayouts)) {
            loadSwitchLayout();
        } else if (m_Parser.isSet(SwitchMonitors)) {
            loadSwitchMonitors();
        }else{
            return false;
        }
        setTimer();
        moveToCenter();
    }else{
        QDBusInterface iface("com.deepin.dde.osd", "/com/deepin/dde/osd", "", QDBusConnection::sessionBus());
        if (m_Parser.isSet(BrightnessUp) || m_Parser.isSet(BrightnessDown)) {
            iface.call("loadBasicNormal", "Brightness");
        } else if (m_Parser.isSet(AudioUp) || m_Parser.isSet(AudioDown)) {
            iface.call("loadBasicNormal", "Audio");
        } else if (m_Parser.isSet(AudioMute)) {
            iface.call("loadBasicNormal", "AudioMute");
        } else if (m_Parser.isSet(SwitchLayouts)) {
            QDBusReply<int> lastActionResult = iface.call("lastAppMode");
            QDBusReply<bool> layoutStatus = iface.call("getLayoutStatus");
            if(!layoutStatus){
                iface.call("loadSwitchLayout");
            }else{
                lastActionResult == SwitchLayoutEnum ? iface.call("highlightNextLayout") : iface.call("highlightCurrentLayout");
            }
        } else if (m_Parser.isSet(SwitchMonitors)) {
            QDBusReply<int> lastActionResult = iface.call("lastAppMode");
            QDBusReply<bool> monitorStatus = iface.call("getMonitorStatus");
            if(!monitorStatus){
                iface.call("loadSwitchMonitors");
            }else{
                lastActionResult == SwitchMonitorEnum ? iface.call("highlightNextMonitor") : iface.call("highlightCurrentMonitor");
            }
        }
        iface.call("tailInWork");
//        this->deleteLater();
        QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
        return false;
    }
    return true;
}

int Osd::lastAppMode(){
    return actionMode;
}

bool Osd::getLayoutStatus(){
    return layoutHasBeenInited;
}

bool Osd::getMonitorStatus(){
    return monitorHasBeenInited;
}

void Osd::cancelNormalText(){
    m_SwitchNormal->cancelText();
}

void Osd::showNormalText(QString text){
    m_SwitchNormal->showText(text);
}

void Osd::addCmdImage(QString cmdString, QString imageName){
    QCommandLineOption cmdOption(cmdString, "\tOSD " + cmdString);
    m_Parser.addOption(cmdOption);
    cmdOptionList << cmdOption;
    imageNameList << imageName;
}

bool Osd::handleAdditionalCmd(){
    for (int i=0,length=cmdOptionList.length();i<length;i++){
        if (QDBusConnection::sessionBus().registerService("com.deepin.dde.osd")) {
            QDBusConnection::sessionBus().registerObject("/com/deepin/dde/osd", this, QDBusConnection::ExportAllSlots);
            if(m_Parser.isSet(cmdOptionList[i])){
                loadAdditionalNormal(imageNameList[i]);
                cancelNormalText();
                setTimer();
                moveToCenter();
                return true;
            }
        }else{
            QDBusInterface iface("com.deepin.dde.osd", "/com/deepin/dde/osd", "", QDBusConnection::sessionBus());
            if(m_Parser.isSet(cmdOptionList[i])){
                iface.call("loadAdditionalNormal", imageNameList[i]);
                iface.call("cancelNormalText");
                iface.call("tailInWork");
//                this->deleteLater();
                QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
                return true;
            }
        }
    }

    return false;
}

void Osd::addCmdImageWithText(QString cmdString, QString imageName, QString text){
    QCommandLineOption cmdOptionWithText(cmdString, "\tOSD " + cmdString);
    m_Parser.addOption(cmdOptionWithText);
    cmdOptionWithTextList << cmdOptionWithText;
    imageNameWithTextList << imageName;
    textList << text;
}

bool Osd::handleAdditionalCmdWithText(){
    for (int i=0,length=cmdOptionWithTextList.length();i<length;i++){
        if (QDBusConnection::sessionBus().registerService("com.deepin.dde.osd")) {
            QDBusConnection::sessionBus().registerObject("/com/deepin/dde/osd", this, QDBusConnection::ExportAllSlots);
            if(m_Parser.isSet(cmdOptionWithTextList[i])){
                loadAdditionalNormal(imageNameWithTextList[i]);
                showNormalText(textList[i]);
                setTimer();
                moveToCenter();
                return true;
            }
        }else{
            QDBusInterface iface("com.deepin.dde.osd", "/com/deepin/dde/osd", "", QDBusConnection::sessionBus());
            if(m_Parser.isSet(cmdOptionWithTextList[i])){
                iface.call("loadAdditionalNormal", imageNameWithTextList[i]);
                iface.call("showNormalText",textList[i]);
                iface.call("tailInWork");
//                this->deleteLater();
                QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
                return true;
            }
        }
    }

    return false;
}

void Osd::loadAdditionalNormal(QString imageName){
    actionMode = NoProgressBar;
    m_SwitchLayout->hideLayout();
    m_SwitchMonitor->hideMonitors();
    m_SwitchNormal->showNormal();
    this->resize(BASE_SIZE, BASE_SIZE);
    m_BackImageLabel->resize(this->size());
    m_BackImageLabel->setStyleSheet(BACK_IMAGE_STYLE);
    m_SwitchNormal->searchAddedImage(imageName);
}

void Osd::loadSwitchLayout(){
    actionMode = SwitchLayoutEnum;
    layoutHasBeenInited = true;
    m_SwitchNormal->hideNormal();
    m_SwitchMonitor->hideMonitors();
    m_SwitchLayout->loadSwitchLayout();
    m_BackImageLabel->resize(this->size());
    m_BackImageLabel->setStyleSheet(BACK_IMAGE_STYLE);
}

void Osd::highlightNextLayout(){
    m_SwitchLayout->highlightNextLayout();
}

void Osd::highlightCurrentLayout(){
    actionMode = SwitchLayoutEnum;
    m_SwitchNormal->hideNormal();
    m_SwitchMonitor->hideMonitors();
    m_SwitchLayout->showLayout();
    m_SwitchLayout->resizeParent();
    m_SwitchLayout->reHiglightKeyboard();
    m_BackImageLabel->resize(this->size());
    m_BackImageLabel->setStyleSheet(BACK_IMAGE_STYLE);
}

void Osd::loadSwitchMonitors(){
    actionMode = SwitchMonitorEnum;
    monitorHasBeenInited = true;
    m_SwitchNormal->hideNormal();
    m_SwitchLayout->hideLayout();
    m_SwitchMonitor->loadSwitchMonitors();
    m_BackImageLabel->resize(this->size());
    m_BackImageLabel->setStyleSheet(BACK_IMAGE_STYLE);
}

void Osd::highlightNextMonitor(){
    m_SwitchMonitor->highlightNextMonitor();
}

void Osd::highlightCurrentMonitor(){
    actionMode = SwitchMonitorEnum;
    m_SwitchNormal->hideNormal();
    m_SwitchLayout->hideLayout();
    m_SwitchMonitor->showMonitors();
    m_SwitchMonitor->resizeParent();
    m_SwitchMonitor->reHighlightMonitor();
    m_BackImageLabel->resize(this->size());
    m_BackImageLabel->setStyleSheet(BACK_IMAGE_STYLE);
}

void Osd::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    // paint app's background
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // for the backgound color
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::transparent);
    pen.setWidth(1);
    QPainterPath path;
    path.addRoundedRect(QRectF(0.5, 0.5, this->width()-1, this->height()-1), 10, 10);
    painter.setPen(pen);
    painter.fillPath(path,QColor(0,0,0,127));

    if (actionMode == AudioProgressBar || actionMode == BrightnessProgressBar) {
        // paint progressbar's background
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        QRectF progressBarBackRect(30,106,80,4);
        brush.setColor(QColor(255,255,255,51));
        painter.setBrush(brush);
        painter.drawRoundedRect(progressBarBackRect,2,2);

        if (actionMode == AudioProgressBar) {
            // paint audio progressbar
            QRectF progressBarRect(30,106,80*(m_SwitchNormal->getVolume()>=1.0 ? 1.0 : m_SwitchNormal->getVolume()),4);
            brush.setColor(Qt::white);
            painter.setBrush(brush);
            painter.drawRoundedRect(progressBarRect,2,2);
        } else if (actionMode == BrightnessProgressBar) {
            // paint brightness progressbar
            QRectF progressBarRect(30,106,80*m_SwitchMonitor->getBrightness(),4);
            brush.setColor(Qt::white);
            painter.setBrush(brush);
            painter.drawRoundedRect(progressBarRect,2,2);
        }
    }
}

