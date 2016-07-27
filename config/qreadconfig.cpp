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

    sysResConf.bOpen = configIniRead->value("SysResource/open").toBool();
    sysResConf.timer = configIniRead->value("SysResource/timer").toBool();

    diskConf.diskPaths = configIniRead->value("Disk/tmpfilepath").toString().split('#');
    diskConf.diskFileNames = configIniRead->value("Disk/diskfilename").toString().split('#');
    diskConf.minFreeSize = configIniRead->value("Disk/minSize").toInt();
    cameraSvrConf.bOpen = configIniRead->value("CameraSvr/open").toBool();
    cameraSvrConf.timer = configIniRead->value("CameraSvr/timer").toInt();
    cameraSvrConf.vdPath = configIniRead->value("CameraSvr/vdPath").toString();

    dbSvrConf.bOpen = configIniRead->value("DBSvr/open").toBool();
    dbSvrConf.timer = configIniRead->value("DBSvr/timer").toInt();
    dbSvrConf.dbName = configIniRead->value("DBSvr/dbname").toString();

    threadSvrConf.bOpen = configIniRead->value("ThreadSvr/open").toBool();
    threadSvrConf.timer = configIniRead->value("ThreadSvr/timer").toInt();

    relAndHisVideoConf.bOpen = configIniRead->value("RelAndHis/open").toBool();
    relAndHisVideoConf.timer = configIniRead->value("RelAndHis/timer").toInt();

    gsoapConf.soapEndpoint = configIniRead->value("gsoapInfo/soap_endpoint").toString();
    gsoapConf.sendTimeout = configIniRead->value("gsoapInfo/send_timeout").toInt();
    gsoapConf.recvTimeout = configIniRead->value("gsoapInfo/recv_timeout").toInt();
    gsoapConf.connTimeout = configIniRead->value("gsoapInfo/connect_timeout").toInt();
    gsoapConf.userID = configIniRead->value("gsoapInfo/userid").toString();
    gsoapConf.passwd = configIniRead->value("gsoapInfo/passwd").toString();

    logInfo.path = configIniRead->value("Log/path").toString();
    logInfo.size = configIniRead->value("Log/size").toInt();

    delete configIniRead;
}

SysResourceConfig   QReadConfig::getSysResConf()
{
    return sysResConf;
}

LogInfo QReadConfig::getLogInfo()
{
    return logInfo;
}

void QReadConfig::printInfo()
{
    LOG(INFO, "******************ProcessDogConfig*********************");
    qInfo()<<"procDogConf.bOpen="<<(procDogConf.bOpen?"true":"false");
    qInfo()<<"procDogConf.nTimer="<<procDogConf.nTimer;
    qInfo()<<"procDogConf.strProcName="<<procDogConf.strProcName;
    qInfo()<<"procDogConf.strPath="<<procDogConf.strPath;

    qInfo()<<"******************DomainSockConfig*********************";
    qInfo()<<"domainSockConf.servername="<<domainSockConf.serverName;
    qInfo()<<"domainSockConf.clientname="<<domainSockConf.clientName;
    qInfo()<<"domainSockConf.checkTimer="<<domainSockConf.checkTimer;

    qInfo()<<"******************SysResourceConfig*********************";
    qInfo()<<"sysResConf.bOpen="<<sysResConf.bOpen;
    qInfo()<<"sysResConf.nTimer="<<sysResConf.timer;

    qInfo()<<"******************CameraSvrConfig*********************";
    qInfo()<<"cameraSvrConf.bOpen="<<cameraSvrConf.bOpen;
    qInfo()<<"cameraSvrConf.nTimer="<<cameraSvrConf.timer;
    qInfo()<<"cameraSvrConf.vdPath="<<cameraSvrConf.vdPath;

    qInfo()<<"******************DBSvrConfig*********************";
    qInfo()<<"dbSvrConf.bOpen="<<dbSvrConf.bOpen;
    qInfo()<<"dbSvrConf.nTimer="<<dbSvrConf.timer;
    qInfo()<<"dbSvrConf.dbName="<<dbSvrConf.dbName;

    qInfo()<<"******************ThreadSvrConfig*********************";
    qInfo()<<"threadSvrConf.bOpen="<<threadSvrConf.bOpen;
    qInfo()<<"threadSvrConf.nTimer="<<threadSvrConf.timer;

    qInfo()<<"******************RelAndHisVideoConfig*********************";
    qInfo()<<"relAndHisVideoConf.bOpen="<<relAndHisVideoConf.bOpen;
    qInfo()<<"relAndHisVideoConf.nTimer="<<relAndHisVideoConf.timer;

    qInfo()<<"******************gsoapConf*********************";
    qInfo()<<"gsoapConf.soapEndpoint="<<gsoapConf.soapEndpoint;
    qInfo()<<"gsoapConf.sendTimeout="<<gsoapConf.sendTimeout;
    qInfo()<<"gsoapConf.recvTimeout="<<gsoapConf.recvTimeout;
    qInfo()<<"gsoapConf.connTimeout="<<gsoapConf.connTimeout;
    qInfo()<<"gsoapConf.userID="<<gsoapConf.userID;
    qInfo()<<"gsoapConf.passwd="<<gsoapConf.passwd;
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

DiskConfig       QReadConfig::getDiskCong()
{
    return diskConf;
}

RelAndHisVideoConfig QReadConfig::getRelAndHisVidConf()
{
    return relAndHisVideoConf;
}
DomainSockConfig     QReadConfig::getDomainSockConf()
{
    return domainSockConf;
}

GsoapConfig QReadConfig::getGsoapInfoConf()
{
    return gsoapConf;
}

QReadConfig::~QReadConfig()
{

}
