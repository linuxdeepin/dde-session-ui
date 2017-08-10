#ifndef OPTIONWIDGET_H
#define OPTIONWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <com_deepin_daemon_langselector.h>

using LangSelector = com::deepin::daemon::LangSelector;

class OptionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OptionWidget(const QString &title, QWidget *parent = 0);

signals:
    void requestSetValue(const QVariant &value);

public slots:
    void setText(const QString &value);

private slots:
    void itemSelected(const int index);
    void getLocaleList(QDBusPendingCallWatcher *w);
    void getCurrentLocale(const QString &value);

private:
    QComboBox *m_combox;
    QLabel *m_label;
    QMap<QString, QLocale> m_list;
    LangSelector *m_dbus;
    LocaleList m_localelist;
};

#endif // OPTIONWIDGET_H
