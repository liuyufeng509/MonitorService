#include "streammonitor.h"
#include <iostream>
#include "../config/qreadconfig.h"
#include"localclient.h"
#include<QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QFileInfo>
#include<QSqlDatabase>
#include<math.h>
#include<QSqlQuery>
#include<QDateTime>
#include<QSqlError>

#include"intcommon/intcommon.nsmap"
#include"intcommon/soapintcommonProxy.h"
#include "intcommon/stdsoap2.h"

using namespace std;

StreamMonitor::StreamMonitor(QObject *parent) : QObject(parent),relReqCount(0),hisReqCount(0)
{
    diskErrInfoMap.insert(DiskStateInfo::CAN_NOT_CREATE_FILE, "不能创建文件"); //向map里添加一对“键-值”
    diskErrInfoMap.insert(DiskStateInfo::CAN_NOT_WRITE_FILE, "不能写入文件"); //向map里添加一对“键-值”
    diskErrInfoMap.insert(DiskStateInfo::CAN_NOT_MOUNT, "硬盘未挂载"); //向map里添加一对“键-值”
    diskErrInfoMap.insert(DiskStateInfo::DISK_OVER_LOAD, "硬盘满"); //向map里添加一对“键-值”
    diskErrInfoMap.insert(DiskStateInfo::DISK_NORMAL, "硬盘状态正常"); //向map里添加一对“键-值”

    hisFileErrInfoMap.insert(HisVdFileState::NORMAL, "历史文件正常"); //向map里添加一对“键-值”
    hisFileErrInfoMap.insert(HisVdFileState::UNNORMAL, "历史文件异常"); //向map里添加一对“键-值”

    relVdRecErrInfoMap.insert(CameraStateInfo::NORMAL, "实时视频录制正常"); //向map里添加一对“键-值”
    relVdRecErrInfoMap.insert(CameraStateInfo::UNNORMAL, "实时视频录制异常"); //向map里添加一对“键-值”
    relVdRecErrInfoMap.insert(CameraStateInfo::NOT_EXIST, "实时视频不存在"); //向map里添加一对“键-值”
    relVdRecErrInfoMap.insert(CameraStateInfo::NOT_ONLINE, "该摄像机不在线"); //向map里添加一对“键-值”

    relHisVdReqErrInfoMap.insert(RelAndHisVdReqStat::NORMAL,"视频调看成功");
    relHisVdReqErrInfoMap.insert(RelAndHisVdReqStat::SSLFAIL,"ssl证书加载失败");
    relHisVdReqErrInfoMap.insert(RelAndHisVdReqStat::UNORMAL,"视频调看失败");

    DbStatusErrInfoMap.insert(DBStatusInfo::NORMAL, "数据库状态正常");
    DbStatusErrInfoMap.insert(DBStatusInfo::DB_OPEN_FAIL, "数据库打开失败");
    DbStatusErrInfoMap.insert(DBStatusInfo::DB_LOST_DB_FILE, "数据库表丢失");
    DbStatusErrInfoMap.insert(DBStatusInfo::DB_TABLE_LOCKED, "数据库表锁定");

    threadErrorInfoMap.insert(ThreadStateInfo::NORMAL, "线程正常");
    threadErrorInfoMap.insert(ThreadStateInfo::Dead, "线程死亡");

    relRqTimer = new QTimer(this);
    connect(relRqTimer, SIGNAL(timeout()), this, SLOT(relVdReqWithTimer()));

    hisRqTimer = new QTimer(this);
    connect(hisRqTimer, SIGNAL(timeout()), this, SLOT(hisVdReqWithTimer()));


    manager = new QNetworkAccessManager(this);      // url request manager


    //初始化 diskInfos

    for(int i=0; i<QReadConfig::getInstance()->getDiskCong().diskPaths.size(); i++)
    {

        DiskStateInfo disk;
        disk.baseInfo.mountPath = QReadConfig::getInstance()->getDiskCong().diskPaths[i];
        disk.state = DiskStateInfo::DISK_NORMAL;
        diskInfos.append(disk);
    }

 }
