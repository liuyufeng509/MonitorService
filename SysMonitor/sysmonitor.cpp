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

    //硬盘文件和硬盘挂在路径数量肯定是一致的
    for(int i=0; i<QReadConfig::getInstance()->getDiskCong().diskPaths.size(); i++)
    {
        disk_t disk;
        disk.diskPach = new char[255];
        disk.fileSystem = new char[255];
        strcpy(disk.diskPach, QReadConfig::getInstance()->getDiskCong().diskPaths[i].toStdString().c_str());
        strcpy(disk.fileSystem, QReadConfig::getInstance()->getDiskCong().diskFileNames[i].toStdString().c_str());

        systemResource.disks.push_back(disk);
    }

    systemResourceMonitor = new SystemResourceMonitor;
}

SysMonitor::~SysMonitor()
{
    delete[] systemResource.cpu.MediaProcessName;
    delete[] systemResource.memory.processName;
    for(int i=0; i<systemResource.disks.size(); i++)
    {
        delete[] systemResource.disks[i].diskPach;
        delete[] systemResource.disks[i].fileSystem;
    }
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

   //磁盘
   /*输入磁盘挂在路径和文件系统名称（10秒获取一次 ）*/
   for(int i=0; i<systemResource.disks.size();i++)
   {
       systemResourceMonitor->GetSystemDiskInfo(systemResource.disks[i]);                   //获取磁盘阵列的使用状况信息
   }

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
   cout<<"---------磁盘-----------"<<endl;
   for(int i=0; i<systemResource.disks.size(); i++)
   {
       cout<<"磁盘总空间："<<systemResource.disks[i].totalSize <<" MB"<< endl;
       cout<<"磁盘剩余空间："<<systemResource.disks[i].freeSize <<" MB"<< endl;
       cout<<"磁盘使用率："<<systemResource.disks[i].usage << endl;
       cout<<"磁盘读取速度："<<systemResource.disks[i].readSpeed << " KB/S"<<endl;
       cout<<"磁盘写入速度："<<systemResource.disks[i].writeSpeed << " KB/S"<<endl;
   }

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
