#include "systemresourcemonitor.h"
#include"../config/qreadconfig.h"
SystemResourceMonitor::SystemResourceMonitor()
{
}

/***************************************************
 * CPU                                             *
 **************************************************/
/*get cpu total time and iedl*/
void SystemResourceMonitor::GetTotalCpuTimeAndIedl(cpuTime_t&cpuTAI)
{
    FILE *fp;
    char buf[BUFFSIZE];
    char cpuName[5];
    long int user,nice,sys,idle,iowait,irq,softirq;

    //open file
    fp = fopen("/proc/stat","r");
    if(fp == NULL)
    {
        perror("fopen:");
       // exit (0);
        return;
    }

    //read to buf
    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%ld%ld%ld%ld%ld%ld%ld",cpuName,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);

    //cpu total time
    cpuTAI.totalCpuTime = user+nice+sys+idle+iowait+irq+softirq;

    //free time
    cpuTAI.idel = idle;

    //close file
    fclose(fp);
}


/*get total process time*/
long SystemResourceMonitor::GetTotalProcessTime(int processID)
{
    //cout<<processID<<endl;
    FILE *fp;
    char buf[256];
    char comm[20];
    char task_state[2];

    long int pid,ppid,pgid,sid,tty_nr,tty_pgrp,task_flags,min_flt,cmin_flt,maj_flt,cmaj_flt,utime,stime,cutime,cstime;

    long int all1;

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
        //exit (0);
        return -1;
    }

    fgets(buf,sizeof(buf),fp);

    sscanf(buf,"%ld%s%s%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld",&pid,comm,task_state,&ppid,&pgid,&sid,&tty_nr,&tty_pgrp,&task_flags,&min_flt,&cmin_flt,&maj_flt,&cmaj_flt,&utime,&stime,&cutime,&cstime);

    all1 = utime + stime + cutime + cstime;

    fclose(fp);
    return all1;

}


/*get process state*/
void SystemResourceMonitor::GetProcessState(int processID,char state[2])
{
    FILE *fp;
    char buf[256];
    char comm[20];
    //char state[2];
    long int pid = 0;

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
        perror("fopen:");
       // exit (0);
        return;
    }

    fgets(buf,sizeof(buf),fp);

    sscanf(buf,"%ld%s%s",&pid,comm,state);

    fclose(fp);
    //cout<<state<<endl;

}


/*get thread total time*/
long SystemResourceMonitor::GetTotalThreadTime(int processID,int threadID)
{
    FILE *fp;
    char buf[256];
    char comm[20];
    char task_state[2];

    long int pid,ppid,pgid,sid,tty_nr,tty_pgrp,task_flags,min_flt,cmin_flt,maj_flt,
            cmaj_flt,utime,stime,cutime,cstime;

    long int all1;

    //processID int to string
    string str_p,str_t;
    char t[256];
    sprintf(t, "%d", processID);
    str_p = t;

    //threadID int to string
    memset(t,0,sizeof(t));
    sprintf(t,"%d",threadID);
    str_t = t;
    string fileName = "/proc/" + str_p +"/task/" + str_t +"/stat";
    //cout<<fileName<<":";
    char *p =(char*)fileName.data();

    //open file
    fp = fopen(p,"r");
    if(fp == NULL)
    {
        perror("fopen:");
      //  exit (0);
        return -1;
    }

    fgets(buf,sizeof(buf),fp);

    sscanf(buf,"%ld%s%s%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld",&pid,comm,task_state,&ppid,&pgid,&sid,&tty_nr,&tty_pgrp,&task_flags,&min_flt,&cmin_flt,&maj_flt,&cmaj_flt,&utime,&stime,&cutime,&cstime);

    all1 = utime + stime;

    fclose(fp);
    return all1;
}


/*get media process ID*/
int SystemResourceMonitor:: GetMediaProcessID(char* processName)
{
    //get process id by shell
    char cmd[100]={0};
    system("touch pid.txt");
    sprintf(cmd,"pidof %s > pid.txt",processName);
    system(cmd);

    //open file
    int fd = open("pid.txt" ,O_RDWR);
    if(fd<0)
    {
        perror("open file failed/n");
        system("rm pid.txt");
        return -1;
    }

    //read file
    char read_buf[50]={0};
    if(read(fd,read_buf,50)<0)
    {
        perror("read():");
        close(fd);
        system("rm pid.txt");
        return -1;
    }

    //close file
    if(close(fd)<0)
    {
        perror("close():");
        system("rm pid.txt");
        return -1;
    }
    //remove file
    system("rm pid.txt");

    return atoi(read_buf);

}