//解析xml协议
void StreamMonitor::doParseXml(QString xml)
{
    LOG(INFO,"rcv xml msg:");
    qInfo()<<xml;
    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(xml, false, &error, &row, &column))     //加载xml内容
    {
        qCritical()<<(QString("parse file failed at line row and column") + QString::number(row, 10) + QString(",") + QString::number(column, 10));
        return ;
    }
    if(document.isNull())
    {
        LOG(WARNING,"document is null");
        return ;
    }

    //解析
    QDomElement message = document.documentElement();        //<message>
    QDomElement type = message.firstChildElement();		//<TYPE>标签
    qInfo()<<"type="<<type.text().toInt();

    switch (type.text().toInt()) {
        case Rcv_Cameras_Info:
        {
            qInfo()<<" Cameras List Parse";
            QDomElement action = message.firstChildElement("ACTION");          //<ACTION>
            qInfo()<<"action(1,init; 2,add; 3,del)="<<action.text().toInt();
            //<Equipment>
            QDomNodeList equipments = message.elementsByTagName("Equipment");
            int nCount = equipments.count();
            for(int i=0; i<nCount; i++)
            {
                QDomNode eqNode = equipments.item(i);
                QDomElement equipment = eqNode.toElement();         //<EquipMent>
                QDomElement cameraId = equipment.firstChildElement(); //<CAMID>
                CameraStateInfo camInfo;
                camInfo.cmareId = cameraId.text();
                qInfo()<<"cameraId="<<cameraId.text();
                QDomElement udpAddr = equipment.lastChildElement("UDP");  //<UDP>
                camInfo.udpAddr = udpAddr.text();
                qInfo()<<"udpAddr="<<udpAddr.text();

                QDomElement uuid = equipment.firstChildElement("UUID");     //<UUID>
                camInfo.uuid = uuid.text();
                switch (action.text().toInt())
                {
                case 1:         //init
                case 2:         //add
                {
                    if(!camerasInfo.contains(camInfo))
                    {
                        camerasInfo.append(camInfo);
                    }else
                        camerasInfo[camerasInfo.indexOf(camInfo)] = camInfo;
                }
                    break;
                case 3:            //del
                {
                    if(camerasInfo.contains(camInfo))
                    {
                        camerasInfo.removeOne(camInfo);
                    }
                }
                    break;
                default:
                    break;
                }
            }
        }
            break;
        case Rcv_Camera_Online:
        {
            qInfo()<<" Cameras online Parse";
            //<Equipment>
            QDomNodeList equipments = message.elementsByTagName("Equipment");
            int nCount = equipments.count();
            for(int i=0; i<nCount; i++)
            {
                QDomNode eqNode = equipments.item(i);
                QDomElement equipment = eqNode.toElement();         //<EquipMent>
                QDomElement cameraId = equipment.firstChildElement(); //<CAMID>
                QDomElement status = equipment.lastChildElement();          //<STATUS>

                CameraStateInfo camInfo;
                camInfo.cmareId = cameraId.text();
                camInfo.online = status.text().toInt()==0? true:false;
                if(camerasInfo.contains(camInfo))
                {
                    camerasInfo[camerasInfo.indexOf(camInfo)].online=camInfo.online;
                }else
                    camerasInfo.append(camInfo);
                qInfo()<<"cameraId="<<cameraId.text()<<" status(0 normal)="<<status.text().toInt();
                //来一条在线状态就要转发给运维
                OMData om;
                om.uuid = uuid;
                om.devId= camInfo.uuid;
                om.type="j7";
                om.status=camInfo.online;
                SendDataToOM(om);
            }
        }
            break;
        case RcV_Threads_Info:
        {
            qInfo()<<" threads list Parse";
            //<Equipment>
            QDomNodeList equipments = message.elementsByTagName("Equipment");
            int nCount = equipments.count();
            for(int i=0; i<nCount; i++)
            {
                QDomNode eqNode = equipments.item(i);
                QDomElement equipment = eqNode.toElement();         //<EquipMent>
                QDomElement threadId = equipment.firstChildElement("THRID"); //<THRID>
                QDomElement action = equipment.lastChildElement("ACTION");          //<ACTION>
                QDomElement optType = equipment.lastChildElement("OPTTYPE");         //<OPTTYPE>
                ThreadStateInfo threadInfo;
                threadInfo.threadId = threadId.text();
                threadInfo.action = action.text().toInt();
                if(!threadsInfo.contains(threadInfo))
                {
                    if(optType.text().toInt()==1)
                        threadsInfo.append(threadInfo);
                }else
                {
                    if(optType.text().toInt()==2)           //1 add  2 delete
                        threadsInfo.removeOne(threadInfo);
                    else
                        threadsInfo[threadsInfo.indexOf(threadInfo)].action = threadInfo.action;
                }
                qInfo()<<"threadId="<<threadId.text()<<" action="<<action.text().toInt();
            }
        }
            break;
        case Rcv_HisFile_Info:
        {
            qInfo()<<" HisFile Info Parse";
            //<Equipment>
            QDomNodeList equipments = message.elementsByTagName("Equipment");
            int nCount = equipments.count();
            for(int i=0; i<nCount; i++)
            {
                QDomNode eqNode = equipments.item(i);
                QDomElement equipment = eqNode.toElement();         //<EquipMent>
                QDomElement cameraId = equipment.firstChildElement(); //<CAMID>
                QDomElement filepath = equipment.lastChildElement();          //<FILE>
                CameraStateInfo camInfo;
                camInfo.cmareId=cameraId.text();
                camInfo.hisVdSta.hisVdPath=filepath.text();
                if(camerasInfo.contains(camInfo))
                {
                    camerasInfo[camerasInfo.indexOf(camInfo)].hisVdSta.hisVdPath = camInfo.hisVdSta.hisVdPath;
                }else
                    camerasInfo.append(camInfo);

                //check the history file
                checkHisFile(camerasInfo[camerasInfo.indexOf(camInfo)]);
                qInfo()<<"cameraId="<<cameraId.text()
                   <<" filepath="<<filepath.text();
            }
        }
            break;
        case Rcv_Thread_Heart:
        {
            qInfo()<<" Thread Heart Parse";
            //<Equipment>
            QDomNodeList equipments = message.elementsByTagName("Equipment");
            int nCount = equipments.count();
            for(int i=0; i<nCount; i++)
            {
                QDomNode eqNode = equipments.item(i);
                QDomElement equipment = eqNode.toElement();         //<EquipMent>
                QDomElement threadId = equipment.firstChildElement(); //<THRID>
                qInfo()<<"threadId="<<threadId.text();
                for(int j=0; j<threadsInfo.size(); j++)
                {
                    if(threadsInfo[j].threadId == threadId.text())
                    {
                        threadsInfo[j].heartime = time(NULL);
                        break;
                    }
                }
            }
        }
            break;
        case Rcv_RelVd_Res:     //改用gsoap调用，所以该case分支不再使用，代码暂且保留。
        {
            qInfo()<<" Rel Vedeo Res Parse";
            //<Equipment>
            QDomNodeList equipments = message.elementsByTagName("Equipment");
            int nCount = equipments.count();
            for(int i=0; i<nCount; i++)
            {
                QDomNode eqNode = equipments.item(i);
                QDomElement equipment = eqNode.toElement();         //<EquipMent>
                QDomElement cameraId = equipment.firstChildElement(); //<CAMID>
                QDomElement status = equipment.lastChildElement();          //<Status>
                qInfo()<<"cameraId="<<cameraId.text()<<" status(0 normal)="<<status.text().toInt();
            }
        }
            break;
        case Rcv_HisVd_Res:
        {
            qInfo()<<"接收到历史视频调看请求的回应";
            //<Equipment>
            QDomNodeList equipments = message.elementsByTagName("Equipment");
            int nCount = equipments.count();
            for(int i=0; i<nCount; i++)
            {
                QDomNode eqNode = equipments.item(i);
                QDomElement equipment = eqNode.toElement();         //<EquipMent>
                QDomElement cameraId = equipment.firstChildElement(); //<CAMID>
                QDomElement httpurl = equipment.lastChildElement();          //<Httpurl>
                qInfo()<<"cameraId="<<cameraId.text()<<" httpurl(0 normal)="<<httpurl.text().toInt();
                checkHisURL(httpurl.text());
            }
        }
            break;
        case Rcv_YUNWEI_Info:
        {
            QDomElement uuidEl = message.firstChildElement("UUID");           //<UUID>
            QDomElement addr = message.firstChildElement("ADDRESS");               //<ADDRESS>
            uuid = uuidEl.text();
            if(isOMInited)
            {
                if(!NetMasterDestroy())
                {
                    LOG(INFO, ">>>>>> [NetMasterDestroy] is Success. <<<<<<\n");
                }
                else
                {
                    LOG(INFO, ">>>>>> [NetMasterDestroy] is Fail! <<<<<<\n");
                    LOG(INFO, ("OM INFO:  uuid:"+uuidEl.text() +" ADDR:"+addr.text()).toStdString().c_str());
                    isOMInited = false;
                    if(!NetMasterInit(addr.text().toStdString().c_str(), uuidEl.text().toStdString().c_str()))
                    {
                        LOG(INFO, ">>>>>> [NetMasterInit] is Success. <<<<<<");
                        isOMInited = true;
                    }else
                    {
                        LOG(INFO, ">>>>>> [NetMasterInit] is Fail! <<<<<<" );
                        isOMInited = false;
                    }
                }
            }else
            {
                if(!NetMasterInit(addr.text().toStdString().c_str(), uuidEl.text().toStdString().c_str()))
                {
                    LOG(INFO, ">>>>>> [NetMasterInit] is Success. <<<<<<");
                    isOMInited = true;
                }else
                {
                    LOG(INFO, ">>>>>> [NetMasterInit] is Fail! <<<<<<" );
                    isOMInited = false;
                }
            }
        }
            break;
        default:
            break;
    }

}

