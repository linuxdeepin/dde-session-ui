#ifndef LOCALEFORMAT_H
#define LOCALEFORMAT_H

#include "optionwidget.h"
#include <QWidget>
#include <QLabel>

class LocaleFormat : public QWidget
{
    Q_OBJECT

    struct Setting {
        LocaleInfo All;
        LocaleInfo number;
        LocaleInfo date;
        LocaleInfo money;
    };

public:
    LocaleFormat(QWidget *parent = 0);
    ~LocaleFormat();

private slots:
    void setOtherEnable(const bool value);
    void setAllInfo(const QVariant &value);
    void setSingleValue(const QVariant &value);
    void saveAllInfo();

private:
    OptionWidget *region;
    OptionWidget *number;
    OptionWidget *date;
    OptionWidget *money;
    OptionWidget *numberLabel;
    OptionWidget *dateLabel;
    OptionWidget *dateShortLabel;
    OptionWidget *moneyLabel;
    Setting m_setting;
};

#endif // LOCALEFORMAT_H
