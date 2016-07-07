#include "commandline.h"
#include<string>
#include<QRegExp>

CommandLine * CommandLine::m_pInstance=NULL;
CommandLine::CommandLine():m_comm("")
{
    commList<<"lsDisk"<<"lsCamera"<<"lsDB"<<"lsThreads"
              <<"lsCPU"<<"lsMem"<<"lsNet"<<"lsPID"<<"lsRelVedio"
                <<"lsHisVedio";
}

void CommandLine::printHelp()
{
    cout<<"*****************query command help*******************"<<endl;
    cout<<"lsDisk:      查询硬盘信息"<<endl;
    cout<<"lsCamera:    查询摄像机信息"<<endl;
    cout<<"lsDB:        查询数据库信息"<<endl;
    cout<<"lsThreads:   查询线程信息"<<endl;
    cout<<"lsCPU:       查询cpu信息"<<endl;
    cout<<"lsMem:       查询内存信息"<<endl;
    cout<<"lsNet:       查询网络信息"<<endl;
    cout<<"lsPID+pid:   查询pid进程信息，空格隔开"<<endl;
    cout<<"lsRelVedio:  查询实时视频调看"<<endl;
    cout<<"lsHisVedio:  查询历史视频调看"<<endl;
}
void CommandLine::lsPid(int pid)
{
    cout<<"list pid info:"<<pid<<endl;
}

void CommandLine::lsDisk()
{
    cout<<"**********list disk info************"<<endl;
    streamMonitor->printDiskInfo();
}

void CommandLine::lsCamera()
{
    cout<<"***********list camera info***********"<<endl;
    streamMonitor->printCameraInfo();
}

void CommandLine::lsDB()
{
    cout<<"list DataBase info"<<endl;
    streamMonitor->printDbStatusInfo();
}

void CommandLine::lsThreads()
{
    cout<<"list threads info"<<endl;
}

void CommandLine::lsCPU()
{
    cout<<"list cpu info"<<endl;
}

void CommandLine::lsMem()
{
    cout<<"list mem info"<<endl;
}

void CommandLine::lsNet()
{
    cout<<"list Net info"<<endl;
}

void CommandLine::lsRelVedio()
{
    cout<<"list RelVedio info"<<endl;
    streamMonitor->printRelVdReqInfo();
}

void CommandLine::lsHisVedio()
{
    cout<<"list lsHisVedio info"<<endl;
}

void CommandLine::run()
{
    forever{
        cout<<"请输入查询命令:"<<flush;
        string comm;
        getline(cin, comm);
        m_comm = QString::fromStdString(comm);

        if(m_comm.leftRef(5).compare("lsPID",Qt::CaseInsensitive)==0)
        {
            QStringList strLst = m_comm.split(QRegExp("\\s+"));
            if(strLst.count()<2)
            {
                cout<<"未输入进程id"<<endl;
            }else
            {
                int pid = strLst[1].toInt();
                lsPid(pid);
                continue;
            }
        }else if(m_comm.compare("lsDisk",Qt::CaseInsensitive)==0)
        {
            lsDisk();
            continue;
        }else if(m_comm.compare("lsCamera",Qt::CaseInsensitive)==0)
        {
            lsCamera();
            continue;
        }else if(m_comm.compare("lsDB",Qt::CaseInsensitive)==0)
        {
            lsDB();
            continue;
        }else if(m_comm.compare("lsThreads",Qt::CaseInsensitive)==0)
        {
            lsThreads();
            continue;
        }else if(m_comm.compare("lsCPU",Qt::CaseInsensitive)==0)
        {
            lsCPU();
            continue;
        }else if(m_comm.compare("lsMem",Qt::CaseInsensitive)==0)
        {
            lsMem();
            continue;
        }else if(m_comm.compare("lsNet",Qt::CaseInsensitive)==0)
        {
            lsNet();
            continue;
        }else if(m_comm.compare("lsRelVedio",Qt::CaseInsensitive)==0)
        {
            lsRelVedio();
            continue;
        }else if(m_comm.compare("lsHisVedio",Qt::CaseInsensitive)==0)
        {
            lsHisVedio();
            continue;
        }else{
            cout<<"非法命令，请重新输入查询命令。"<<endl;
        }

    }
}
