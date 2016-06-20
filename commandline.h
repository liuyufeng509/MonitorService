#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include<iostream>
#include<QString>
#include<QStringList>

using namespace std;
class CommandLine
{
public:
    CommandLine();

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

};

#endif // COMMANDLINE_H
