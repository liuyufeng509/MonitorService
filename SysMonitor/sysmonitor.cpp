#include "sysmonitor.h"
#include <iostream>
#include "../config/qreadconfig.h"
SysMonitor::SysMonitor(QObject *parent) : QObject(parent)
{
    memset(&systemResource,0,sizeof(systemResource));
    systemResource.cpu.MediaProcessName = new char[255];
    strcpy(systemResource.cpu.MediaProcessName, QReadConfig::getInstance()->getProcDogConf().strProcName.toStdString().c_str());
    systemResource.memory.processName = new char[255];
    strcpy(systemResource.memory.processName, QReadConfig::getInstance()->getProcDogConf().strProcName.toStdString().c_str());



    systemResourceMonitor = new SystemResourceMonitor;
}

SysMonitor::~SysMonitor()
{
    delete[] systemResource.cpu.MediaProcessName;
    delete[] systemResource.memory.processName;
    delete systemResourceMonitor;
}

void SysMonitor::monitorProcess()
{
    systemResourceMonitor->MediaProcessMonitor();
}

void SysMonitor::monitorSysResource()
{
    //CPU
    /*please input process name and thread ID in struct cpu*/
    systemResourceMonitor->GetSysProThrCpuUsage(systemResource.cpu);

    //内存
    /*please input process name and thread ID in struct memory*/
   systemResourceMonitor->GetSysProThrMemUsage(systemResource.memory);        //获取系统系统内存空间/剩余大小/进程/线程内存使用率


   //网络
   systemResourceMonitor->GetNetInterfaceInfo(systemResource.net);                   //获取网络接口使用状况信息111


   //------------test print-----------
   cout<<"---------cpu-----------"<<endl;
   cout<<"系统CPU使用率："<<systemResource.cpu.systemCpu<<endl;
   cout<<"进程CPU使用率："<<systemResource.cpu.processCpu<< endl;
   cout<<"线程CPU使用率："<<systemResource.cpu.threadCpu<< endl;
   cout<<"---------内存-----------"<<endl;
   cout<<"系统总内存："<<systemResource.memory.systemTotalMem<<" MB"<<endl;
   cout<<"系统剩余内存："<<systemResource.memory.systemFreeMem <<" MB"<< endl;
   cout<<"系统内存使用率："<<systemResource.memory.systemMemUsage << endl;
   cout<<"进程内存使用率："<<systemResource.memory.processMemUsage << endl;
   cout<<"线程内存使用率："<<systemResource.memory.threadMemUsage << endl;

   cout<<"---------网络-----------"<<endl;
   cout<<"网卡一名称："<<systemResource.net.netInterOneName << endl;
   cout<<"网卡一上传速度："<<systemResource.net.netInterOneRtUpSpeed << " MB/S"<< endl;
   cout<<"网卡一下载速度："<<systemResource.net.netInterOneRtDownSpeed <<" MB/S"<< endl;
   cout<<"网卡一带宽使用率："<<systemResource.net.netInterOneBandwidthUsage <<endl;
   cout<<"网卡二名称："<<systemResource.net.netInterTwoName << endl;
   cout<<"网卡二上传速度："<<systemResource.net.netInterTwoRtUpSpeed <<" MB/S"<< endl;
   cout<<"网卡二下载速度："<<systemResource.net.netInterTwoRtDownSpeed <<" MB/S"<< endl;
   cout<<"网卡二带宽使用率："<<systemResource.net.netInterTwoBandwidthUsage <<endl;



}