/*get system process and thread CPU usage*/
void SystemResourceMonitor::GetSysProThrCpuUsage(struct CPU& cpu)
{
    //first
    //system CPU
    //cpu.MediaProcessName = "HbMedia";
    long cpuTotalTime1,cpuTotalTime2,idle1,idle2;
    float systemUsage;
    cpuTime_t cpuT;
    cpuTime_t &cpuTAI = cpuT;
    GetTotalCpuTimeAndIedl(cpuTAI);

    //total cpu time
    cpuTotalTime1 = cpuTAI.totalCpuTime;

    //free time
    idle1 = cpuTAI.idel;

    //process CPU
    //get media process ID
    cpu.MediaProcessID = GetMediaProcessID(cpu.MediaProcessName);
    float processUsage;
    int processID = cpu.MediaProcessID;
    long processTotalTime1,processTotalTime2;

    //first get process total time
    processTotalTime1 = GetTotalProcessTime(processID);

    //thread CPU
    float threadUsage;
    //get thread ID
    int threadID = cpu.threadID;
    long threadTotalTime1,threadTotalTime2;

    //first get thread total cpu time
    threadTotalTime1 = GetTotalThreadTime(processID,threadID);

    //second get data
    sleep(CK_TIME);
    memset(&cpuTAI,0,sizeof(cpuTime_t));
    GetTotalCpuTimeAndIedl(cpuTAI);
    cpuTotalTime2 = cpuTAI.totalCpuTime;
    idle2 = cpuTAI.idel;
    systemUsage = (float)(cpuTotalTime2-cpuTotalTime1-(idle2-idle1)) / (cpuTotalTime2-cpuTotalTime1)*100 ;

    //get system cpu usage
    cpu.systemCpu = systemUsage;

    //get process cpu usage
    processTotalTime2 = GetTotalProcessTime(processID);
    processUsage = (float)(processTotalTime2-processTotalTime1) / (cpuTotalTime2-cpuTotalTime1) * 100;
    cpu.processCpu = processUsage;

    //get thread cpu usage
    threadTotalTime2 = GetTotalThreadTime(processID,threadID);
    threadUsage = (float)(threadTotalTime2-threadTotalTime1)/(cpuTotalTime2-cpuTotalTime1)*100;
    cpu.threadCpu = threadUsage;
}


/***************************************************
 * 内存                                             *
 **************************************************/
/*get memory info*/
void SystemResourceMonitor::GetSysProThrMemUsage(memory_t& memoryInfo)
{
    GetSystemMemInfo(memoryInfo);
    GetProcessMemUsage(memoryInfo);
    GetThreadMemUsage(memoryInfo);
}


/*get system memory info*/
void SystemResourceMonitor::GetSystemMemInfo(memory_t& memoryInfo)
{
    GetSystemMemTotalAndFree(memoryInfo);
    memoryInfo.systemMemUsage = (float)(memoryInfo.systemTotalMem-memoryInfo.systemFreeMem)/(memoryInfo.systemTotalMem)*100;
}


/*get process memory usage*/
void SystemResourceMonitor::GetProcessMemUsage(memory_t& memoryInfo)
{
    //memoryInfo.processName = "HbMedia";
    GetProcessUseMem(memoryInfo);
    GetSystemMemTotalAndFree(memoryInfo);
    memoryInfo.processMemUsage = ((float)memoryInfo.processUseMem) / ((float)memoryInfo.systemTotalMem)*100;
}


/*get thread mem usage*/
void SystemResourceMonitor::GetThreadMemUsage(memory_t& memoryInfo)
{
    GetThreadUseMem(memoryInfo);
    GetSystemMemTotalAndFree(memoryInfo);
    memoryInfo.threadMemUsage = ((float)memoryInfo.threadUseMem)/((float)memoryInfo.systemTotalMem)*100;
}


/*get system memory total and free size*/
void SystemResourceMonitor::GetSystemMemTotalAndFree(memory_t& memoryInfo)
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
      //  exit (0);
        return;
    }

    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%ld",name,&memTotalSize);
    memoryInfo.systemTotalMem = memTotalSize;
    memset(name,0,sizeof(name));
    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%ld",name,&memFreeSize);
    memoryInfo.systemFreeMem = memFreeSize;

    fclose(fp);
}


