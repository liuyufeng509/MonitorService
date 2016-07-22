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
//硬盘状态信息，根据此信息判断是否进行自恢复和通知流媒体服务
struct DiskStateInfo{
    enum DiskState{
        DISK_NORMAL,
        CAN_NOT_CREATE_FILE,
        CAN_NOT_WRITE_FILE,
        CAN_NOT_MOUNT,
        DISK_OVER_LOAD,
    };
    QString    mountPath;       //硬盘的挂载路径
    DiskState state;        //暂时只有硬盘状态，其他的信息如读写速度等，后续资源监控时添加
    qint64 total_size ;      //硬盘总大小
    qint64 available_size;                //非root用户可以用的磁盘空间大小
    qint64 free_size;                                  //硬盘的所有剩余空间
    qint64 f_blocks;                                //块个数
};

//CPU信息

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

#endif // COMMON_H
