#ifndef SYSMONITORTHREAD_H
#define SYSMONITORTHREAD_H

#include<QThread>
#include<QTimer>
#include"sysmonitor.h"
#include"../config/qreadconfig.h"
/**
 * @brief The SysMonitorThread class
 * 系统资源监控线程类，关于系统资源监控的业务逻辑都在该线程中实现。
 * 搭建框架，只定时器和监控类两个成员。
 * 定时器用来定时轮询，监控类实现具体的监控业务。
 */

class SysMonitorThread : public QThread
{
public:
    SysMonitorThread(QObject *parent=0)
    {
        //timer = new QTimer(this);
        //sysMonitor = new SysMonitor(this);
    }
    ~SysMonitorThread()
    {
        delete timer;
        delete sysMonitor;
    }

   //void monitorCPU();

protected:
    void run();
signals:
   // void done();

private:
    QTimer *timer;

    SysMonitor *sysMonitor;
};

#endif // SYSMONITORTHREAD_H
