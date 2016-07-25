#ifndef COMMON_H
#define COMMON_H
#include<QString>
#include<QStringList>
#include <QtDebug>
#include <QtGlobal>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/statfs.h>
#include <sys/vfs.h>
#include <errno.h>
//硬盘基本信息
struct DiskBaseInfo
{
    QString    mountPath;       //硬盘的挂载路径

    qint64 total_size ;      //硬盘总大小
    qint64 available_size;                //非root用户可以用的磁盘空间大小
    qint64 free_size;                                  //硬盘的所有剩余空间
    qint64 f_blocks;                                //块个数
};

//硬盘状态信息，根据此信息判断是否进行自恢复和通知流媒体服务
struct DiskStateInfo{
    enum DiskState{
        DISK_NORMAL,
        CAN_NOT_CREATE_FILE,
        CAN_NOT_WRITE_FILE,
        CAN_NOT_MOUNT,
        DISK_OVER_LOAD,
    };
    DiskBaseInfo baseInfo;
    DiskState state;        //暂时只有硬盘状态，其他的信息如读写速度等，后续资源监控时添加
};

//CPU 信息
struct CPUInfo
{
    float occupy;           //占用率
};

//Mem 信息
struct MemInfo
{
    //系统内存的属性
    long totalMem;           //内存大小  单位（K）
    long freeMem;            //空闲内存大小 单位（K）

    //进程内存属性
    long procVRTTotalMem;       //进程所占虚拟内存大小 单位（K）
    long procResTotalMem;       //进程所占实际物理内存大小 单位（K）

    //进程及系统内存属性
    float occpy;            //占用率
};

//系统资源信息
struct SysResource
{
    CPUInfo cpu;
    MemInfo mem;
    QList<DiskBaseInfo> disks;       //多个硬盘
};

//进程资源信息
struct ProcResource
{
    QString procName;
    CPUInfo cpu;
    MemInfo mem;
};

//历史文件是否正常
struct HisVdFileState
{
    enum HisVdState{
        NORMAL,
        UNNORMAL,
        NOT_EXIST,
        NOT_OPEN,
    };
    HisVdFileState():hisVdPath(""), state(NORMAL)
    {

    }
    QString hisVdPath;      //历史文件路径
    HisVdState  state;         //历史文件状态
};

//摄像机监控信息结构体：包括当前录制是否正常，历史文件是否录制正常
struct CameraStateInfo
{

    CameraStateInfo():cmareId(""),online(false),relVdSta(NORMAL),udpAddr("")
    {

    }
    bool operator==(const CameraStateInfo  &other) const
    {
        return this->cmareId==other.cmareId;
    }

    enum RelVdState{
        NORMAL,
        UNNORMAL,
        NOT_EXIST,
        NOT_ONLINE
    };
    QString  cmareId;
    QString  udpAddr;           //组播地址
    bool       online;
    int          relVdSta;      //正在录制的视频信息
    HisVdFileState hisVdSta;    //历史文件信息
};

//线程状态
struct ThreadStateInfo
{
    ThreadStateInfo():threadId(""),state(NORMAL), heartime(time(NULL))
    {

    }

    bool operator==(const ThreadStateInfo  &other) const
    {
        return this->threadId==other.threadId;
    }
    enum ThreadState{
        NORMAL, //正常
        BLOCK,      //阻塞
        Dead,         //死亡
    };
    QString     threadId;
    int             action;             //1 http线程 2：视频流线程 3：磁盘检测线程
    ThreadState state;

    time_t heartime;
};

//流媒体服务器历史视频和实时视频能否调看
struct RelAndHisVdReqStat
{
    RelAndHisVdReqStat():relVdReq(NORMAL), hisVdReq(NORMAL)
    {

    }

    enum{
        NORMAL=0,
        UNORMAL=1,
        SSLFAIL=110,
    };
    int             relVdReq;           //实时视频调看状态:0---实时视频调看成功  110--ssl证书加载失败 1--实时视频调看失败
    int             hisVdReq;
};

//数据库状态信息结构体
struct DBStatusInfo
{
    enum{
        NORMAL=0,
        DB_OPEN_FAIL=1,
        DB_LOST_DB_FILE=2,
        DB_TABLE_LOCKED=3,
    };
    int DBState;                    //数据库状态
    QString CameraID;       //数据文件对应的摄像机ID
};

enum                            //日志级别
{
    WARNING=1,
    DEBUG=2,
    GUZHANG=3,
    INFO=4,
};

#define LOG(type, msg)  \
    switch(type)    \
{                               \
    case WARNING:   \
        qWarning(msg);  \
        break;                  \
    case DEBUG:             \
        qDebug(msg);        \
        break;                      \
    case GUZHANG:       \
        qFatal(msg);        \
        break;                  \
    case INFO:              \
        qInfo(msg);         \
        break;                  \
}

QString getCwdPath();
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#define BUF_SIZE 1024
std::string getPidByName(char* task_name);  //根据进程名获取pid
int kill_spider_backgroud(char* task_name); //依据进程名，杀死进程

int getDiskInfo(char *path, DiskStateInfo &diskInfo);   //获取磁盘状态

void getSysResource(SysResource &sysRes);               //获取系统资源信息
void getProcResource(ProcResource &procRes);            //获取进程的资源信息

//获取cpu信息的结构体及函数
typedef struct procstat {
     char processorName[20];
     unsigned int user;
     unsigned int nice;
     unsigned int system;
     unsigned int idle;
     unsigned int iowait;
     unsigned int irq;
     unsigned int softirq;
     unsigned int stealstolen;
     unsigned int guest;

     long int utime;
     long int stime;
     long int cutime;
     long int cstime;
} Procstat;

void getCPUInfo(CPUInfo &cpu, char*procname="");    //获取cpu信息
void getProcCpuStats(Procstat & ps, int processID);//获取进程cpu信息
void  getCPUStatus(Procstat& ps);           //获取系统cpu信息
float calculateCPUUse(Procstat ps1, Procstat ps2);//计算系统cpu利用率
float calculateProcCPUUse(Procstat ps1, Procstat ps2);//计算进程cpu利用率


void getMemInfo(MemInfo &mem, char* procname="");         //获取mem信息

#endif // COMMON_H
