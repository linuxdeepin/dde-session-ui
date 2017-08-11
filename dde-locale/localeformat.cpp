#include "localeformat.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QDateTime>
#include <QPushButton>
#include <QFile>
#include <QSettings>

LocaleFormat::LocaleFormat(QWidget *parent)
    : QWidget(parent)
{
    region = new OptionWidget("区域");
    number = new OptionWidget("数字");
    date = new OptionWidget("时间");
    money = new OptionWidget("货币");

    QCheckBox *check = new QCheckBox("细节设置");
    check->setChecked(false);
    setOtherEnable(false);

    numberLabel = new OptionWidget("数字");
    numberLabel->setText("1,000.01");
    dateLabel = new OptionWidget("时间");
    dateLabel->setText("2017");
    dateShortLabel = new OptionWidget("");
    dateShortLabel->setText("20");
    moneyLabel = new OptionWidget("货币");
    moneyLabel->setText("￥");

    QList<QWidget*> list;
    QList<OptionWidget*> alone;
    alone << number << date << money;
    list << region << check << number << date << money << new QLabel("示例") << numberLabel << dateLabel << dateShortLabel << moneyLabel;
    QVBoxLayout *layout = new QVBoxLayout;

    for (QWidget *w : list)
        layout->addWidget(w);

    QPushButton *acceptBtn = new QPushButton("应用");
    layout->addWidget(acceptBtn);

    setLayout(layout);

    connect(check, &QCheckBox::clicked, this, &LocaleFormat::setOtherEnable);
    connect(region, &OptionWidget::requestSetValue, this, &LocaleFormat::setAllInfo);

    for (OptionWidget * w : alone) {
        connect(w, &OptionWidget::requestSetValue, this, &LocaleFormat::setSingleValue);
    }
    connect(acceptBtn, &QPushButton::clicked, this, &LocaleFormat::saveAllInfo);
}

LocaleFormat::~LocaleFormat()
{
}

void LocaleFormat::setOtherEnable(const bool value)
{
    number->setEnabled(value);
    date->setEnabled(value);
    money->setEnabled(value);
}

void LocaleFormat::setAllInfo(const QVariant &value)
{
    LocaleInfo localeinfo = value.value<LocaleInfo>();
    QLocale locale(localeinfo.id);
    numberLabel->setText(locale.toCurrencyString(double(123456789)," "));
    dateLabel->setText(QDateTime::currentDateTime().toString(locale.dateFormat(QLocale::ShortFormat)) + "(短日期格式)");
    dateShortLabel->setText(QDateTime::currentDateTime().toString(locale.dateFormat(QLocale::LongFormat)) + "(长日期格式)");
    moneyLabel->setText(locale.toCurrencyString(double(123456789)));

    m_setting.All = localeinfo;
    m_setting.date = localeinfo;
    m_setting.money = localeinfo;
    m_setting.number = localeinfo;
}

void LocaleFormat::setSingleValue(const QVariant &value)
{
    LocaleInfo localeinfo = value.value<LocaleInfo>();
    QLocale locale(localeinfo.id);
    OptionWidget *w = qobject_cast<OptionWidget*>(sender());
    if (w) {
        if (w == number) {
            numberLabel->setText(locale.toCurrencyString(double(123456789)," "));
            m_setting.number = localeinfo;
        }
        if (w == date) {
            dateLabel->setText(QDateTime::currentDateTime().toString(locale.dateFormat(QLocale::ShortFormat)) + "(短日期格式)");
            dateShortLabel->setText(QDateTime::currentDateTime().toString(locale.dateFormat(QLocale::LongFormat)) + "(长日期格式)");
            m_setting.date = localeinfo;
        }
        if (w == money) {
            moneyLabel->setText(locale.toCurrencyString(24));
            m_setting.money = localeinfo;
        }
    }
}

void LocaleFormat::saveAllInfo()
{
    const QString &localeFile = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first() + "/.pam_environment";
    QSettings settings(localeFile, QSettings::IniFormat);
    settings.setValue("LANG", m_setting.All.id);
    QLocale locale(m_setting.All.id);
    settings.setValue("LANGUAGE", locale.name());
    settings.setValue("LC_NUMERIC", m_setting.number.id);
    settings.setValue("LC_TIME", m_setting.date.id);
    settings.setValue("LC_MONETARY", m_setting.money.id);
}
