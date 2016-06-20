#ifndef STREAMMONITOR_H
#define STREAMMONITOR_H

#include <QObject>

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

signals:

public slots:
    void monitorCamera();
};

#endif // STREAMMONITOR_H
