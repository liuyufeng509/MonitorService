#ifndef STREAMMONITOR_H
#define STREAMMONITOR_H
#include "../common.h"
#include <QObject>
#include<QList>
#include<QMap>
#include<QString>
/**
 * @brief The StreamMonitor class
 * 该类属于流媒体服务监控类，实现具体的流媒体服务逻辑监控，如CPU，内存等。
 * 此处只实现了简单的信息打印。
 */
class StreamMonitor : public QObject
{
    Q_OBJECT
    enum RcvMsgType
    {
        Rcv_Cameras_Info =1,       //所有的摄像机
        Rcv_Camera_Online=2,             //摄像机在线状态
        RcV_Threads_Info=3,                 //核心线程列表信息
        Rcv_HisFile_Info=4,                     //历史文件信息
        Rcv_Thread_Heart=5,                  //线程心跳
        Rcv_RelVd_Res=6,                        //实时视频调看结果
        Rcv_HisVd_Res=7,                        //历史视频调看结果
    };
    enum SendMsgType
    {
        Send_Disk_Info=5,

    };

public:
    explicit StreamMonitor(QObject *parent = 0);

    void checkHisFile(CameraStateInfo &camera);//检查历史文件是否有问题
    void sendDiskState();                       //硬盘有问题，返回消息
    void sendHisFileState();                    //历史文件有问题，返回消息
    void sendRelVdRecState();               //当前录制文件有问题，返回消息


    void monitorDiskInfo();

    void printInfo();
    void printDiskInfo();
signals:

public slots:
    void doParseXml(QString xml);
    void monitorCamera();

private:
    DiskStateInfo  diskInfo;                         //硬盘信息状态
    QList<CameraStateInfo> camerasInfo;//摄像机信息状态
    QList<ThreadStateInfo>  threadsInfo; //所有线程的状态信息
    QMap<int, QString>    diskErrInfoMap;       //存放错误码与错误信息的映射
    QMap<int, QString>    hisFileErrInfoMap;       //存放错误码与错误信息的映射
    QMap<int, QString>    relVdRecErrInfoMap;       //存放错误码与错误信息的映射
};

#endif // STREAMMONITOR_H
