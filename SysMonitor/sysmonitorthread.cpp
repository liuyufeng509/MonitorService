#include "sysmonitorthread.h"
#include<iostream>
void SysMonitorThread::run()
{
    timer = new QTimer;
    sysMonitor = new SysMonitor;
    connect(timer, SIGNAL(timeout()), sysMonitor, SLOT(monitorCPU()));
    std::cout<<"SysMonitor timer begin"<<std::endl;
    timer->start(1000);
    exec();
}

//void SysMonitorThread::monitorCPU()
//{
//   if(QReadConfig::getInstance()->getCPUConf().bOpen)
//       {
//       }
//   }
//}