void StreamMonitor::checkHisFile(CameraStateInfo &camera)
{
        //检测规则?
    //有问题则返回给流媒体
    QFile file(camera.hisVdSta.hisVdPath);
    if(!file.exists())
    {
        camera.hisVdSta.state = HisVdFileState::NOT_EXIST;
    }else if(!file.open(QIODevice::ReadOnly))
    {
        camera.hisVdSta.state=HisVdFileState::NOT_OPEN;
    }else
    {
        quint8 data[86];//先验证前86个字节
        file.read((char*)data, sizeof(data));
        if(data[0]==0&&data[1]==0&&data[14]==0&&data[15]==0&&data[32]==0&&data[33]==0&&data[62]==0&&
            data[63]==0&&data[81]==0&&data[82]==0&&data[83]==0&&
            data[2]==1&&data[16]==1&&data[34]==1&&data[64]==1&&data[84]==1&&
            data[3]==0xba&&data[17]==0xbb&&data[35]==0xbc&&data[65]==0xe0&&(data[85]&0x1f)==7)
        {
            camera.hisVdSta.state=HisVdFileState::NORMAL;
        }else
            camera.hisVdSta.state=HisVdFileState::UNNORMAL;
    }

}

void StreamMonitor::sendMsg(QString xml)
{
    QByteArray datas;
     qint32 size=xml.length();
    char data[4];
    memset(data, 0, 4);
    memcpy(data, &size, sizeof(qint32));
    datas.append(QByteArray(data, 4));
    datas.append(xml.toLatin1());
    LocalClient::getInstance()->writeData(datas);
}

