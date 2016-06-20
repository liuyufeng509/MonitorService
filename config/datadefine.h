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
};

struct DBSvrConfig
{
    DBSvrConfig():bOpen(false),timer(10)
    {

    }
    bool 	    bOpen;		//是否开启
    int 	    timer;		//轮询间隔
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
    DomainSockConfig():serverName(""),clientName("")
    {

    }
    QString         serverName;  //服务器名称
    QString         clientName;  //客户端名称
};
#endif // DATADEFINE_H
