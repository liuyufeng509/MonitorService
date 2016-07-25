#include "common.h"
#include<QMutex>
#include<QDateTime>
#include<QFile>
#include<QDir>
#include"config/qreadconfig.h"
#include <unistd.h>
#include <iostream>
using namespace std;
#define MAX_SIZE  255
QString getCwdPath()
{
    char current_absolute_path[MAX_SIZE];
    int cnt = readlink("/proc/self/exe", current_absolute_path, MAX_SIZE);
    if (cnt < 0 || cnt >= MAX_SIZE)
    {
        printf("***Error***\n");
        exit(-1);
    }

    int i;
    for (i = cnt; i >=0; --i)
    {
        if (current_absolute_path[i] == '/')
        {
            current_absolute_path[i+1] = '\0';
            break;
        }
    }
    return QString(current_absolute_path);
}

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(current_date).arg(context_info).arg(msg);


    QString path = getCwdPath();
    QFile file(path+"log.txt");
    QFileInfo fileInfo(file);
    if(fileInfo.size()>(QReadConfig::getInstance()->getLogInfo().size<<20))
    {
        QFile::remove(file.fileName());
    }else
    {
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << message << "\r\n";
        file.flush();
        file.close();
    }
    mutex.unlock();
}

std::string getPidByName(char* task_name)
{
    DIR *dir;
    struct dirent *ptr;
    char pidstr[256];
    FILE *fp;
    char filepath[50];//大小随意，能装下cmdline文件的路径即可
    char cur_task_name[50];//大小随意，能装下要识别的命令行文本即可
    char buf[BUF_SIZE];
    dir = opendir("/proc"); //打开路径
    if (NULL != dir)
    {
        while ((ptr = readdir(dir)) != NULL) //循环读取路径下的每一个文件/文件夹
        {
            //如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (DT_DIR != ptr->d_type)
                continue;

            sprintf(filepath, "/proc/%s/status", ptr->d_name);//生成要读取的文件的路径
            fp = fopen(filepath, "r");//打开文件
            if (NULL != fp)
            {
                if( fgets(buf, BUF_SIZE-1, fp)== NULL ){
                    fclose(fp);
                    continue;
                }
                sscanf(buf, "%*s %s", cur_task_name);

                //如果文件内容满足要求则打印路径的名字（即进程的PID）
                if (!strcmp(task_name, cur_task_name))
                {
                    printf("PID:  %s\n", ptr->d_name);
                    strcpy(pidstr, ptr->d_name);
                }

                fclose(fp);
            }

        }
        closedir(dir);//关闭路径
    }
    return std::string(pidstr);
}

int kill_spider_backgroud(char* task_name)
{
      /* send signal SIGTERM to kill */
    std::string pidstr =getPidByName(task_name);
    int pid = atoi(pidstr.c_str());
   if(pid > 0)
   {
       kill(pid, SIGTERM);
   }
   return 0;
}


int getDiskInfo(char *path, DiskStateInfo &diskInfo)
{
    struct statfs disk_info;
    int ret = 0;

    if ((ret=statfs(path, &disk_info)) == -1)
    {
      fprintf(stderr, "Failed to get file disk infomation,\
          errno:%u, reason:%s\n", errno, strerror(errno));
      return -1;
    }
    long long total_size = disk_info.f_blocks * disk_info.f_bsize;
    long long available_size = disk_info.f_bavail * disk_info.f_bsize;
    long long free_size = disk_info.f_bfree * disk_info.f_bsize;
    diskInfo.baseInfo.total_size = total_size;
    diskInfo.baseInfo.available_size = available_size;
    diskInfo.baseInfo.free_size = free_size;
    diskInfo.baseInfo.f_blocks = disk_info.f_blocks;
    diskInfo.baseInfo.mountPath = QString(path);

    //输出每个块的长度，linux下内存块为4KB
    printf("block size: %ld bytes\n", disk_info.f_bsize);
    //输出块个数
    printf("total data blocks: %ld \n", disk_info.f_blocks);
    //输出path所在磁盘的大小
    printf("total file disk size: %d MB\n",total_size >> 20);
    //输出非root用户可以用的磁盘空间大小
    printf("avaiable size: %d MB\n",available_size >> 20);
    //输出硬盘的所有剩余空间
    printf("free size: %d MB\n",free_size >> 20);
    //输出磁盘上文件节点个数
    printf("total file nodes: %ld\n", disk_info.f_files);
    //输出可用文件节点个数
    printf("free file nodes: %ld\n", disk_info.f_ffree);
    //输出文件名最大长度
    printf("maxinum length of file name: %ld\n", disk_info.f_namelen);

    return 0;
}

