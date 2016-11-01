#ifndef IOSPEEDMONITORTHREAD_H
#define IOSPEEDMONITORTHREAD_H

#include<QThread>
#include"../config/qreadconfig.h"
class IOSpeedMonitorThread : public QThread
{
public:
    IOSpeedMonitorThread(QObject *parent=0)
    {

    }
    ~IOSpeedMonitorThread()
    {

    }



protected:
    void run();
signals:

private:

};

#endif // IOSPEEDMONITORTHREAD_H


