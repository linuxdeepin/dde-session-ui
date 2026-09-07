// SPDX-FileCopyrightText: 2015 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WINDOW_H
#define WINDOW_H

#include <DConfig>

#include <QWidget>
#include <QDBusAbstractAdaptor>
#include <QEvent>
#include <QList>
#include <QMouseEvent>
#include <QTimer>

#include <memory>

const int quitInterval = 10000;

class QLabel;

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

    void setupSize();
    bool ensurePlatformReady();
    void initializePlatformWindow();
    void raiseWindow();
    void scheduleRaiseWindow(int delay = 50);
    void setTimer();
    void setLogoVisible(bool visible);
    void cleanupBeforeExit();
    void grabNativeKeyboard();
    void releaseNativeKeyboard();

protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

public Q_SLOTS:
    void onNameLost(QString name);

private:
    void updateScreenWindows();
    void setScreenWindowsVisible(bool visible);
    QVariant configValue(const QString &key, const QVariant &defaultValue) const;

    int m_clickCount;
    QTimer *m_timer;
    std::unique_ptr<Dtk::Core::DConfig> m_dConfig;
    QLabel *m_logo;
    QLabel *m_shutdownTextLabel;
    bool m_logoVisible;
    bool m_platformWindowInitialized;
    bool m_raisePending;
    bool m_nativeKeyboardGrabbed;
    QList<QWidget *> m_screenWindows;
};

class BlackWidgetAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.deepin.dde.BlackScreen1")

public:
    explicit BlackWidgetAdaptor(Window * parent);
    ~BlackWidgetAdaptor();
    Window *parent() const;

public slots:
    void Raise();
    void Quit();
    void setActive(bool visible);
    void quitDBusService();
    bool blackScreenVisible();
    void releaseGrabDevices();
    void releaseGrabDevicesHideBlack();
};

#endif // WINDOW_H
