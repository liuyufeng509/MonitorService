#ifndef SYSMONITOR_H
#define SYSMONITOR_H

#include <QObject>
/**
 * @brief The SysMonitor class
 * 该类属于系统资源监控类，实现具体的系统资源业务逻辑监控，如CPU，内存等。
 * 此处只实现了简单的信息打印。
 */
class SysMonitor : public QObject
{
    Q_OBJECT
public:
    explicit SysMonitor(QObject *parent = 0);

signals:

public slots:
    void monitorCPU();
};

#endif // SYSMONITOR_H
