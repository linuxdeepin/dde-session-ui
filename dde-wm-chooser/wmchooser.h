#ifndef WMCHOOSER_H
#define WMCHOOSER_H

#include "../boxframe/boxframe.h"

#include <QWidget>
#include <QPushButton>

class WMChooser : public QWidget
{
    Q_OBJECT

public:
    WMChooser(QWidget *parent = 0);

    void setConfigPath(const QString &path);
    void createParentDir(const QString &fullfilepath);

signals:
    void screenChanged();

private slots:
    void saveConfig();

protected:
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    QPushButton *m_gorgeousBtn;
    QPushButton *m_topSpeedBtn;
    QString m_configPath;
};

#endif // WMCHOOSER_H
