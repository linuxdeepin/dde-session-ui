#ifndef PROCESSINFOMANAGER_H
#define PROCESSINFOMANAGER_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <QPointer>

#include <com_deepin_startmanager.h>

using StartManagerInter = com::deepin::StartManager;

class ProcessInfo
{
public:
    unsigned total_mem_bytes;
    QString cgroup_path;
    QString app_name;
    QString desktop;
    QStringList pid_list;
};

class ProcessInfoModel;
class ProcessInfoManager : public QObject
{
    Q_OBJECT

    friend class ProcessInfoModel;

public:
    explicit ProcessInfoManager(QObject *parent = nullptr);

signals:
    void processInfoListChanged() const;

private slots:
    void scanProcessInfos();
    void startRefreshData();
    void stopRefreshData() { m_refreshTimer->stop(); }

private:
    void appendCGroupPath(const QString &path, const QString &desktop);

private:
    QPointer<QTimer> m_refreshTimer;
    QPointer<StartManagerInter> m_startManagerInter;
    QString m_sessionId;
    QList<ProcessInfo> processInfoList;
};

#endif // PROCESSINFOMANAGER_H