void StreamMonitor::sendDiskState()
{
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement message = doc.createElement("message");  //<message>
    doc.appendChild(message);
    QDomElement type = doc.createElement("TYPE");//<TYPE>
    QDomText typeContent = doc.createTextNode(QString::number(Send_Disk_Info));
    type.appendChild(typeContent);
    message.appendChild(type);
    for(int i=0; i<diskInfos.size(); i++)
    {
        QDomElement equip = doc.createElement("Equipment");//<Equipment>
        message.appendChild(equip);
        QDomElement mPath = doc.createElement("MPath");//<MPath>
        QDomText mPath_str= doc.createTextNode(diskInfos[i].baseInfo.mountPath);
        mPath.appendChild(mPath_str);
        equip.appendChild(mPath);
        QDomElement status = doc.createElement("Status");//<Status>
        QDomText status_str = doc.createTextNode(QString::number(diskInfos[i].state));
        status.appendChild(status_str);
        equip.appendChild(status);
    }

    QString diskXml = doc.toString();
    sendMsg(diskXml);
}

void StreamMonitor::monitorDiskInfo()
{
    //1.检测硬盘是否可以创建文件
    //diskInfos.clear();
    bool isAllBad = true;           //所有硬盘都坏掉的时候，就是流媒体服务坏掉了。
    for(int i=0; i<diskInfos.size(); i++)
    {
        bool isStChanged = false;       //状态变化了才上报
        QFileInfo fileInfo(diskInfos[i].baseInfo.mountPath);
        QString fileName = diskInfos[i].baseInfo.mountPath[diskInfos[i].baseInfo.mountPath.length()-1]=='/'? diskInfos[i].baseInfo.mountPath+"test":diskInfos[i].baseInfo.mountPath+"/test";

        QFile file(fileName);
        getDiskInfo((char*)diskInfos[i].baseInfo.mountPath.toStdString().c_str(), diskInfos[i]);

        qInfo()<<"filename:"<<fileName<<" mountpath:"<<diskInfos[i].baseInfo.mountPath;
        OMData om;
        om.uuid = uuid;
        om.devId = uuid;
        om.type="j14";

        if(!fileInfo.isDir())       //加载目录是否存在判断硬盘是否加载
        {
            if(diskInfos[i].state!=DiskStateInfo::CAN_NOT_MOUNT)
                isStChanged = true;
            diskInfos[i].state = DiskStateInfo::CAN_NOT_MOUNT;
            QString inf = "磁盘："+diskInfos[i].baseInfo.mountPath + "挂载目录不存在";
            LOG(WARNING,inf.toStdString().c_str());
        }else if(diskInfos[i].baseInfo.free_size<(QReadConfig::getInstance()->getDiskCong().minFreeSize<<20))      //硬盘空闲空间小于15G时，硬盘空间不足
        {
            if(diskInfos[i].state!=DiskStateInfo::DISK_OVER_LOAD)
                isStChanged = true;
            diskInfos[i].state = DiskStateInfo::DISK_OVER_LOAD;
            QString inf = "磁盘："+diskInfos[i].baseInfo.mountPath + "空间不足";
            LOG(WARNING,inf.toStdString().c_str());
        }else if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))         //是否能打开
        {
            //cout<<"创建文件:"<<fileName.toStdString()<<"失败"<<endl;
            if(diskInfos[i].state!=DiskStateInfo::CAN_NOT_CREATE_FILE)
                isStChanged = true;
            diskInfos[i].state = DiskStateInfo::CAN_NOT_CREATE_FILE;
            QString inf = "磁盘："+ fileName + " 创建文件失败";
            LOG(WARNING,inf.toStdString().c_str());
        }else{
            QTextStream in(&file);
            in<<"this is a test file"<<"\n";
            file.close();
            QFile::remove(fileName);
            if(diskInfos[i].state!=DiskStateInfo::DISK_NORMAL)
                isStChanged = true;
            diskInfos[i].state = DiskStateInfo::DISK_NORMAL;
            QString inf = "磁盘："+diskInfos[i].baseInfo.mountPath + "正常";
            LOG(INFO,inf.toStdString().c_str());
            isAllBad = false;
        }
        om.status = diskInfos[i].baseInfo.mountPath +":"+ QString::number(diskInfos[i].state==DiskStateInfo::DISK_NORMAL? 1:0);

        //发送给运维
        if(isStChanged)
        {
            SendDataToOM(om);
            LOG(INFO,"发送硬盘状态信息给运维中心");
        }
    }

    //判断是否发送流媒体服务状态的信息。
    OMData om;
    om.uuid = uuid;
    om.devId = uuid;
    om.type="j9";

    StreamInfoToOM t;
    t = strToOM;
    if(isAllBad)
    {
       strToOM.disksState = StreamInfoToOM::UNORMAL;
    }else
    {
       strToOM.disksState = StreamInfoToOM::NORMAL;
    }

    if(strToOM.isStreamDown())
        om.status = QString::number(0);
    else
        om.status=QString::number(1);

    if(strToOM.isSend(t))
    {
        SendDataToOM(om);           //发送运维
        qInfo()<<"流媒体服务状态在线状态发生变化，上传运维中心";
    }else
        qInfo()<<"流媒体服务状态在线状态未发生变化，不需上传运维中心";

    sendDiskState();        //发送硬盘状态信息
     return;
}
void StreamMonitor::sendDbStatus()
{
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement message = doc.createElement("message");  //<message>
    doc.appendChild(message);
    QDomElement type = doc.createElement("TYPE");//<TYPE>
    QDomText typeContent = doc.createTextNode(QString::number(Send_DB_Info));
    type.appendChild(typeContent);
    message.appendChild(type);

    for(int i=0; i<dbStatusInfos.size(); i++)
    {
        if(dbStatusInfos[i].DBState!=DBStatusInfo::NORMAL)
        {
            QDomElement equip = doc.createElement("Equipment");//<Equipment>
            message.appendChild(equip);
            QDomElement camID = doc.createElement("CAMID");     //<CAMID>
            QDomText camid_str = doc.createTextNode(dbStatusInfos[i].CameraID);
            camID.appendChild(camid_str);
            QDomElement status = doc.createElement("STATUS ");//<STATUS>
            QDomText status_str= doc.createTextNode(QString::number(dbStatusInfos[i].DBState));
            status.appendChild(status_str);

            equip.appendChild(camID);
            equip.appendChild(status);
        }
    }

    QString DbXml = doc.toString();
    sendMsg(DbXml);
}

