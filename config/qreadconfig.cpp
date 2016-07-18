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

    delete configIniRead;
}

SysResourceConfig   QReadConfig::getSysResConf()
{
    return sysResConf;
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
    cout<<"domainSockConf.checkTimer="<<domainSockConf.checkTimer<<endl;

    cout<<"******************SysResourceConfig*********************"<<endl;
    cout<<"sysResConf.bOpen="<<sysResConf.bOpen<<endl;
    cout<<"sysResConf.nTimer="<<sysResConf.timer<<endl;

    cout<<"******************CameraSvrConfig*********************"<<endl;
    cout<<"cameraSvrConf.bOpen="<<cameraSvrConf.bOpen<<endl;
    cout<<"cameraSvrConf.nTimer="<<cameraSvrConf.timer<<endl;
    cout<<"cameraSvrConf.vdPath="<<cameraSvrConf.vdPath.toStdString()<<endl;

    cout<<"******************DBSvrConfig*********************"<<endl;
    cout<<"dbSvrConf.bOpen="<<dbSvrConf.bOpen<<endl;
    cout<<"dbSvrConf.nTimer="<<dbSvrConf.timer<<endl;
    cout<<"dbSvrConf.dbName="<<dbSvrConf.dbName.toStdString()<<endl;

    cout<<"******************ThreadSvrConfig*********************"<<endl;
    cout<<"threadSvrConf.bOpen="<<threadSvrConf.bOpen<<endl;
    cout<<"threadSvrConf.nTimer="<<threadSvrConf.timer<<endl;

    cout<<"******************RelAndHisVideoConfig*********************"<<endl;
    cout<<"relAndHisVideoConf.bOpen="<<relAndHisVideoConf.bOpen<<endl;
    cout<<"relAndHisVideoConf.nTimer="<<relAndHisVideoConf.timer<<endl;

    cout<<"******************gsoapConf*********************"<<endl;
    cout<<"gsoapConf.soapEndpoint="<<gsoapConf.soapEndpoint.toStdString()<<endl;
    cout<<"gsoapConf.sendTimeout="<<gsoapConf.sendTimeout<<endl;
    cout<<"gsoapConf.recvTimeout="<<gsoapConf.recvTimeout<<endl;
    cout<<"gsoapConf.connTimeout="<<gsoapConf.connTimeout<<endl;
    cout<<"gsoapConf.userID="<<gsoapConf.userID.toStdString()<<endl;
    cout<<"gsoapConf.passwd="<<gsoapConf.passwd.toStdString()<<endl;

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
