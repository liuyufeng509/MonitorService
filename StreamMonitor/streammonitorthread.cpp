#include "streammonitorthread.h"
#include<iostream>

StreamMonitorThread::StreamMonitorThread(QObject *parent)
{

}
void StreamMonitorThread::init()
{
    //创建线程对象时，先把socket监听起来
    LocalServer::getInstance()->setServName(QReadConfig::getInstance()->getDomainSockConf().serverName);
    LocalServer::getInstance()->listen();
    LOG(INFO, "LocalServer 开始监听");

    LocalClient::getInstance()->setServName(QReadConfig::getInstance()->getDomainSockConf().clientName);
    LocalClient::getInstance()->requestConnection();
    LOG(INFO, "LocalClient 请求建立链接");
}

void StreamMonitorThread::run()
{
    init();
    streamMonitor = new StreamMonitor;
    CommandLine::getInstance()->setStreamMonitor(streamMonitor);            //便于访问监控数据结果
    //接收socketserver中的xml协议，转发到streamMonitor中进行解析处理
    connect(LocalServer::getInstance(), SIGNAL(emitData(QString)), streamMonitor, SLOT(doParseXml(QString)));

    //1.监听摄像机信息(磁盘是否正常、摄像机是否正在录制、历史文件是否正常，其中轮询监控的是前两项)
    cameraMonTimer = new QTimer;
    connect(cameraMonTimer, SIGNAL(timeout()), streamMonitor, SLOT(monitorCamera()));
    cameraMonTimer->start(QReadConfig::getInstance()->getCameraSvrConf().timer);

    //2.摄像机实时视频调看
    relAndHisVdReqTimer = new QTimer;
    connect(relAndHisVdReqTimer, SIGNAL(timeout()), streamMonitor, SLOT(monitorRelAndHisVdReq()));
    relAndHisVdReqTimer->start(QReadConfig::getInstance()->getRelAndHisVidConf().timer);

    //数据库状态监视
    dbMonitorTimer = new QTimer;
    connect(dbMonitorTimer, SIGNAL(timeout()), streamMonitor, SLOT(monitorDBStatus()));
    dbMonitorTimer->start(QReadConfig::getInstance()->getDBSvrConf().timer);

    //定时检测线程心跳
    threadHeartTimer = new QTimer;
    connect(threadHeartTimer, SIGNAL(timeout()), streamMonitor, SLOT(monitorThreads()));
    threadHeartTimer->start(QReadConfig::getInstance()->getThreadSvrConf().timer);

    //启动10秒后，向流媒体请求一次基本数据，目的是为了防止监控服务重启后，没有流媒体数据无法监控。
    getHbDataTimer = new QTimer;
    streamMonitor->setGetHbDataTimer(getHbDataTimer);
    connect(getHbDataTimer, SIGNAL(timeout()), streamMonitor, SLOT(getHbBaseData()));
    getHbDataTimer->start(10000);
    exec();
}

StreamMonitorThread::~StreamMonitorThread()
{
    delete cameraMonTimer;
    delete relAndHisVdReqTimer;
    delete streamMonitor;
    delete dbMonitorTimer;
    delete threadHeartTimer;
    delete getHbDataTimer;
}