void StreamMonitor::sendThreadInfo(const ThreadStateInfo& thread)
{
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement message = doc.createElement("message");  //<message>
    doc.appendChild(message);
    QDomElement type = doc.createElement("TYPE");//<TYPE>
    QDomText typeContent = doc.createTextNode(QString::number(Send_Thread_Info));
    type.appendChild(typeContent);
    message.appendChild(type);

    QDomElement equip = doc.createElement("Equipment");//<Equipment>
    message.appendChild(equip);
    QDomElement thrId = doc.createElement("THRID");     //<THRID>
    QDomText thrId_str = doc.createTextNode(thread.threadId);
    thrId.appendChild(thrId_str);

    QDomElement action = doc.createElement("ACTION");   //<ACTION>
    QDomText action_str = doc.createTextNode(QString::number(thread.action));
    action.appendChild(action_str);

    QString threadXml = doc.toString();
    sendMsg(threadXml);
}

 QString StreamMonitor::getFileName(QString cmeraId)
 {
        QString dbPath = "/usr/local/"+cmeraId+"_Video.db";
         QString fileName = "";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath); // 数据库名与路径, 此时是放在同目录下
        if(db.open())
        {
            QSqlQuery query;
            if (query.exec(QString("select FILENAME from  VIDEO_RECORD")))   //尝试列出  表的所有记录
            {
                while(query.next())
                {
                    QString res = query.value(0).toString();
                    if(fileName<res)
                    {
                        fileName = res;
                    }
                }

            }else
            {
                LOG(WARNING, "查询文件名失败");
            }

        }else
        {
            LOG(WARNING, (QString("打不开数据库：")+dbPath).toStdString().c_str());
        }
        LOG(INFO, (QString("filename=")+fileName).toStdString().c_str());
        return fileName;
 }

  void StreamMonitor::sendRelVdRecState(QString cameraId)
  {
      QDomDocument doc;
      QDomProcessingInstruction instruction;
      instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
      doc.appendChild(instruction);

      QDomElement message = doc.createElement("message");  //<message>
      doc.appendChild(message);
      QDomElement type = doc.createElement("TYPE");//<TYPE>
      QDomText typeStr = doc.createTextNode(QString::number(Send_Rec_State));
      type.appendChild(typeStr);
      message.appendChild(type);

      QDomElement equip = doc.createElement("Equipment");//<Equipment>
      message.appendChild(equip);
      QDomElement camid = doc.createElement("CAMID ");//<CAMID >
      QDomText camidStr = doc.createTextNode(cameraId);
      camid.appendChild(camidStr);
      equip.appendChild(camid);

      QString relVdRecXml = doc.toString();
      sendMsg(relVdRecXml);
  }

  int  StreamMonitor::monitorRelVdWithGsoap(const CameraStateInfo &camera)
  {
      //初始化gsoap对象
      intcommonProxy p;
      p.soap_endpoint = QReadConfig::getInstance()->getGsoapInfoConf().soapEndpoint.toStdString().c_str();
      p.send_timeout=QReadConfig::getInstance()->getGsoapInfoConf().sendTimeout;
      p.recv_timeout=QReadConfig::getInstance()->getGsoapInfoConf().recvTimeout;
      p.connect_timeout=QReadConfig::getInstance()->getGsoapInfoConf().connTimeout;
      p.userid = QReadConfig::getInstance()->getGsoapInfoConf().userID.toStdString().c_str();
      p.passwd = QReadConfig::getInstance()->getGsoapInfoConf().passwd.toStdString().c_str();

  #if defined(WITH_OPENSSL)
      soap_ssl_init();
      if ( soap_ssl_client_context(&p, SOAP_SSL_NO_AUTHENTICATION, NULL, NULL, NULL, NULL, NULL) )
      {
          p.soap_stream_fault(std::cerr);
          LOG(GUZHANG,"加载客户端证书失败。");
          return 110;
      }
  #endif

      ns__HBUSERINFO nsUserInfo;
      nsUserInfo.strUser="test";
      nsUserInfo.nPermission=1;
      nsUserInfo.nCtrlTimeOut=30;

      struct ns__HBNET_VIDEO_STREAM VideoStream;
      VideoStream.szDescription = (char*)soap_malloc(&p, 256);
      strcpy(VideoStream.szDescription, camera.udpAddr.toStdString().c_str());

      if(SOAP_OK==p.SwitchCameraToStream(p.soap_endpoint, "", nsUserInfo, camera.cmareId.toLong(),&VideoStream))
      {
          qInfo()<<"摄像机"<<camera.cmareId<<"实时视频调看成功,释放请求";
          ns__Response res;
          p.AbandonCameraStream(p.soap_endpoint, "", nsUserInfo, camera.cmareId.toLong(),&res);
          qInfo()<<"retcode="<<res.retcode<<" strMessage="<<QString(res.strMessage.c_str());
          return 0;
      }else
      {
          qCritical()<<"摄像机"<<camera.cmareId<<"实时视频调看失败,释放请求";
          p.soap_stream_fault(std::cerr);
          return 1;
      }
    return 0;
  }
