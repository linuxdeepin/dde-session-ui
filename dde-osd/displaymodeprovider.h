#ifndef DISPLAYMODEPROVIDER_H
#define DISPLAYMODEPROVIDER_H

#include "osdprovider.h"

#include <com_deepin_daemon_display.h>

using namespace com::deepin::daemon;

class DisplayModeProvider : public AbstractOSDProvider
{
    Q_OBJECT
public:
    explicit DisplayModeProvider(QObject *parent = 0);

    bool checkConditions() const Q_DECL_OVERRIDE;
    QSize contentSize() const Q_DECL_OVERRIDE;

    QListView::Flow flow() const Q_DECL_OVERRIDE;

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
    QList<QPair<uchar,QString>> m_planItems;
    QPair<uchar,QString> m_currentPlan;

    uchar m_displayMode;
    QStringList m_outputNames;
    QString m_primaryScreen;

    Display *m_displayInter;

    void updateOutputNames();
    void updatePlanItems();

    QString getPlanItemName(QPair<uchar,QString> &plan) const;
    QString getPlanItemIcon(QPair<uchar,QString> &plan) const;

private slots:
    void displayModeChanged(const uchar &mode);
    void primaryChanged(const QString &primary);
};

#endif // DISPLAYMODEPROVIDER_H