void getSysResource(SysResource &sysRes)
{
    LOG(INFO,"监控系统资源");
    getCPUInfo(sysRes.cpu);
    getMemInfo(sysRes.mem);
    for(int i=0; i<sysRes.disks.size(); i++)
    {
        DiskStateInfo disk;
        getDiskInfo((char*)sysRes.disks[i].mountPath.toStdString().c_str(), disk);
        sysRes.disks[i] = disk.baseInfo;
    }
}

void getProcResource(ProcResource &procRes)
{
    LOG(INFO,"监控进程资源");
    getCPUInfo(procRes.cpu, (char*)procRes.procName.toStdString().c_str());
    getMemInfo(procRes.mem, (char*)procRes.procName.toStdString().c_str());
}

void getProcCpuStats(Procstat & ps, int processID)
{
    //cout<<processID<<endl;
    FILE *fp;
    char buf[256];
    char comm[20];
    char task_state[2];

    long int pid,ppid,pgid,sid,tty_nr,tty_pgrp,task_flags,min_flt,cmin_flt,maj_flt,cmaj_flt;

    //processID int to string
    string str;
    char t[256];
    sprintf(t, "%d", processID);
    str = t;

    string fileName = "/proc/" + str +"/stat";
    //cout<<fileName<<":";
    char *p =(char*)fileName.data();

    //open process file
    fp = fopen(p,"r");
    if(fp == NULL)
    {
        perror("fopen:process file");
        return ;
    }

    fgets(buf,sizeof(buf),fp);

    sscanf(buf,"%ld%s%s%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld",&pid,comm,task_state,&ppid,&pgid,&sid,&tty_nr,&tty_pgrp,&task_flags,&min_flt,&cmin_flt,&maj_flt,&cmaj_flt,&ps.utime,&ps.stime,&ps.cutime,&ps.cstime);
}
void getCPUStatus(Procstat& ps) {
     // Get "/proc/stat" info.
     FILE* inputFile = NULL;

     chdir("/proc");
     inputFile = fopen("stat", "r");
     if (!inputFile) {
          perror("error: Can not open file.\n");
     }

     char buff[1024];
     fgets(buff, sizeof(buff), inputFile); // Read 1 line.
     printf(buff);
     sscanf(buff, "%s %u %u %u %u %u %u %u %u %u", ps.processorName, &ps.user, &ps.nice, &ps.system, &ps.idle, &ps.iowait, &ps.irq, &ps.softirq, &ps.stealstolen, &ps.guest); // Scan from "buff".
     printf("user: %u\n", ps.user);

     fclose(inputFile);
}
float calculateCPUUse(Procstat ps1, Procstat ps2) {
     unsigned int totalCPUTime = (ps2.user + ps2.nice + ps2.system + ps2.idle + ps2.iowait + ps2.irq + ps2.softirq + ps2.stealstolen + ps2.guest) - (ps1.user + ps1.nice + ps1.system + ps1.idle + ps1.iowait + ps1.irq + ps1.softirq + ps1.stealstolen + ps1.guest);
     unsigned int idleCPUTime = ps2.idle - ps1.idle;

     float CPUUse = ((float) totalCPUTime - (float) idleCPUTime) / (float) totalCPUTime;

    // printf("totalCPUTime: %u\nidleCPUTime: %u\n", totalCPUTime, idleCPUTime);

     return CPUUse;
}
float calculateProcCPUUse(Procstat ps1, Procstat ps2) {
     unsigned int totalCPUTime = (ps2.user + ps2.nice + ps2.system + ps2.idle + ps2.iowait + ps2.irq + ps2.softirq + ps2.stealstolen + ps2.guest) -
             (ps1.user + ps1.nice + ps1.system + ps1.idle + ps1.iowait + ps1.irq + ps1.softirq + ps1.stealstolen + ps1.guest);
     unsigned int procTotalTime = (ps2.utime+ps2.stime+ps2.cutime+ps2.cstime)-(ps1.utime+ps1.stime+ps1.cutime+ps1.cstime);

     unsigned int totalCPUTimePS2 = (ps2.user + ps2.nice + ps2.system + ps2.idle + ps2.iowait + ps2.irq + ps2.softirq + ps2.stealstolen + ps2.guest);
     unsigned int totalCPUTimePS1 = (ps1.user + ps1.nice + ps1.system + ps1.idle + ps1.iowait + ps1.irq + ps1.softirq + ps1.stealstolen + ps1.guest);
     float CPUUse = ((float)procTotalTime) / (float) totalCPUTime;
     printf("totalCPUTime: %u\procTotalTime: %u ProcCPUUSE:%f\n", totalCPUTime, procTotalTime,CPUUse);
    printf("totalCPUTimePS2: %u\totalCPUTimePS1: %u \n", totalCPUTimePS2, totalCPUTimePS1);
     return CPUUse;
}

