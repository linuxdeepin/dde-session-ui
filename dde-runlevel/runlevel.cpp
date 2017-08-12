#include "runlevel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>
#include <QJsonDocument>
#include <QFileInfo>
#include <QSettings>
#include <QProcess>
#include <QDebug>

RunLevel::RunLevel(QWidget *parent)
    : DWindow(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    m_serviceName = new QLabel;
    m_powerTarget = new QRadioButton("poweroff.target");
    m_rescueTarget = new QRadioButton("rescue.target");
    m_multiuserTarget = new QRadioButton("multi-user.target");
    m_graphicalTarget = new QRadioButton("graphical.target");
    m_rebootTarget = new QRadioButton("reboot.target");
    m_description = new QLabel;

    m_description->setWordWrap(true);
    m_description->setAlignment(Qt::AlignLeft);
    m_serviceName->setAlignment(Qt::AlignLeft);

    QHBoxLayout *serNameLayout = new QHBoxLayout;
    serNameLayout->addWidget(new QLabel("服务: "), 0, Qt::AlignLeft);
    serNameLayout->addWidget(m_serviceName, 0, Qt::AlignLeft);
    serNameLayout->addStretch();
    layout->addLayout(serNameLayout);

    QHBoxLayout *descriptionLayout = new QHBoxLayout;
    descriptionLayout->addWidget(new QLabel("描述信息"), 0, Qt::AlignLeft);
    descriptionLayout->addWidget(m_description, 0, Qt::AlignLeft);
    descriptionLayout->addStretch();
    layout->addLayout(descriptionLayout);

    layout->addWidget(new QLabel("服务运行级别 (单选)"), 0, Qt::AlignLeft);

    QVBoxLayout *checkLayout = new QVBoxLayout;
    m_list << m_powerTarget << m_rescueTarget << m_multiuserTarget << m_graphicalTarget << m_rebootTarget;
    for (QRadioButton * radio : m_list) {
        checkLayout->addWidget(radio, 0, Qt::AlignLeft);
        connect(radio, &QRadioButton::clicked, this, &RunLevel::levelClicked);
    }

    layout->addLayout(checkLayout);

    QPushButton *button = new QPushButton("应用");
    layout->addWidget(button);

    setLayout(layout);

    connect(button, &QPushButton::clicked, this, &RunLevel::save);
}

RunLevel::~RunLevel()
{

}

void RunLevel::setFile(const QString &path)
{
    QSettings setting(path, QSettings::NativeFormat);
    m_serviceFile = path;
    QFileInfo file(m_serviceFile);
    m_serviceName->setText(file.baseName());

    setting.beginGroup("Install");
    const QString target = setting.value("WantedBy").toString();

    for (QRadioButton *radio : m_list) {
        radio->blockSignals(true);
        radio->setChecked(radio->text() == target);
        radio->blockSignals(false);
    }

    setting.beginGroup("Unit");
    m_description->setText(setting.value("Description").toString());
}

void RunLevel::save()
{
    QSettings setting(m_serviceFile, QSettings::NativeFormat);
    for (QRadioButton *radio : m_list)
        if (radio->isChecked()) {
            setting.beginGroup("Install");
            setting.setValue("WantedBy", radio->text());
            QFileInfo file(m_serviceFile);
            QProcess::startDetached("systemctl disable " + file.baseName());
            QProcess::startDetached("systemctl enable " + file.baseName());
        }
}

void RunLevel::levelClicked()
{
    QRadioButton *radio = qobject_cast<QRadioButton*>(sender());

    if (radio) {
        for (QRadioButton *r : m_list) {
            r->blockSignals(true);
            r->setChecked(r == radio);
            r->blockSignals(false);
        }
    }
}
