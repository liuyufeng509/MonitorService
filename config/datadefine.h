#ifndef DATADEFINE_H
#define DATADEFINE_H
#include<QString>
//进程狗监控配置
struct ProcessDogConfig
{
    ProcessDogConfig():bOpen(false),strProcName("")
      ,strPath(""), nTimer(10)
    {

    }

    bool            bOpen;   		//是否开启
    QString         strProcName;	//要监控的进程名称
    QString         strPath;		//要启动的进程的执行文件路径
    int             nTimer;		//轮询间隔
};

//摄像机信息监控模块配置
struct CameraSvrConfig
{
    CameraSvrConfig():bOpen(false),timer(10)
    {

    }

    bool 	    bOpen;		//是否开启
    int         timer;		//轮询间隔
    QString  vdPath;    //摄像机视频文件保存的路径
};

struct DBSvrConfig
{
    DBSvrConfig():bOpen(false),timer(10)
    {

    }
    bool 	    bOpen;		//是否开启
    int 	    timer;		//轮询间隔
    QString dbName; //数据库名称
};


struct ThreadSvrConfig
{
    ThreadSvrConfig():bOpen(false),timer(10)
    {

    }
    bool 	    bOpen;		//是否开启
    int 	    timer;		//轮询间隔
};


struct CPUConfig
{
    CPUConfig():bOpen(false),timer(10)
    {

    }
    bool 	    bOpen;		//是否开启
    int 	    timer;		//轮询间隔
};


struct MEMConfig
{
    MEMConfig():bOpen(false),timer(10)
    {

    }
    bool 	    bOpen;		//是否开启
    int 	    timer;		//轮询间隔
};


struct DiskConfig
{
    DiskConfig():bOpen(false),timer(10)
    {

    }
    bool	    bOpen;		//是否开启
    int		    timer;		//轮询间隔
    QStringList diskPaths;       //硬盘们的挂载路径
};


struct NetConfig
{
    NetConfig():bOpen(false),timer(10)
    {

    }
    bool            bOpen;              //是否开启
    int             timer;              //轮询间隔
};


struct ProcessConfig
{
    ProcessConfig():bOpen(false),timer(10)
    {

    }
    bool            bOpen;              //是否开启
    int             timer;              //轮询间隔
};

//实时和历史视频调看模块配置
struct RelAndHisVideoConfig
{
    RelAndHisVideoConfig():bOpen(false),timer(10)
    {

    }
    bool            bOpen;              //是否开启
    int             timer;              //轮询间隔
};

//domainSocket 配置项
struct DomainSockConfig
{
    DomainSockConfig():serverName(""),clientName(""),checkTimer(0)
    {

    }
    QString         serverName;  //服务器名称
    QString         clientName;  //客户端名称
    int                 checkTimer;    //检测连接状态定时器
};
#endif // DATADEFINE_H
