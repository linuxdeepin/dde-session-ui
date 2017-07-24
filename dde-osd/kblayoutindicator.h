#ifndef KBLAYOUTINDICATOR_H
#define KBLAYOUTINDICATOR_H

#include <QSystemTrayIcon>
#include <QMenu>

#include <com_deepin_daemon_inputdevice_keyboard.h>

using KeyboardInterface = com::deepin::daemon::inputdevice::Keyboard;

class DataWrapper : public QObject
{
    Q_OBJECT
public:
    explicit DataWrapper(KeyboardInterface *kinter, QObject *parent = nullptr);
    virtual ~DataWrapper() {}

    QString currentLayout() const;
    void setCurrentLayout(const QString &currentLayout);

    QStringList layoutList() const;
    void setLayoutList(const QStringList &layoutList);

    QString localizedNameOfLayout(const QString &layout) const;

signals:
    void layoutListChanged(const QStringList &list);
    void currentLayoutChanged(const QString &layout);

private:
    KeyboardInterface *m_keyboardInter;

    QString m_currentLayout;
    QStringList m_layoutList;
    KeyboardLayoutList m_layoutListAll;

    void initLayoutListAll();
};

class KBLayoutIndicator : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit KBLayoutIndicator(QWidget *parent = nullptr);

private:
    KeyboardInterface *m_keyboardInter;

    DataWrapper *m_data;

    QMenu *m_menu;
    QAction *m_addLayoutAction;

    void updateMenu();
    void updateIcon();

    void switchToNextLayout();

    void handleDataChanged();

    void handleActionTriggered(QAction *action);
    void handleActivated(QSystemTrayIcon::ActivationReason reason);
};

#endif // KBLAYOUTINDICATOR_H
