#ifndef QREADCONFIG_H
#define QREADCONFIG_H
#include<QSettings>
#include"datadefine.h"
/***
 * QReadConfig类：
 * 该类是一个单例模式，负责读取并存储流媒体监控服务的配置信息。
 *
*/

class QReadConfig
{
public:
    static QReadConfig*  getInstance()
    {
        if(m_pInstance == NULL)
        {
            m_pInstance = new QReadConfig();
        }
        return m_pInstance;
    }

    //获取各个配置项信息
    ProcessDogConfig getProcDogConf();
    CameraSvrConfig  getCameraSvrConf();
    DBSvrConfig      getDBSvrConf();
    ThreadSvrConfig  getThreadSvrConf();
    CPUConfig        getCPUConf();
    MEMConfig        getMemConf();
    DiskConfig       getDiskCong();
    NetConfig        getNetConfig();
    ProcessConfig    getProcConf();
    RelAndHisVideoConfig getRelAndHisVidConf();
    DomainSockConfig     getDomainSockConf();

    void readConfigFile(QString filePath);  //读取配置文件

    void printInfo();       //打印配置信息

    ~QReadConfig();

private:
    QReadConfig():configIniRead(NULL)
    {

    }

    static QReadConfig *m_pInstance;        //单例模式

    QSettings *configIniRead;//负责ini文件的读取

    //配置项结构体
    ProcessDogConfig        procDogConf;
    CameraSvrConfig         cameraSvrConf;
    DBSvrConfig             dbSvrConf;
    ThreadSvrConfig         threadSvrConf;
    CPUConfig               cpuConf;
    MEMConfig               memConf;
    DiskConfig              diskConf;
    NetConfig               netConf;
    ProcessConfig           procConf;
    RelAndHisVideoConfig    relAndHisVideoConf;
    DomainSockConfig        domainSockConf;
};

#endif // QREADCONFIG_H