/*get process use memory*/
void SystemResourceMonitor::GetProcessUseMem(memory_t& memoryInfo)
{
    FILE* fp;
    char buf[100];
    char name[20];
    long memTotalSize;
    //get process ID
    int processID = GetMediaProcessID(memoryInfo.processName);
    //processID int to string
    string str;
    char t[256];
    sprintf(t, "%d", processID);
    str = t;
    string fileName = "/proc/"+str+"/status";
    char *pach =(char*)fileName.data();
    fp = fopen(pach,"r");

    if(fp == NULL)
    {
        perror("fopen:");
     //   exit (0);
        return;
    }
    //读取进程内存信息，在第16行
    for(int i = 0; i <= VmRssLine; i++)
    {
        fgets(buf,sizeof(buf),fp);
    }
    sscanf(buf,"%s%ld",name,&memTotalSize);
    memoryInfo.processUseMem = memTotalSize;
}


/*get thread use mem*/
void SystemResourceMonitor::GetThreadUseMem(memory_t& memoryInfo)
{
    //memoryInfo.processName = "HbMedia";
    FILE* fp;
    char buf[100];
    char name[20];
    long memTotalSize;
    //get process ID
    int processID = GetMediaProcessID(memoryInfo.processName);
    int threadID = memoryInfo.threadID;
    //processID int to string
    string str_p;
    char t[256];
    sprintf(t, "%d", processID);
    str_p = t;

    //processID int to string
    string str_t;
    memset(t,0,sizeof(t));
    sprintf(t, "%d", threadID);
    str_t = t;
    string fileName = "/proc/"+str_p+"/task/"+str_t+"/status";
    char *pach =(char*)fileName.data();
    fp = fopen(pach,"r");

    if(fp == NULL)
    {
        perror("fopen:");
    //    exit (0);
        return;
    }
    //读取线程内存信息，在第16行
    for(int i = 0; i <= VmRssLine; i++)
    {
        fgets(buf,sizeof(buf),fp);
    }
    sscanf(buf,"%s%ld",name,&memTotalSize);
    memoryInfo.threadUseMem = memTotalSize;
}


/***************************************************
 * 网络                                             *
 **************************************************/
/*get net interface info*/
void SystemResourceMonitor::GetNetInterfaceInfo(net_t& net)
{
    GetNetInterName(net);
    //cout<<"first net interface name: "<<net.netInterOneName<<endl;
    //cout<<"second net interface name: "<<net.netInterTwoName<<endl;
    char* netInterName = (char*)malloc(20);
    strcpy(netInterName,net.netInterOneName);
    net.netInterOneSpeedLevel = GetNetInterSpeedLevel(netInterName);
    memset(netInterName,0,20);
    strcpy(netInterName,net.netInterTwoName);
    net.netInterTwoSpeedLevel = GetNetInterSpeedLevel(netInterName);
    memset(netInterName,0,20);
    strcpy(netInterName,net.netInterOneName);
    long *ifconfig_result;
    double netOneRec_mb1 = 0;
    double netOneSed_mb1 = 0;
    double netOneRec_mb2 = 0;
    double netOneSed_mb2 = 0;
    double netTwoRec_mb1 = 0;
    double netTwoSed_mb1 = 0;
    double netTwoRec_mb2 = 0;
    double netTwoSed_mb2 = 0;
    /*eth0 计算机上的网卡的名字*/
    ifconfig_result = GetNetInterRealTimeSpeed(netInterName);

    /*保存在文件中的数值的单位是B，经过计算换算成MB*/
    netOneRec_mb1 = (double)ifconfig_result[0]/(1024*1024);
    //cout<<"接收流量："<<rec_mb1<<"MB"<<endl;

    /*保存在文件中的数值的单位是B，经过计算换算成MB*/
    netOneSed_mb1 = (double)ifconfig_result[1]/(1024*1024);
    //cout<<"发送流量："<<sed_mb1<<"MB"<<endl;

    memset(netInterName,0,20);
    strcpy(netInterName,net.netInterTwoName);
    memset(ifconfig_result,0,sizeof(long)*2);
    ifconfig_result = GetNetInterRealTimeSpeed(netInterName);

    /*保存在文件中的数值的单位是B，经过计算换算成MB*/
    netTwoRec_mb1 = (double)ifconfig_result[0]/(1024*1024);
    //cout<<"接收流量："<<rec_mb1<<"MB"<<endl;

    /*保存在文件中的数值的单位是B，经过计算换算成MB*/
    netTwoSed_mb1 = (double)ifconfig_result[1]/(1024*1024);
   // cout<<"发送流量："<<sed_mb1<<"MB"<<endl;

    sleep(CK_TIME);
    //第二次获取
    memset(netInterName,0,20);
    strcpy(netInterName,net.netInterOneName);
    memset(ifconfig_result,0,sizeof(long)*2);
    ifconfig_result = GetNetInterRealTimeSpeed(netInterName);

    /*保存在文件中的数值的单位是B，经过计算换算成MB*/
    netOneRec_mb2 = (double)ifconfig_result[0]/(1024*1024);
    //cout<<"接收流量："<<rec_mb2<<"MB"<<endl;

    /*保存在文件中的数值的单位是B，经过计算换算成MB*/
    netOneSed_mb2 = (double)ifconfig_result[1]/(1024*1024);
    //cout<<"发送流量："<<sed_mb2<<"MB"<<endl;
    net.netInterOneRtUpSpeed = netOneSed_mb2-netOneSed_mb1;
    net.netInterOneRtDownSpeed = netOneRec_mb2-netOneRec_mb1;
    //
    memset(netInterName,0,20);
    strcpy(netInterName,net.netInterTwoName);
    memset(ifconfig_result,0,sizeof(long)*2);
    ifconfig_result = GetNetInterRealTimeSpeed(netInterName);

    /*保存在文件中的数值的单位是B，经过计算换算成MB*/
    netTwoRec_mb2 = (double)ifconfig_result[0]/(1024*1024);
    //cout<<"接收流量："<<rec_mb2<<"MB"<<endl;

    /*保存在文件中的数值的单位是B，经过计算换算成MB*/
    netTwoSed_mb2 = (double)ifconfig_result[1]/(1024*1024);
   //cout<<"发送流量："<<sed_mb2<<"MB"<<endl;
    net.netInterTwoRtUpSpeed = netTwoSed_mb2-netTwoSed_mb1;
    net.netInterTwoRtDownSpeed = netTwoRec_mb2-netTwoRec_mb1;

    //get usage
    GetNetInterBandwidthUsage(net);
    free(netInterName);
}


