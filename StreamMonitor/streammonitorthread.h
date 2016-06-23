#ifndef STREAMMONITORTHREAD_H
#define STREAMMONITORTHREAD_H

#include<QThread>
#include<QTimer>
#include"streammonitor.h"
#include"../config/qreadconfig.h"
#include"localserver.h"
#include "localclient.h"
/**
 * @brief The SysMonitorThread class
 * 流媒体服务监控线程类，关于流媒体提服务监控的业务逻辑都在该线程中实现。
 * 搭建框架，只定时器和监控类两个成员。
 * 定时器用来定时轮询，监控类实现具体的监控业务。
 */

class StreamMonitorThread : public QThread
{
public:
    StreamMonitorThread(QObject *parent=0);

    ~StreamMonitorThread();

protected:
    void run();
signals:
   // void done();

private:
    QTimer *cameraMonTimer;                 //监听摄像机信息计时器

    StreamMonitor *streamMonitor;           //流媒体监控模块
};

#endif // STREAMMONITORTHREAD_H