void StreamMonitor::hisVdReqWithTimer()
{
    if(hisReqCount==3)
    {
       hisRqTimer ->stop();
        hisReqCount=0;
        if(relAndHisVdReqStat.hisVdReq==RelAndHisVdReqStat::UNORMAL)
        {
            //重启流媒体进程
            LOG(GUZHANG,"3次历史视频调看失败,重启流媒体进程");
        }else
            qInfo()<<"第三次请求历史视频成功";
    }else
    {
        for(int i=0; i<camerasInfo.size(); i++)
        {
            if(camerasInfo[i].online)
            {
                sendHisVdRequest(camerasInfo[i].cmareId);
                hisReqCount++;
                qInfo()<<"the "<<hisReqCount<<" time to request his vd";
                break;
            }
        }
    }
}

void StreamMonitor::monitorThreads()
{
    LOG(INFO,"monitorThreadsHeart");
    for(int i=0; i<threadsInfo.size(); i++)
    {
        if((time(NULL)-threadsInfo[i].heartime)>20)
        {
            QString str = threadsInfo[i].action==1? "http线程":"磁盘检测线程";
            cout<<str.toStdString()<<" 心跳超时, 发送给流媒体处理";
            threadsInfo[i].state = ThreadStateInfo::Dead;
            sendThreadInfo(threadsInfo[i]);
        }else
            threadsInfo[i].state = ThreadStateInfo::NORMAL;
    }

}

