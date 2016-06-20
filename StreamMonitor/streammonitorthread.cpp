#include "streammonitorthread.h"
#include<iostream>
void StreamMonitorThread::run()
{
    timer = new QTimer;
    streamMonitor = new StreamMonitor;
    connect(timer, SIGNAL(timeout()), streamMonitor, SLOT(monitorCamera()));
    std::cout<<" StreamThead timer begin"<<std::endl;
    timer->start(1000);
    exec();
}
