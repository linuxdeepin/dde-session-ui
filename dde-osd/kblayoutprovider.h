#ifndef KBLAYOUTPROVIDER_H
#define KBLAYOUTPROVIDER_H

#include "osdprovider.h"

#include <com_deepin_daemon_inputdevice_keyboard.h>

using namespace com::deepin::daemon::inputdevice;

class KBLayoutProvider : public AbstractOSDProvider
{
    Q_OBJECT
public:
    explicit KBLayoutProvider(QObject *parent = 0);

    bool checkConditions() const Q_DECL_OVERRIDE;
    QSize contentSize() const Q_DECL_OVERRIDE;
    QMargins contentMargins() const Q_DECL_OVERRIDE;

    void highlightCurrent() Q_DECL_OVERRIDE;
    void highlightNext() Q_DECL_OVERRIDE;

    void sync() Q_DECL_OVERRIDE;

    // model
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    // delegate
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    KeyboardLayoutList m_database;

    QStringList m_userLayouts;
    QString m_userCurrentLayout;

    Keyboard *m_keyboardInter;

    QString describeLayout(const QString &layout) const;

private slots:
    void currentLayoutChanged(const QString &layout);
    void userLayoutListChanged(const QStringList &layouts);
};

#endif // KBLAYOUTPROVIDER_H
