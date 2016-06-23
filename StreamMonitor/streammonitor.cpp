#include "streammonitor.h"
#include <iostream>
#include "../config/qreadconfig.h"
#include<QFile>
#include <QTextStream>

using namespace std;

StreamMonitor::StreamMonitor(QObject *parent) : QObject(parent)
{
    errInfoMap.insert(DiskStateInfo::CAN_NOT_CREATE_FILE, "不能创建文件"); //向map里添加一对“键-值”
    errInfoMap.insert(DiskStateInfo::CAN_NOT_MOUNT, "硬盘未挂载"); //向map里添加一对“键-值”
    errInfoMap.insert(DiskStateInfo::DISK_OVER_LOAD, "硬盘满"); //向map里添加一对“键-值”
    errInfoMap.insert(DiskStateInfo::DISK_NORMAL, "硬盘状态正常"); //向map里添加一对“键-值”

    errInfoMap.insert(HisVdFileState::NORMAL, "历史文件正常"); //向map里添加一对“键-值”
    errInfoMap.insert(HisVdFileState::UNNORMAL, "历史文件异常"); //向map里添加一对“键-值”

    errInfoMap.insert(CameraStateInfo::NORMAL, "实时视频录制正常"); //向map里添加一对“键-值”
    errInfoMap.insert(CameraStateInfo::UNNORMAL, "实时视频录制异常"); //向map里添加一对“键-值”
}

void StreamMonitor::doParseXml(QString xml)
{
    cout<<"rcv xml msg:" <<endl;
    cout<<xml.toStdString()<<endl;
}


void StreamMonitor::monitorDiskInfo()
{
    //1.检测硬盘是否可以创建文件
    QFile file(QReadConfig::getInstance()->getDiskCong().tmpFilePath);
     if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))
     {
        cout<<"创建文件:"<<QReadConfig::getInstance()->getDiskCong().tmpFilePath.toStdString()<<"失败"<<endl;
        diskInfo.state = DiskStateInfo::CAN_NOT_CREATE_FILE;
        return;
     }else
     {
         QTextStream in(&file);
         in<<"this is a test file"<<"\n";
         file.close();
         QFile::remove(QReadConfig::getInstance()->getDiskCong().tmpFilePath);
         diskInfo.state = DiskStateInfo::DISK_NORMAL;
         return;
     }

     //2.检测硬盘是否挂载

     //3.检测硬盘是否满溢--调用张震接口

    ////////////////////////////////////////////////////////////
     diskInfo.state = DiskStateInfo::DISK_NORMAL;
     return;
}

void StreamMonitor::monitorCamera()
{
   if( QReadConfig::getInstance()->getCameraSvrConf().bOpen)
   {
        cout<<"monitoring DiskInfo First...."<<endl;
        monitorDiskInfo();
        cout<<"monitoring Camera info now"<<endl;
   }
   printInfo();
}

void StreamMonitor::printInfo()
{
    cout<<"diskInfo.state="<<errInfoMap.value(diskInfo.state).toStdString()<<endl;
}
