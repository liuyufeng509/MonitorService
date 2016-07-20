#include <QCoreApplication>
#include"config/qreadconfig.h"
#include<QProcess>
#include"SysMonitor/sysmonitorthread.h"
#include"StreamMonitor/streammonitorthread.h"
#include "commandline.h"
#include <QtDebug>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>

void init_daemon()
{
int pid;
int i;
pid=fork();
if(pid<0)
    exit(1);  //创建错误，退出
else if(pid>0) //父进程退出
    exit(0);

setsid(); //使子进程成为组长
pid=fork();
if(pid>0)
    exit(0); //再次退出，使进程不是组长，这样进程就不会打开控制终端
else if(pid<0)
    exit(1);

//关闭进程打开的文件句柄
for(i=0;i<NOFILE;i++)
    close(i);
chdir("/root/test");  //改变目录
umask(0);//重设文件创建的掩码
return;
}
int main(int argc, char *argv[])
{
     init_daemon();
    QCoreApplication a(argc, argv);

    //读取配置文件
    QReadConfig::getInstance()->readConfigFile("/home/liuyufeng/MonitorService/config.ini");
    QReadConfig::getInstance()->printInfo();

    qInstallMessageHandler(outputMessage);

    LOG(WARNING, "Start MonitorService");

    //启动系统资源监控线程，进程狗应该是属于系统资源监控的一部分
    SysMonitorThread sysMonitorThread;
    sysMonitorThread.start();


    //启动业务逻辑监控线程
  //  StreamMonitorThread streamMonitorThread;
 //   streamMonitorThread.start();


    //命令行处理类
//    CommandLine::getInstance()->printHelp();
  //  CommandLine::getInstance()->run();

//    qApp->quit();
//    QProcess::startDetached(qApp->applicationFilePath(), QStringList());  重启本身

    return a.exec();
}