void StreamMonitor::monitorDBStatus()
{
    LOG(INFO,"monitoring DB status");
    dbStatusInfos.clear();
    for(int i=0; i<camerasInfo.size(); i++)
    {
        QString dbPath = "/usr/local/"+camerasInfo[i].cmareId+"_Video.db";
        DBStatusInfo dbStaInfo;
        dbStaInfo.CameraID = camerasInfo[i].cmareId;
        dbStaInfo.DBState = DBStatusInfo::NORMAL;

        if(!QFile::exists(dbPath))
        {
            dbStaInfo.DBState = DBStatusInfo::DB_LOST_DB_FILE;
            LOG(WARNING, (QString("丢失数据库文件：")+dbPath).toStdString().c_str());
        }else
        {
            //1.判断能否打开数据库
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                if(db.isOpen())
                {
                    LOG(WARNING, "db already opened");
                    return;
                }
                 db.setDatabaseName(dbPath); // 数据库名与路径, 此时是放在同目录下
                 if(db.open())
                 {
                     QSqlQuery queryWrite;
                     QString insertSql = "insert into VIDEO_RECORD (CAMID,FILENAME) VALUES(11111,\"testInsert\")";
                     if(!queryWrite.exec(insertSql))
                     {
                          dbStaInfo.DBState = DBStatusInfo::DB_TABLE_LOCKED;
                          QString querys = queryWrite.lastError().text();
                          cout<<querys.toStdString();
                          LOG(WARNING, (QString("插入数据失败:") + querys).toStdString().c_str());
                     }else
                     {
                         //delete the rec insertted.
                         QSqlQuery queryDelete;
                         QString delSql = "delete from VIDEO_RECORD  where CAMID=11111 and FILENAME=\"testInsert\"";
                         if(!queryDelete.exec(delSql))
                         {
                             dbStaInfo.DBState = DBStatusInfo::DB_TABLE_LOCKED;
                             QString querys = queryDelete.lastError().text();
                             cout<<querys.toStdString();
                             LOG(WARNING, (QString("删除数据失败:") + querys).toStdString().c_str());
                         }
                     }
                 }else
                 {
                     dbStaInfo.DBState = DBStatusInfo::DB_OPEN_FAIL;
                     LOG(WARNING, (QString("打开失败的数据库文件：")+dbPath).toStdString().c_str());
                 }
        }

        dbStatusInfos.append(dbStaInfo);
    }

       //send to stream
    if(dbStatusInfos.size()>0)
       sendDbStatus();
}

 void StreamMonitor::relVdReqWithTimer()
 {
    if(relReqCount==3)
    {
        relRqTimer->stop();
        relReqCount=0;
        if(relAndHisVdReqStat.relVdReq==RelAndHisVdReqStat::UNORMAL)
        {
            //重启流媒体进程:此处只杀死流媒体进程，进程狗功能会自动重启流媒体服务
            kill_spider_backgroud((char*)QReadConfig::getInstance()->getProcDogConf().strProcName.toStdString().c_str());
            LOG(WARNING,"3次失败,重启流媒体进程");
        }
    }else
    {
        for(int i=0; i<camerasInfo.size(); i++)
        {
            if(camerasInfo[i].online)
            {
                relAndHisVdReqStat.relVdReq = monitorRelVdWithGsoap(camerasInfo[i]);
                if(relAndHisVdReqStat.relVdReq==RelAndHisVdReqStat::UNORMAL)
                {
                   relReqCount++;
                }else
                {
                   relReqCount=0;
                   relRqTimer->stop();
                }
                break;
            }
        }
    }
 }

 void StreamMonitor::sendHisVdRequest(QString cameraId)
 {
     QDomDocument doc;
     QDomProcessingInstruction instruction;
     instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
     doc.appendChild(instruction);

     QDomElement message = doc.createElement("message");  //<message>
     doc.appendChild(message);
     QDomElement type = doc.createElement("TYPE");//<TYPE>
     QDomText typeStr = doc.createTextNode(QString::number(Send_HisVd_Req));
     type.appendChild(typeStr);
     message.appendChild(type);

     QDomElement equip = doc.createElement("Equipment");//<Equipment>
     message.appendChild(equip);
     QDomElement camid = doc.createElement("CAMID ");//<CAMID >
     QDomText camidStr = doc.createTextNode(cameraId);
     camid.appendChild(camidStr);
     equip.appendChild(camid);

     QString relVdRecXml = doc.toString();
     sendMsg(relVdRecXml);
 }

void StreamMonitor::checkHisURL(QString httpUrl)
{
    qheader.setUrl(httpUrl);
    long int DownedSize = 86;       //只需要下载文件的前86个字节即可
    QString Range="bytes "+QString::number(DownedSize)+"-";//告诉服务器从DownedSize起开始传输
    qheader.setRawHeader("Range",Range.toLatin1());

    reply = manager->get(QNetworkRequest(qheader));
    qInfo()<<"下载历史视频流的前86字节";

    //关联下载完成后的信号和槽
    connect(reply,SIGNAL(finished()),this,SLOT(httpFinished()));
    //关联有可用数据是的信号和槽
    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
}

//接收历史调看视频文件
void StreamMonitor::httpReadyRead()
{
    QByteArray data = reply->read(100);
    qInfo()<<"收到历史视频流数据："<<data.toHex();
    if(data[0]==0&&data[1]==0&&data[14]==0&&data[15]==0&&data[32]==0&&data[33]==0&&data[62]==0&&
        data[63]==0&&data[81]==0&&data[82]==0&&data[83]==0&&
        data[2]==1&&data[16]==1&&data[34]==1&&data[64]==1&&data[84]==1&&
        data[3]==0xba&&data[17]==0xbb&&data[35]==0xbc&&data[65]==0xe0&&(data[85]&0x1f)==7)
    {
        relAndHisVdReqStat.hisVdReq = RelAndHisVdReqStat::NORMAL;
        hisReqCount=0;
        hisRqTimer->stop();
        qInfo()<<"请求历史视频成功";
    }else
    {
        relAndHisVdReqStat.hisVdReq = RelAndHisVdReqStat::UNORMAL;
        if(hisReqCount==0)
        {
            hisRqTimer->start(2000);
            qInfo()<<"请求历史视频失败，开启计时器，重复请求";
        }else
            {
            hisReqCount++;
            qInfo()<<"第"<<hisReqCount<<"次请求历史视频";
        }

    }
}