void getCPUInfo(CPUInfo &cpu, char *procname)
{
    Procstat ps1, ps2;
    getCPUStatus(ps1);
    sleep(1);
    getCPUStatus(ps2);
    cpu.occupy = calculateCPUUse(ps1, ps2)*100;
    if(strlen(procname)==0)
    {
        return;
    }else
    {
        std::string pid = getPidByName(procname);
        getProcCpuStats(ps1, atoi(pid.c_str()));
        sleep(1);
        getProcCpuStats(ps2, atoi(pid.c_str()));
        cpu.occupy=calculateProcCPUUse(ps1, ps2)*100;
    }

}

void getMemInfo(MemInfo &mem, char* procname)         //获取mem信息
{
    FILE* fp;
    char buf[100];
    char name[20];
    long memTotalSize;
    long memFreeSize;
    fp = fopen("/proc/meminfo","r");
    if(fp == NULL)
    {
        perror("fopen:");
        LOG(WARNING, "打不开系统内存文件");
        exit (0);
    }

    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%ld",name,&memTotalSize);
    mem.totalMem= memTotalSize;
    memset(name,0,sizeof(name));
    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%ld",name,&memFreeSize);
    mem.freeMem = memFreeSize;
    mem.occpy = (float)(memTotalSize-memFreeSize)/(float)memTotalSize;
    fclose(fp);
    if(strlen(procname)==0)
    {
        return;
    }else
    {
        FILE* fpProc;
        char buf[100];
        char name[20];
        long memTotalSize;

        //processID int to string
        string str= getPidByName(procname);
        string fileName = "/proc/"+str+"/status";
        char *pach =(char*)fileName.data();
        fpProc = fopen(pach,"r");

        if(fpProc == NULL)
        {
            perror("fopen:");
            LOG(WARNING, "打不开进程内存文件");
            return;
        }
        //读取进程内存信息，在第16行
        for(int i = 0; i < 16; i++)
        {
            fgets(buf,sizeof(buf),fp);
        }
        sscanf(buf,"%s%ld",name,&memTotalSize);
        mem.procResTotalMem = memTotalSize;
        fgets(buf,sizeof(buf),fp);
        sscanf(buf,"%s%ld",name,&memTotalSize);
        mem.procVRTTotalMem = memTotalSize;

        mem.occpy = (float)mem.procResTotalMem/(float)mem.totalMem*100;

    }
}















