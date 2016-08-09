#include "sysmonitorthread.h"
#include<iostream>
void SysMonitorThread::run()
{
    LOG(INFO,"SysMonitorThread start");
    proDogtimer = new QTimer;
    sysMonitor = new SysMonitor;
    connect(proDogtimer, SIGNAL(timeout()), sysMonitor, SLOT(monitorProcess()));
    LOG(INFO, "process dog  timer begin");
    sysMonitor->monitorProcess();       //先手动调一遍，然后在从定时器论询。
    proDogtimer->start(QReadConfig::getInstance()->getProcDogConf().nTimer);

    sysTimer = new QTimer;
    connect(sysTimer, SIGNAL(timeout()), sysMonitor, SLOT(monitorSysResource()));
    LOG(INFO,"monitor system resource  timer begin");
    sysTimer->start(QReadConfig::getInstance()->getSysResConf().timer);
    exec();
}


