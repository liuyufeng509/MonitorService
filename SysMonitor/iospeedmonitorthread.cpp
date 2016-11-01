#include "iospeedmonitorthread.h"
#include<iostream>
#include <QFile>
#include <QMutex>
#include<stdio.h>
using namespace std;
extern QList<DiskIOSpeedList> g_speedList;
extern QMutex mutex;
QString GetSpeed(QString cmd)
{
    FILE *pf;
    char buff[100]={};
    if(isDebug)
        cout<<cmd.toStdString().c_str()<<endl;
    pf = popen(cmd.toStdString().c_str(), "r");
    fread(buff,sizeof(buff),sizeof(buff),pf);
    pclose(pf);
    return QString(buff);
}

void IOSpeedMonitorThread::run()
{
    LOG(INFO,"Getting IO Speed start!");
    g_speedList.clear();
    for(int i=0; i<QReadConfig::getInstance()->getDiskCong().diskPaths.size(); i++)
    {
        DiskIOSpeedList disklist;
        disklist.mountPath = QReadConfig::getInstance()->getDiskCong().diskPaths[i];
        if(disklist.mountPath.right(1)!="/")
            disklist.mountPath += "/";
        g_speedList.append(disklist);
    }
    while(1)
    {
        mutex.lock();
        for(int i=0; i<g_speedList.size();i++)
        {
            QString cmd = "LANG=zh_US.UTF-8 && dd if=/dev/zero of="+g_speedList[i].mountPath+"test bs=8k count=35000 2>&1 | sed -n \"3,1p\" | awk  \'{print $8 \" \" $9}\'";
            QString cmd2 = "LANG=zh_US.UTF-8 && dd if="+g_speedList[i].mountPath+"test of=/dev/null bs=8k count=55000 2>&1 | sed -n \"3,1p\" | awk  \'{print $8 \" \" $9}\'";
            DiskIOSpeed ioSpeed;
            ioSpeed.wSpeed = GetSpeed(cmd);
            ioSpeed.rSpeed = GetSpeed(cmd2);
            qInfo()<<"ioSpeed.wSpeed.toStdString().c_str()="<<ioSpeed.wSpeed;
            qInfo()<<"ioSpeed.rSpeed.toStdString().c_str()="<<ioSpeed.rSpeed;
            if(ioSpeed.wSpeed.length()!=0 && ioSpeed.rSpeed.length()!=0)
                g_speedList[i].PushSpeedInfo(ioSpeed);
        }
         mutex.unlock();
         sleep(1);
    }
}


