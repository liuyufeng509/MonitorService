#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include<iostream>
#include<QString>
#include<QStringList>
#include"StreamMonitor/streammonitor.h"
using namespace std;
class CommandLine
{
public:
    static CommandLine * getInstance()
    {
        if(m_pInstance == NULL)
        {
            m_pInstance = new CommandLine();
        }
        return m_pInstance;
    }

    void setStreamMonitor(StreamMonitor* pStreamMon)
    {
        streamMonitor= pStreamMon;
    }

private:
    CommandLine();

    static CommandLine * m_pInstance;

public:
    void printHelp();
    //bool checkCommand(QString command);
    void run();


    void lsPid(int pid);
    void lsDisk();
    void lsCamera();
    void lsDB();
    void lsThreads();

    void lsCPU();
    void lsMem();
    void lsNet();
    void lsRelVedio();
    void lsHisVedio();
private:
    QString         m_comm;
    QStringList     commList;
    StreamMonitor *streamMonitor;//便于访问监控数据结果
};

#endif // COMMANDLINE_H
