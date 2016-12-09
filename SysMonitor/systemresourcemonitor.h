#ifndef SYSTEMRESOURCEMONITOR_H
#define SYSTEMRESOURCEMONITOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define __DEBUG__ 1
#define CK_TIME 1
#define BUFFSIZE 128
#define VmRssLine 16
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/vfs.h>
#include <net/if.h>         //ifconf ifreq等结构体
#include <sys/ioctl.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <vector>
using namespace std;

typedef struct CPU
{
    CPU():MediaProcessID(0),threadID(0),systemCpu(0),processCpu(0),
        threadCpu(0), MediaProcessName(NULL)
    {

    }

    int MediaProcessID;                     //媒体进程ID
    int threadID;                           //线程ID
    float systemCpu;                        //系统CPU使用率
    float processCpu;                       //进程CPU使用率
    float threadCpu;                        //线程CPU使用率
    char* MediaProcessName;                 //媒体进程名称（获取ID用）

}cpu_t;

typedef struct cpuTime
{
    long totalCpuTime;                      //CPU总时间
    long idel;                              //CPU空闲时间
}cpuTime_t;

typedef struct MEMORY
{
    char* processName;                      //进程名称
    char state[2];                          //进程状态
    int processID;                          //进程ID
    int threadID;                           //线程ID
    float systemMemUsage;                   //系统内存使用率
    float processMemUsage;                  //进程内存使用率
    float threadMemUsage;                   //线程内存使用率
    long systemTotalMem;                    //系统总内存大小
    long systemFreeMem;                     //系统空闲内存大小
    long processUseMem;                     //进程占用内存大小
    long threadUseMem;                      //线程占用内存大小

}memory_t;

typedef struct NET
{
    char netInterOneName[20];               //网络接口一名称
    char netInterTwoName[20];               //网络接口二名称
    long netInterOneSpeedLevel;             //网络接口一速度级别
    long netInterTwoSpeedLevel;             //网络接口二速度级别
    double netInterOneRtUpSpeed;            //网络接口一实时上传速度
    double netInterTwoRtUpSpeed;            //网络接口二实时上传速度
    double netInterOneRtDownSpeed;          //网络接口一实时下载速度
    double netInterTwoRtDownSpeed;          //网络接口二实时下载速度
    float netInterOneBandwidthUsage;        //网络接口一带宽使用率
    float netInterTwoBandwidthUsage;        //网络接口二带宽使用率
}net_t;

typedef struct SYSTEMRESOURCE
{
    SYSTEMRESOURCE()
    {
        memset(&cpu, 0, sizeof(cpu));
        memset(&memory, 0, sizeof(memory));
        memset(&net, 0, sizeof(net));
    }

    cpu_t cpu;
    memory_t memory;
    net_t net;
}systemResource_t;

class SystemResourceMonitor
{
public:
    SystemResourceMonitor();

    //CPU
    /*please input process name and thread ID in struct cpu*/
    void GetSysProThrCpuUsage(cpu_t& cpu);                  //获取系统/进程/线程CPU利用率
    //内存
    /*please input process name and thread ID in struct memory*/
    void GetSysProThrMemUsage(memory_t& memoryInfo);        //获取系统系统内存空间/剩余大小/进程/线程内存使用率


    //网络
    void GetNetInterfaceInfo(net_t& net);                   //获取网络接口使用状况信息111

    //进程监控
    void MediaProcessMonitor();            //媒体进程监控/杀死/重启

private:
    vector<long> vect_r;                                    //存放10次磁盘读取速度
    vector<long> vect_w;                                    //存放十次磁盘写入速度
    int GetMediaProcessID(char* processName);               //根据进程名称获取进程ID
    void GetTotalCpuTimeAndIedl(cpuTime_t&cpuTAI);          //获取系统总CPU时间大小和空闲时间大小
    long GetTotalProcessTime(int processID);                //获取进程CPU总时间
    void GetProcessState(int processID,char state[2]);      //获取进程状态
    long GetTotalThreadTime(int processID,int threadID);    //获取线程CPU总时间
    void GetSystemMemInfo(memory_t& memoryInfo);            //获取系统内存总大小/剩余大小/使用率
    void GetProcessMemUsage(memory_t& memoryInfo);          //获取进程内存使用率
    void GetThreadMemUsage(memory_t& memoryInfo);           //获取线程内存使用率
    void GetSystemMemTotalAndFree(memory_t& memoryInfo);    //获取系统内存总大小和剩余大小
    void GetProcessUseMem(memory_t& memoryInfo);            //获取进程占用内存大小
    void GetThreadUseMem(memory_t& memoryInfo);             //获取线程占用内存大小
    void GetNetInterName(net_t& net);                       //获取网络接口名称
    long GetNetInterSpeedLevel(char* netInterName);         //获取网络接口速度级别
    long* GetNetInterRealTimeSpeed(char* netInterName);     //获取网络接口实时上传和下载速度
    void GetNetInterBandwidthUsage(net_t& net);             //获取网络接口带宽使用率
    void KillAndStartProcess(char* processName);            //杀死并重启进程
    bool StartProcess();                                                            //启动流媒体进程

    int count;       //连续启动失败的次数
};

#endif // SYSTEMRESOURCEMONITOR_H
