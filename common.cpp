#include "common.h"
#include<QMutex>
#include<QDateTime>
#include<QFile>
#include<QDir>
#include"config/qreadconfig.h"
#include <unistd.h>
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
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
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

    if (ret == statfs(path, &disk_info) == -1)
    {
      fprintf(stderr, "Failed to get file disk infomation,\
          errno:%u, reason:%s\n", errno, strerror(errno));
      return -1;
    }
    long long total_size = disk_info.f_blocks * disk_info.f_bsize;
    long long available_size = disk_info.f_bavail * disk_info.f_bsize;
    long long free_size = disk_info.f_bfree * disk_info.f_bsize;
    diskInfo.total_size = total_size;
    diskInfo.available_size = available_size;
    diskInfo.free_size = free_size;
    diskInfo.f_blocks = disk_info.f_blocks;

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
