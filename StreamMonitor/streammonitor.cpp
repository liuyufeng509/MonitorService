#include "streammonitor.h"
#include <iostream>
#include "../config/qreadconfig.h"

StreamMonitor::StreamMonitor(QObject *parent) : QObject(parent)
{

}

void StreamMonitor::monitorCamera()
{
   if( QReadConfig::getInstance()->getCameraSvrConf().bOpen)
        std::cout<<"monitoring Camera now...."<<std::endl;
}
