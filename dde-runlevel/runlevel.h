#ifndef RUNLEVEL_H
#define RUNLEVEL_H

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <DWindow>

DWIDGET_USE_NAMESPACE

class RunLevel : public DWindow
{
    Q_OBJECT

public:
    RunLevel(QWidget *parent = 0);
    ~RunLevel();

    void setFile(const QString &path);
    void save();

private slots:
    void levelClicked();

private:
    QLabel *m_serviceName;
    QRadioButton *m_powerTarget;
    QRadioButton *m_rescueTarget;
    QRadioButton *m_multiuserTarget;
    QRadioButton *m_graphicalTarget;
    QRadioButton *m_rebootTarget;
    QList<QRadioButton*> m_list;
    QString m_serviceFile;
    QLabel *m_description;
};

#endif // RUNLEVEL_H
