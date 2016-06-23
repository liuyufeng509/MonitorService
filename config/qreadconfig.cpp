#include "qreadconfig.h"
#include<iostream>
#include<QTextCodec>
using namespace std;

QReadConfig* QReadConfig::m_pInstance = NULL;

void QReadConfig::readConfigFile(QString filePath)
{
    configIniRead = new QSettings(filePath, QSettings::IniFormat);

    //configIniRead->setIniCodec(QTextCodec::codecForName("UTF-8"));
    procDogConf.bOpen = configIniRead->value("processDog/open").toBool();
    procDogConf.nTimer = configIniRead->value("processDog/timer").toInt();
    procDogConf.strProcName = configIniRead->value("processDog/name").toString();
    procDogConf.strPath = configIniRead->value("processDog/path").toString();

    domainSockConf.serverName = configIniRead->value("DoMainSock/sername").toString();
    domainSockConf.clientName = configIniRead->value("DoMainSock/clientname").toString();
    domainSockConf.checkTimer = configIniRead->value("DoMainSock/checktime").toInt();

    cameraSvrConf.bOpen = configIniRead->value("CameraSvr/open").toBool();
    cameraSvrConf.timer = configIniRead->value("CameraSvr/timer").toInt();

    dbSvrConf.bOpen = configIniRead->value("DBSvr/open").toBool();
    dbSvrConf.timer = configIniRead->value("DBSvr/timer").toInt();

    threadSvrConf.bOpen = configIniRead->value("ThreadSvr/open").toBool();
    threadSvrConf.timer = configIniRead->value("ThreadSvr/timer").toInt();

    cpuConf.bOpen = configIniRead->value("CPU/open").toBool();
    cpuConf.timer = configIniRead->value("CPU/timer").toInt();

    memConf.bOpen = configIniRead->value("MEM/open").toBool();
    memConf.timer = configIniRead->value("MEM/timer").toInt();

    diskConf.bOpen = configIniRead->value("Disk/open").toBool();
    diskConf.timer = configIniRead->value("Disk/timer").toInt();
    diskConf.tmpFilePath = configIniRead->value("Disk/tmpfilepath").toString();

    netConf.bOpen = configIniRead->value("Net/open").toBool();
    netConf.timer = configIniRead->value("Net/timer").toInt();

    procConf.bOpen = configIniRead->value("Process/open").toBool();
    procConf.timer = configIniRead->value("Process/timer").toInt();

    relAndHisVideoConf.bOpen = configIniRead->value("RelAndHis/open").toBool();
    relAndHisVideoConf.timer = configIniRead->value("RelAndHis/timer").toInt();

    delete configIniRead;
}

void QReadConfig::printInfo()
{
    cout<<"******************ProcessDogConfig*********************"<<endl;
    cout<<"procDogConf.bOpen="<<procDogConf.bOpen<<endl;
    cout<<"procDogConf.nTimer="<<procDogConf.nTimer<<endl;
    cout<<"procDogConf.strProcName="<<procDogConf.strProcName.toStdString()<<endl;
    cout<<"procDogConf.strPath="<<procDogConf.strPath.toStdString()<<endl;

    cout<<"******************DomainSockConfig*********************"<<endl;
    cout<<"domainSockConf.servername="<<domainSockConf.serverName.toStdString()<<endl;
    cout<<"domainSockConf.clientname="<<domainSockConf.clientName.toStdString()<<endl;

    cout<<"******************CameraSvrConfig*********************"<<endl;
    cout<<"cameraSvrConf.bOpen="<<cameraSvrConf.bOpen<<endl;
    cout<<"cameraSvrConf.nTimer="<<cameraSvrConf.timer<<endl;

    cout<<"******************DBSvrConfig*********************"<<endl;
    cout<<"dbSvrConf.bOpen="<<dbSvrConf.bOpen<<endl;
    cout<<"dbSvrConf.nTimer="<<dbSvrConf.timer<<endl;

    cout<<"******************ThreadSvrConfig*********************"<<endl;
    cout<<"threadSvrConf.bOpen="<<threadSvrConf.bOpen<<endl;
    cout<<"threadSvrConf.nTimer="<<threadSvrConf.timer<<endl;

    cout<<"******************CPUConfig*********************"<<endl;
    cout<<"cpuConf.bOpen="<<cpuConf.bOpen<<endl;
    cout<<"cpuConf.nTimer="<<cpuConf.timer<<endl;

    cout<<"******************MEMConfig*********************"<<endl;
    cout<<"memConf.bOpen="<<memConf.bOpen<<endl;
    cout<<"memConf.nTimer="<<memConf.timer<<endl;

    cout<<"******************DiskConfig*********************"<<endl;
    cout<<"diskConf.bOpen="<<diskConf.bOpen<<endl;
    cout<<"diskConf.nTimer="<<diskConf.timer<<endl;

    cout<<"******************NetConfig*********************"<<endl;
    cout<<"netConf.bOpen="<<netConf.bOpen<<endl;
    cout<<"netConf.nTimer="<<netConf.timer<<endl;

    cout<<"******************ProcessConfig*********************"<<endl;
    cout<<"procConf.bOpen="<<procConf.bOpen<<endl;
    cout<<"procConf.nTimer="<<procConf.timer<<endl;

    cout<<"******************RelAndHisVideoConfig*********************"<<endl;
    cout<<"relAndHisVideoConf.bOpen="<<relAndHisVideoConf.bOpen<<endl;
    cout<<"relAndHisVideoConf.nTimer="<<relAndHisVideoConf.timer<<endl;

}

ProcessDogConfig QReadConfig::getProcDogConf()
{
    return procDogConf;
}

CameraSvrConfig  QReadConfig::getCameraSvrConf()
{
    return cameraSvrConf;
}

DBSvrConfig      QReadConfig::getDBSvrConf()
{
    return dbSvrConf;
}

ThreadSvrConfig  QReadConfig::getThreadSvrConf()
{
    return threadSvrConf;
}
CPUConfig        QReadConfig::getCPUConf()
{
    return cpuConf;
}
MEMConfig        QReadConfig::getMemConf()
{
    return memConf;
}
DiskConfig       QReadConfig::getDiskCong()
{
    return diskConf;
}
NetConfig        QReadConfig::getNetConfig()
{
    return netConf;
}
ProcessConfig    QReadConfig::getProcConf()
{
    return procConf;
}
RelAndHisVideoConfig QReadConfig::getRelAndHisVidConf()
{
    return relAndHisVideoConf;
}
DomainSockConfig     QReadConfig::getDomainSockConf()
{
    return domainSockConf;
}

QReadConfig::~QReadConfig()
{

}
