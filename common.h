#ifndef COMMON_H
#define COMMON_H
#include<QString>

//硬盘状态信息，根据此信息判断是否进行自恢复和通知流媒体服务
struct DiskStateInfo{
    enum DiskState{
        DISK_NORMAL,
        CAN_NOT_CREATE_FILE,
        CAN_NOT_MOUNT,
        DISK_OVER_LOAD,
    };
    DiskState state;        //暂时只有硬盘状态，其他的信息如读写速度等，后续资源监控时添加
};

//历史文件是否正常
struct HisVdFileState
{
    enum HisVdState{
        NORMAL,
        UNNORMAL
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

    CameraStateInfo():cmareId(""),online(false),relVdSta(NORMAL)
    {

    }
    bool operator==(const CameraStateInfo  &other) const
    {
        return this->cmareId==other.cmareId;
    }

    enum RelVdState{
        NORMAL,
        UNNORMAL
    };
    QString  cmareId;
    bool       online;
    int          relVdSta;      //正在录制的视频信息
    HisVdFileState hisVdSta;    //历史文件信息
};

//线程状态
struct ThreadStateInfo
{
    ThreadStateInfo():threadId(""),state(NORMAL)
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
};

#endif // COMMON_H
