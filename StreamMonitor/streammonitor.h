#ifndef STREAMMONITOR_H
#define STREAMMONITOR_H
#include "../common.h"
#include <QObject>
#include<QList>
#include<QMap>
#include<QString>
#include<QTimer>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkReply>

/**
 * @brief The StreamMonitor class
 * 该类属于流媒体服务监控类，实现具体的流媒体服务逻辑监控。
 *
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
        Rcv_YUNWEI_Info=8,                      //运维中心的ip port以及uuid信息
    };
    enum SendMsgType
    {
        Send_Thread_Info=1,
        Send_DB_Info=2,
        Send_HisVd_Req=4,               //历史视频调看
        Send_Disk_Info=5,
        Send_Rec_State=6,               //录制状态
        Send_GetData_Req=7,
    };

public:
    explicit StreamMonitor(QObject *parent = 0);

    void checkHisFile(CameraStateInfo &camera);//检查历史文件是否有问题
    void checkHisURL(QString httpUrl);  //调看该url对应的历史视频

    bool sendMsg(QString xml);
    void sendDiskState();                       //硬盘有问题，返回消息
    void sendHisFileState();                    //历史文件有问题，返回消息,此处先保留，上报运维即可
    void sendRelVdRecState(QString cameraId);               //当前录制文件有问题，返回消息
    void sendHisVdRequest(QString cameraId);               //发送历史视频调看请求
    void sendDbStatus();                                                    //发送数据库状态到流媒体服务
    void sendThreadInfo(const ThreadStateInfo& thread);                                                 //发送线程监控结果给流媒体


    QString getFileName(QString cmeraId);                       //读取数据库，获取当前录制的视频文件路径(索引中距离当前时间点最近的记录即当前录制的文件)

    void monitorDiskInfo();
    int monitorRelVdWithGsoap(const CameraStateInfo &camera);   //0---实时视频调看成功  110--ssl证书加载失败 1--实时视频调看失败

    void printInfo();
    void printDiskInfo();
    void printCameraInfo();
    void printRelVdReqInfo();
    void printHisVdReqInfo();
    void printDbStatusInfo();
    void printThreadsInfo();

    void setGetHbDataTimer(QTimer *timer){getHbTimer = timer;}
signals:

public slots:
    void doParseXml(QString xml);
    void monitorCamera();                       //摄像机状态监控
    void monitorRelAndHisVdReq();       //实时视频和历史视频调看监控
    void monitorDBStatus();                     //数据库状态监视
    void monitorThreads();                      //监视线程状态

    void relVdReqWithTimer();               //利用定时器重复请求实时视频调看,达到3次或者成功为止
    void hisVdReqWithTimer();               //利用定时器重复请求历史视频调看,达到3次或者成功为止

    void SendALLToOM();

    void getHbBaseData();                   //发送获取数据消息给流媒体
private slots:
    void httpReadyRead();//接受到数据时的处理
    void httpFinished();//完成下载后处理
private:
    QList<DiskStateInfo>  diskInfos;                         //硬盘信息状态
    QList<CameraStateInfo> camerasInfo;         //摄像机信息状态
    QList<ThreadStateInfo>  threadsInfo;            //所有线程的状态信息
    RelAndHisVdReqStat        relAndHisVdReqStat;       //流媒体实时和历史视频调看状态
    QList<DBStatusInfo>                    dbStatusInfos;                   //存放数据库的状态
    QMap<int, QString>    diskErrInfoMap;       //存放错误码与错误信息的映射
    QMap<int, QString>    hisFileErrInfoMap;       //存放错误码与错误信息的映射
    QMap<int, QString>    relVdRecErrInfoMap;       //存放错误码与错误信息的映射
    QMap<int, QString>    relHisVdReqErrInfoMap;       //存放错误码与错误信息的映射
    QMap<int, QString>    DbStatusErrInfoMap;           //存放错误码与错误信息的映射
    QMap<int, QString>    threadErrorInfoMap;           //存放错误码与错误信息的映射
    //视频调看相关,请求失败，则需要请求3次
    int   relReqCount;          //实时视频请求调看的次数
    QTimer * relRqTimer;        //实时视频请求调看计时器

    //QNetworkAccessManager类用于发送网络请求和接受回复，
    //具体的，它是用QNetworkRequest 类来管理请求，
    //QNetworkReply类进行接收回复，并对数据进行处理。
    QNetworkAccessManager * manager;
    QNetworkReply * reply;
    QNetworkRequest qheader;

    int   hisReqCount;               //历史视频请求调看的次数
    QTimer * hisRqTimer;        //历史视频请求调看计时器

    QTimer *sendToOMTimer;      //初始化运维后，触发该计时器，收集所有状态，上报运维。
    QTimer *getHbTimer;
    //清空所有数据（HbMedia 进程不存在时)
    void clearAll();

};

#endif // STREAMMONITOR_H
