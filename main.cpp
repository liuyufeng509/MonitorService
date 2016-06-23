#include <QCoreApplication>
#include"config/qreadconfig.h"
#include<QProcess>
#include"SysMonitor/sysmonitorthread.h"
#include"StreamMonitor/streammonitorthread.h"
#include "commandline.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //读取配置文件
    QReadConfig::getInstance()->readConfigFile("/home/liuyufeng/MonitorService/config.ini");
//    QReadConfig::getInstance()->printInfo();

    //启动系统资源监控线程，进程狗应该是属于系统资源监控的一部分
//    SysMonitorThread sysMonitorThread;
//    sysMonitorThread.start();

    //启动业务逻辑监控线程
    StreamMonitorThread streamMonitorThread;
    streamMonitorThread.start();


    //命令行处理类
    CommandLine::getInstance()->printHelp();
    CommandLine::getInstance()->run();

//    qApp->quit();
//    QProcess::startDetached(qApp->applicationFilePath(), QStringList());  重启本身

    return a.exec();
}
