#include "sysmonitor.h"
#include <iostream>
#include "../config/qreadconfig.h"
SysMonitor::SysMonitor(QObject *parent) : QObject(parent)
{

}

void SysMonitor::monitorCPU()
{
   if( QReadConfig::getInstance()->getCPUConf().bOpen)
        std::cout<<"monitoring CPU now...."<<std::endl;
}
