#include "sysmonitor.h"
#include <iostream>
#include "../config/qreadconfig.h"
SysMonitor::SysMonitor(QObject *parent) : QObject(parent)
{
    systemResourceMonitor = new SystemResourceMonitor;
    procRes.procName = QReadConfig::getInstance()->getProcDogConf().strProcName;
    for(int i=0; i<QReadConfig::getInstance()->getDiskCong().diskPaths.size(); i++)
    {
        DiskBaseInfo disk;
        disk.mountPath = QReadConfig::getInstance()->getDiskCong().diskPaths[i];
        sysRes.disks.append(disk);
    }
}

SysMonitor::~SysMonitor()
{
    delete systemResourceMonitor;
}

void SysMonitor::monitorProcess()
{
    LOG(INFO,"进程狗启动");
    systemResourceMonitor->MediaProcessMonitor();
}

void SysMonitor::monitorSysResource()
{
    getSysResource(sysRes);
    getProcResource(procRes);

    LOG(INFO, "*************系统资源***************");
    LOG(INFO, ("系统CPU占用率:"+QString("%1").arg(sysRes.cpu.occupy)+"%").toStdString().c_str());
    LOG(INFO, ("系统全部内存:"+QString::number(sysRes.mem.totalMem>>10)+"M").toStdString().c_str());
    LOG(INFO, ("系统空闲内存:"+QString::number(sysRes.mem.freeMem>>10)+"M").toStdString().c_str());
    LOG(INFO, ("系统内存占用率:"+QString("%1").arg(sysRes.mem.occpy)+"%").toStdString().c_str());
    for(int i=0; i<sysRes.disks.size(); i++)
    {
        LOG(INFO, ("硬盘："+QString::number(i+1)+" 路径："+sysRes.disks[i].mountPath).toStdString().c_str());
        LOG(INFO, ("总大小："+QString::number(sysRes.disks[i].total_size>>20)+"M").toStdString().c_str());
        LOG(INFO, ("非root可用大小："+QString::number(sysRes.disks[i].available_size>>20)+"M").toStdString().c_str());
        LOG(INFO, ("空闲："+QString::number(sysRes.disks[i].free_size>>20)+"M").toStdString().c_str());
        LOG(INFO, ("块个数："+QString::number(sysRes.disks[i].f_blocks>>20)+"M").toStdString().c_str());
        qInfo()<<"读速度:"<<sysRes.disks[i].rSpeed<<"  写速度:"<<sysRes.disks[i].wSpeed;
    }

    LOG(INFO, "*************进程资源***************");
    LOG(INFO, ("进程名称:"+procRes.procName).toStdString().c_str());
    LOG(INFO, ("进程CPU占用率:"+QString("%1").arg(procRes.cpu.occupy)+"%").toStdString().c_str());
    LOG(INFO, ("进程虚拟内存:"+QString::number(procRes.mem.procVRTTotalMem>>10)+"M").toStdString().c_str());
    LOG(INFO, ("进程物理内存:"+QString::number(procRes.mem.procResTotalMem)+"K").toStdString().c_str());
    LOG(INFO, ("进程内存占用率:"+QString("%1").arg(procRes.mem.occpy)+"%").toStdString().c_str());

}