void StreamMonitor::httpFinished()
{
    qInfo()<<"接收历史视频流结束";
    reply->deleteLater();
}

 void StreamMonitor::monitorRelAndHisVdReq()
  {
      //1.实时视频调看，通过gsoap接口调看
     qInfo()<<"实时视频调看监控开始";
     if(relReqCount==0)                       //判断如果count不为0，说明正在用定时器重复请求中，此次不请求
     {
         for(int i=0; i<camerasInfo.size(); i++)
         {
             if(camerasInfo[i].online)
             {
                 relAndHisVdReqStat.relVdReq = monitorRelVdWithGsoap(camerasInfo[i]);
                 if(relAndHisVdReqStat.relVdReq==RelAndHisVdReqStat::UNORMAL)
                 {
                    qWarning()<<"调看摄像机："<<camerasInfo[i].cmareId<<"实时视频失败,启动定时器，请求三次";
                    relRqTimer->start(2000);
                 }else
                 {
                     qWarning()<<"调看摄像机："<<camerasInfo[i].cmareId<<"实时视频成功";
                     relReqCount=0;
                 }
                 break;
             }
         }
     }

     //2.历史视频调看
     //发送历史视频调看请求
     qInfo()<<"发送历史视频调看请求，开始历史视频调看状态监控";
     for(int i=0; i<camerasInfo.size(); i++)
     {
         if(camerasInfo[i].online)
         {
            qInfo()<<"请求摄像机"<<camerasInfo[i].cmareId<<"的历史视频";
            sendHisVdRequest(camerasInfo[i].cmareId);
            break;
         }
     }

  }

void StreamMonitor::monitorCamera()
{
   if( QReadConfig::getInstance()->getCameraSvrConf().bOpen)
   {
        LOG(INFO,"monitoring DiskInfo First....");
        monitorDiskInfo();
        LOG(INFO,"monitoring Camera info now");
        for(int i=0; i<camerasInfo.count(); i++)
        {
            if(camerasInfo.at(i).online)
            {
                //历史文件正确性检测，不是由监控主动发起，是由流媒体发送，出发监控进行监控,此处只检测当前录制的文件状态是否正常
                 //将录制状态计入内存;
                //若不能录制，重启录制线程
                QString recingFilePath = getFileName(camerasInfo.at(i).cmareId);
                QFileInfo fileInfo(recingFilePath);
                if(fileInfo.exists())
                {
                    if(abs(fileInfo.lastModified().toTime_t()-time(NULL))<120)  //修改时间在两分钟内
                    {
                        camerasInfo[i].relVdSta = CameraStateInfo::UNNORMAL;
                        QString inf = "摄像机："+camerasInfo[i].cmareId +"实时视频录制有问题,通知流媒体重启线程";
                        LOG(WARNING,inf.toStdString().c_str());
                        //有问题，通知流媒体，重启线程
                        sendRelVdRecState(camerasInfo.at(i).cmareId);
                    }else
                    {
                        camerasInfo[i].relVdSta = CameraStateInfo::NORMAL;
                        QString inf = "摄像机："+camerasInfo[i].cmareId +"实时视频录制正常";
                        LOG(INFO,inf.toStdString().c_str());
                    }
                }else
                {
                    qWarning()<<"未找到摄像机 "<<camerasInfo[i].cmareId<<" 当前正在录制的文件名";
                }
            }else
            {
                 camerasInfo[i].relVdSta = CameraStateInfo::NOT_ONLINE;
                 QString inf = "摄像机："+camerasInfo[i].cmareId +"不在线";
                 LOG(INFO,inf.toStdString().c_str());
            }
        }
   }
  // printInfo();
}

void StreamMonitor::printDiskInfo()
{
    for(int i=0; i<diskInfos.size(); i++)
    {
         qInfo()<<"路径为"<<diskInfos[i].baseInfo.mountPath<<"的硬盘状态:"<<diskErrInfoMap.value(diskInfos[i].state);
    }
}

void StreamMonitor::printCameraInfo()
{
    for(int i=0; i<camerasInfo.size(); i++)
    {
        qInfo()<<"*****************camera:"<<camerasInfo[i].cmareId<<" info***********";
        qInfo()<<"在线状态:"<<(camerasInfo[i].online? "在线":"不在线");
        qInfo()<<"实时视频录制状态："<<relVdRecErrInfoMap[camerasInfo[i].relVdSta];
        qInfo()<<"历史文件路径:"<<camerasInfo[i].hisVdSta.hisVdPath<<"   文件状态:"<<hisFileErrInfoMap[camerasInfo[i].hisVdSta.state];
    }
}
void StreamMonitor::printRelVdReqInfo()
{
    qInfo()<<"*****************real Vedio request info***********";
    qInfo()<<"实时视频调看状态："<<relHisVdReqErrInfoMap[relAndHisVdReqStat.relVdReq];
}

void StreamMonitor::printHisVdReqInfo()
{
    qInfo()<<"*****************his Vedio request info***********";
    qInfo()<<"历史视频调看状态："<<relHisVdReqErrInfoMap[relAndHisVdReqStat.hisVdReq];
}

void StreamMonitor::printDbStatusInfo()
{
    for(int i=0; i<dbStatusInfos.size(); i++)
    {
        qInfo()<<"数据库"<<dbStatusInfos[i].CameraID<<" 状态 "<<DbStatusErrInfoMap[dbStatusInfos[i].DBState];
    }
}

void StreamMonitor::printThreadsInfo()
{
    LOG(INFO,"**********线程状态************");
    for(int i=0; i<threadsInfo.size(); i++)
    {
        qInfo()<<"线程id:"<<threadsInfo[i].threadId<<" 线程类型:"
           <<(threadsInfo[i].action==1? "http线程":"磁盘检测线程")<<
           " 状态:"<<threadErrorInfoMap[threadsInfo[i].state];
    }
}

void StreamMonitor::printInfo()
{
}
