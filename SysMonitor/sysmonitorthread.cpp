#include "sysmonitorthread.h"
#include<iostream>
void SysMonitorThread::run()
{
    proDogtimer = new QTimer;
    sysMonitor = new SysMonitor;
    connect(proDogtimer, SIGNAL(timeout()), sysMonitor, SLOT(monitorProcess()));
    std::cout<<"process dog  timer begin"<<std::endl;
    proDogtimer->start(QReadConfig::getInstance()->getProcDogConf().nTimer);

    sysTimer = new QTimer;
    connect(sysTimer, SIGNAL(timeout()), sysMonitor, SLOT(monitorSysResource()));
    std::cout<<"monitor system resource  timer begin"<<std::endl;
    sysTimer->start(QReadConfig::getInstance()->getSysResConf().timer);
    exec();
}


