#include "optionwidget.h"
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>

OptionWidget::OptionWidget(const QString &title, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(10);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setFixedWidth(100);
    layout->addWidget(titleLabel, 0, Qt::AlignLeft);
    QList<QLocale> allLocales = QLocale::matchingLocales(
                QLocale::AnyLanguage,
                QLocale::AnyScript,
                QLocale::AnyCountry);

    m_label = new QLabel;
    layout->addWidget(m_label);
    m_label->hide();
    m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_combox = new QComboBox;
    m_combox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_combox, 0, Qt::AlignLeft);
    layout->addStretch();

    m_dbus = new LangSelector("com.deepin.daemon.LangSelector",
                              "/com/deepin/daemon/LangSelector",
                              QDBusConnection::sessionBus(), this);

    m_dbus->setSync(false);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(m_dbus->GetLocaleList(), this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &OptionWidget::getLocaleList);

    connect(m_dbus, &__LangSelector::CurrentLocaleChanged, this, &OptionWidget::getCurrentLocale);

    setLayout(layout);

    connect(m_combox, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, &OptionWidget::itemSelected);
}

void OptionWidget::setText(const QString &value)
{
    m_combox->hide();
    m_label->show();

    m_label->setText(value.simplified());
}

void OptionWidget::itemSelected(const int index)
{
    emit requestSetValue(m_combox->itemData(index));
}

void OptionWidget::getLocaleList(QDBusPendingCallWatcher *w)
{
    QDBusPendingReply<LocaleList> reply = w->reply();
    m_localelist = reply.value();
    for (const LocaleInfo &locale : m_localelist) {
        QVariant v;
        v.setValue(locale);
        m_combox->addItem(locale.name + "(" + locale.id + ")", v);
    }

    m_dbus->currentLocale();

    w->deleteLater();
}

void OptionWidget::getCurrentLocale(const QString &value)
{
    for (const LocaleInfo &locale : m_localelist) {
        if (locale.id == value) {
            QVariant v;
            v.setValue(locale);
            m_combox->setCurrentIndex(m_combox->findData(v));
            emit requestSetValue(v);
            break;
        }
    }
}
