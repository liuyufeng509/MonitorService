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
using namespace std;

StreamMonitor::StreamMonitor(QObject *parent) : QObject(parent)
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
}
//解析xml协议
void StreamMonitor::doParseXml(QString xml)
{
    cout<<"rcv xml msg:" <<endl;
    cout<<xml.toStdString()<<endl;
    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(xml, false, &error, &row, &column))     //加载xml内容
    {
        cout<<(QString("parse file failed at line row and column") + QString::number(row, 10) + QString(",") + QString::number(column, 10)).toStdString()<<endl;
        return ;
    }
    if(document.isNull())
    {
        cout<<"document is null"<<endl;
        return ;
    }

    //解析
    QDomElement message = document.documentElement();        //<message>
    QDomElement type = message.firstChildElement();		//<TYPE>标签
    cout<<"type="<<type.text().toInt()<<flush;

     switch (type.text().toInt()) {
    case Rcv_Cameras_Info:
    {
        cout<<" Cameras List Parse"<<endl;
        QDomElement action = message.firstChildElement("ACTION");          //<ACTION>
        cout<<"action(1,init; 2,add; 3,del)="<<action.text().toInt()<<endl;
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
            cout<<"cameraId="<<cameraId.text().toStdString()<<endl;
            switch (action.text().toInt())
            {
            case 1:         //init
            case 2:         //add
            {
                if(!camerasInfo.contains(camInfo))
                {
                    camerasInfo.append(camInfo);
                }
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
        cout<<" Cameras online Parse"<<endl;
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
            cout<<"cameraId="<<cameraId.text().toStdString()<<" status(0 normal)="<<status.text().toInt()<<endl;
        }
    }
        break;
    case RcV_Threads_Info:
    {
        cout<<" threads list Parse"<<endl;
        //<Equipment>
        QDomNodeList equipments = message.elementsByTagName("Equipment");
        int nCount = equipments.count();
        for(int i=0; i<nCount; i++)
        {
            QDomNode eqNode = equipments.item(i);
            QDomElement equipment = eqNode.toElement();         //<EquipMent>
            QDomElement threadId = equipment.firstChildElement(); //<THRID>
            QDomElement action = equipment.lastChildElement();          //<ACTION>
            ThreadStateInfo threadInfo;
            threadInfo.threadId = threadId.text();
            threadInfo.action = action.text().toInt();
            if(!threadsInfo.contains(threadInfo))
            {
                threadsInfo.append(threadInfo);
            }else
            {
                threadsInfo[threadsInfo.indexOf(threadInfo)].action = threadInfo.action;
            }
            cout<<"threadId="<<threadId.text().toStdString()<<" action="<<action.text().toInt()<<endl;
        }
    }
        break;
    case Rcv_HisFile_Info:
    {
        cout<<" HisFile Info Parse"<<endl;
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
            cout<<"cameraId="<<cameraId.text().toStdString()
               <<" filepath="<<filepath.text().toStdString()<<endl;
        }
    }
        break;
    case Rcv_Thread_Heart:
    {
        cout<<" Thread Heart Parse"<<endl;
        //<Equipment>
        QDomNodeList equipments = message.elementsByTagName("Equipment");
        int nCount = equipments.count();
        for(int i=0; i<nCount; i++)
        {
            QDomNode eqNode = equipments.item(i);
            QDomElement equipment = eqNode.toElement();         //<EquipMent>
            QDomElement threadId = equipment.firstChildElement(); //<THRID>
            cout<<"threadId="<<threadId.text().toStdString()<<endl;
        }
    }
        break;
    case Rcv_RelVd_Res:
    {
        cout<<" Rel Vedeo Res Parse"<<endl;
        //<Equipment>
        QDomNodeList equipments = message.elementsByTagName("Equipment");
        int nCount = equipments.count();
        for(int i=0; i<nCount; i++)
        {
            QDomNode eqNode = equipments.item(i);
            QDomElement equipment = eqNode.toElement();         //<EquipMent>
            QDomElement cameraId = equipment.firstChildElement(); //<CAMID>
            QDomElement status = equipment.lastChildElement();          //<Status>
            cout<<"cameraId="<<cameraId.text().toStdString()<<" status(0 normal)="<<status.text().toInt()<<endl;
        }
    }
        break;
    case Rcv_HisVd_Res:
    {
        cout<<" Rel Vedeo Res Parse"<<endl;
        //<Equipment>
        QDomNodeList equipments = message.elementsByTagName("Equipment");
        int nCount = equipments.count();
        for(int i=0; i<nCount; i++)
        {
            QDomNode eqNode = equipments.item(i);
            QDomElement equipment = eqNode.toElement();         //<EquipMent>
            QDomElement cameraId = equipment.firstChildElement(); //<CAMID>
            QDomElement status = equipment.lastChildElement();          //<Status>
            cout<<"cameraId="<<cameraId.text().toStdString()<<" status(0 normal)="<<status.text().toInt()<<endl;
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
        QDomText mPath_str= doc.createTextNode(diskInfos[i].mountPath);
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
    diskInfos.clear();
    for(int i=0; i<QReadConfig::getInstance()->getDiskCong().diskPaths.size(); i++)
    {
        DiskStateInfo disk;
        disk.mountPath = QReadConfig::getInstance()->getDiskCong().diskPaths[i];
        QFileInfo fileInfo(disk.mountPath);
        QString fileName = disk.mountPath[disk.mountPath.length()-1]=='/'? disk.mountPath+"test":disk.mountPath+"/test";
        QFile file(fileName);
        bool isOver = false; //磁盘是否满，需张震接口获取
        if(!fileInfo.isDir())       //加载目录是否存在判断硬盘是否加载
        {
            disk.state = DiskStateInfo::CAN_NOT_MOUNT;
        }else if(isOver)
        {
            disk.state = DiskStateInfo::DISK_OVER_LOAD;
        }else if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))         //是否能打开
        {
            cout<<"创建文件:"<<fileName.toStdString()<<"失败"<<endl;
            disk.state = DiskStateInfo::CAN_NOT_CREATE_FILE;
        }else{
            QTextStream in(&file);
            in<<"this is a test file"<<"\n";
            file.close();
            QFile::remove(fileName);
            disk.state = DiskStateInfo::DISK_NORMAL;
        }
        diskInfos.append(disk);
    }
    sendDiskState();        //发送硬盘状态信息
     return;
}
 QString StreamMonitor::getFileName(QString cmeraId)
 {
//        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//        db.setDatabaseName("NEW_AR_POI.sqlite"); // 数据库名与路径, 此时是放在同目录下
//        if(db.open())
//        {
//            QSqlQuery query;
//            if (query.exec(QString("select * from  ")+cmeraId))   //尝试列出  表的所有记录
//            {
//                return "/tmp/2016-05-20/241241/1463737497.ps";
//            }else
//                return "null";
//        }else
//            return "null";

      return "/home/liuyufeng/testvd/1463737497.ps";
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

void StreamMonitor::monitorCamera()
{
   if( QReadConfig::getInstance()->getCameraSvrConf().bOpen)
   {
        cout<<"monitoring DiskInfo First...."<<endl;
        monitorDiskInfo();
        cout<<"monitoring Camera info now"<<endl;
        for(int i=0; i<camerasInfo.count(); i++)
        {
            if(camerasInfo.at(i).online)
            {
                 //将录制状态计入内存
                //若不能录制，重启录制进程(录制进程如何获取？)
                QString recingFilePath = getFileName(camerasInfo.at(i).cmareId);
                QFileInfo fileInfo(recingFilePath);
                if(fileInfo.exists())
                {
                    if(abs(fileInfo.lastModified().toTime_t()-time(NULL))<120)  //修改时间在两分钟内
                    {
                        camerasInfo[i].relVdSta = CameraStateInfo::UNNORMAL;
                        //有问题，通知流媒体，重启线程
                        sendRelVdRecState(camerasInfo.at(i).cmareId);
                    }else
                    {
                        camerasInfo[i].relVdSta = CameraStateInfo::NORMAL;
                    }
                }
            }else
            {
                 camerasInfo[i].relVdSta = CameraStateInfo::NOT_ONLINE;
            }
        }
   }
  // printInfo();
}

void StreamMonitor::printDiskInfo()
{
    for(int i=0; i<diskInfos.size(); i++)
    {
         cout<<"路径为"<<diskInfos[i].mountPath.toStdString()<<"的硬盘状态:"<<diskErrInfoMap.value(diskInfos[i].state).toStdString()<<endl;
    }
}

void StreamMonitor::printCameraInfo()
{
    for(int i=0; i<camerasInfo.size(); i++)
    {
        cout<<"*****************camera:"<<camerasInfo[i].cmareId.toStdString()<<" info***********"<<endl;
        cout<<"在线状态:"<<(camerasInfo[i].online? "在线":"不在线")<<endl;
        cout<<"实时视频录制状态："<<relVdRecErrInfoMap[camerasInfo[i].relVdSta].toStdString()<<endl;
        cout<<"历史文件路径:"<<camerasInfo[i].hisVdSta.hisVdPath.toStdString()<<"   文件状态:"<<hisFileErrInfoMap[camerasInfo[i].hisVdSta.state].toStdString()<<endl;
    }
}

void StreamMonitor::printInfo()
{
}