/*get net interface name*/
void SystemResourceMonitor::GetNetInterName(net_t& net)
{
    /*struct ifreq
    这个结构定义在include/net/if.h，用来配置ip地址，激活接口，配置MTU等接口信息的
    struct ifconf
    通常是用来保存所有接口信息的*/
    int inter_fd;
    int if_len;
    struct ifconf ifc;//保存所有接口信息
    struct ifreq buf[5];//接口信息,获取网卡的信息

    if ((inter_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket(AF_INET, SOCK_DGRAM, 0)");
      //  exit(1);
        return;
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;

    if (ioctl(inter_fd, SIOCGIFCONF, (char *) &ifc) == -1)
    {
        perror("SIOCGIFCONF ioctl");
    //    exit(1);
        return;
    }

    if_len = ifc.ifc_len / sizeof(struct ifreq);//接口数量
    int count = 0;
    while (if_len-- > 0)
    {
        count++;
        if(count == 1)
        {
//            net->netInterOneName = buf[if_len].ifr_name;
            strcpy(net.netInterOneName, buf[if_len].ifr_name);
            //cout<<"first net interface name: "<<net.netInterOneName<<endl;

        }
        if(count == 2)
        {
            strcpy(net.netInterTwoName, buf[if_len].ifr_name);
            //cout<<"second net interface name: "<<net.netInterTwoName<<endl;
        }

    }
}


/*get net interface speed level*/
long SystemResourceMonitor::GetNetInterSpeedLevel(char* netInterName)
{

    char *p = netInterName;
    //cout<<p<<endl;
    string str = p;
    //cout<<str<<endl;
    string fileName = "dmesg | grep "+str+"> net.txt";
    char* pach = (char*)fileName.data();
    system("touch net.txt");
    system(pach);
    FILE* fp;
    fp = fopen("net.txt","r");
    char buf[256];
    fgets(buf,sizeof(buf),fp);
    fgets(buf,sizeof(buf),fp);
    fgets(buf,sizeof(buf),fp);
    char buf1[10],buf2[10],buf3[10],buf4[10],buf5[10],buf6[10];
    long level;
    sscanf(buf,"%s%s%s%s%s%s%ld",buf1,buf2,buf3,buf4,buf5,buf6,&level);
    //cout<<level<<endl;
    fclose(fp);
    system("rm net.txt");
    return level;

}


/*get net interface real time speed*/
long* SystemResourceMonitor::GetNetInterRealTimeSpeed(char* netInterName)
{
    int fd = open("/proc/net/dev", O_RDONLY | O_EXCL);
      if (-1 == fd)
      {
          printf("/proc/net/dev not exists!\n");
          return NULL;
      }

          char buf[1024*2];
          lseek(fd, 0, SEEK_SET);
          int nBytes = read(fd, buf, sizeof(buf)-1);
          if (-1 == nBytes)
          {
              perror("read error");
              close(fd);
              return NULL;
          }
          buf[nBytes] = '\0';
          //返回第一次指向ath0位置的指针
          char* pDev = strstr(buf, netInterName);
          if (NULL == pDev)
          {
              printf("don't find dev %s\n", netInterName);
              return NULL;
          }
          char *p;
          char *ifconfig_value;
          int i = 0;
          static long rx2_tx10[2];
          /*去除空格，制表符，换行符等不需要的字段*/
          for (p = strtok(pDev, " \t\r\n"); p; p = strtok(NULL, " \t\r\n"))
          {
              i++;
              ifconfig_value = (char*)malloc(20);
              strcpy(ifconfig_value, p);
              /*得到的字符串中的第二个字段是接收流量*/
              if(i == 2)
              {
                  rx2_tx10[0] = atol(ifconfig_value);
              }
              /*得到的字符串中的第十个字段是发送流量*/
              if(i == 10)
              {
                  rx2_tx10[1] = atol(ifconfig_value);
                  break;
              }
              free(ifconfig_value);
          }
          close(fd);
          return rx2_tx10;

}


/*get net interface BandwidthUsage*/
void SystemResourceMonitor::GetNetInterBandwidthUsage(net_t& net)
{
    net.netInterTwoBandwidthUsage = (net.netInterTwoRtDownSpeed + net.netInterTwoRtUpSpeed)/(net.netInterTwoSpeedLevel)*100;
    net.netInterOneBandwidthUsage = (net.netInterOneRtDownSpeed + net.netInterOneRtUpSpeed)/(net.netInterOneSpeedLevel)*100;
}


/***************************************************
 * 进程监控/杀死/重启                                 *
 **************************************************/
void SystemResourceMonitor::KillAndStartProcess(char* processName)
{
    int processID = GetMediaProcessID(processName);
    char link[100], path[100];
    memset(link,0,sizeof(link));
    memset(path,0,sizeof(path));
    sprintf(link, "/proc/%d/exe",processID);
    readlink(link, path, sizeof(path));

    char* path1 = (char*)malloc(100);
    strcpy(path1,path);

    string str = path1;

    // int to string
    string strKill;
    char t[256];
    sprintf(t, "%d", processID);
    strKill = t;

    string fileName1 = "kill -9 " + strKill;
    char *p =(char*)fileName1.data();
    //
    system(p);

    string str1 = /*"gnome-terminal -e "+*/str;
    char* fileName = NULL;
    fileName = (char*)str1.data();
    if(isDebug)
        cout<<fileName<<endl;
    signal(SIGCHLD, SIG_IGN);
    if(fork()==0)
    {
        if( execl( fileName, "StreamPro",NULL, NULL ) == -1 )
        {
          qWarning()<<"进程:"<<fileName<<" 启动失败";
        }
    }
    //system(fileName);
    LOG(INFO, ("KillAndStartProcess end:"+ QString(fileName)).toStdString().c_str());
}

void SystemResourceMonitor::StartProcess()
{

    string str = QReadConfig::getInstance()->getProcDogConf().strPath.toStdString();

    string str1 = /*"gnome-terminal -e "+*/str;   //后台运行
    if(isDebug)
        cout<<str1<<endl;
    char* fileName = NULL;
    fileName = (char*)str1.data();
    signal(SIGCHLD, SIG_IGN);
    if(fork()==0)
    {
        if( execl( fileName, "StreamPro",NULL, NULL ) == -1 )
        {
          qWarning()<<"进程:"<<fileName<<" 启动失败";
        }
    }
    LOG(INFO, ("StartProcess end:"+ QString(fileName)).toStdString().c_str());
}

void SystemResourceMonitor::MediaProcessMonitor()
{
    char *processName = new char[255];
    memset(processName, 0, 255);
    strcpy(processName, QReadConfig::getInstance()->getProcDogConf().strProcName.toStdString().c_str());
    int processID = atoi(getPidByName(processName).c_str());
    if(processID==0)            //进程号为0，说明未启动
    {
        LOG(INFO,"进程id为0，程序未启动,启动之");
        StartProcess();
    }else                               //进程如果已启动，判断进程状态
    {
        char state[2] = {0};
        GetProcessState(processID,state);
        LOG(INFO,("进程运行状态:"+QString(state[0])).toStdString().c_str());
        if(state[0] == 'Z')
        {
            KillAndStartProcess(processName);
        }
    }
    delete[] processName;
}
