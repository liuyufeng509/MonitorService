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
public:
    explicit StreamMonitor(QObject *parent = 0);

    void printInfo();
    void monitorDiskInfo();

signals:

public slots:
    void doParseXml(QString xml);
    void monitorCamera();

private:
    DiskStateInfo  diskInfo;                         //硬盘信息状态
    QList<CameraStateInfo> cameraInfos;//摄像机信息状态
    QMap<int, QString>    errInfoMap;       //存放错误码与错误信息的映射
};

#endif // STREAMMONITOR_H
