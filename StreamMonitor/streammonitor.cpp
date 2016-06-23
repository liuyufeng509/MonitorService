#include "streammonitor.h"
#include <iostream>
#include "../config/qreadconfig.h"
#include"localclient.h"
#include<QFile>
#include <QTextStream>
#include <QDomDocument>

using namespace std;

StreamMonitor::StreamMonitor(QObject *parent) : QObject(parent)
{
    diskErrInfoMap.insert(DiskStateInfo::CAN_NOT_CREATE_FILE, "不能创建文件"); //向map里添加一对“键-值”
    diskErrInfoMap.insert(DiskStateInfo::CAN_NOT_MOUNT, "硬盘未挂载"); //向map里添加一对“键-值”
    diskErrInfoMap.insert(DiskStateInfo::DISK_OVER_LOAD, "硬盘满"); //向map里添加一对“键-值”
    diskErrInfoMap.insert(DiskStateInfo::DISK_NORMAL, "硬盘状态正常"); //向map里添加一对“键-值”

    hisFileErrInfoMap.insert(HisVdFileState::NORMAL, "历史文件正常"); //向map里添加一对“键-值”
    hisFileErrInfoMap.insert(HisVdFileState::UNNORMAL, "历史文件异常"); //向map里添加一对“键-值”

    relVdRecErrInfoMap.insert(CameraStateInfo::NORMAL, "实时视频录制正常"); //向map里添加一对“键-值”
    relVdRecErrInfoMap.insert(CameraStateInfo::UNNORMAL, "实时视频录制异常"); //向map里添加一对“键-值”
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
            case 1:
                break;
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
    type.setNodeValue(QString::number(Send_Disk_Info));
    message.appendChild(type);
    QDomElement equip = doc.createElement("Equipment");//<Equipment>
    message.appendChild(equip);
    QDomElement status = doc.createElement("Status");//<Status>
    status.setNodeValue(QString::number(diskInfo.state));
    equip.appendChild(status);

    QString diskXml = doc.toString();
    QByteArray datas;
     qint32 size=diskXml.length();
    char data[4];
    memset(data, 0, 4);
    memcpy(data, &size, sizeof(qint32));
    datas.append(QByteArray(data, 4));
    datas.append(diskXml.toLatin1());
    LocalClient::getInstance()->writeData(datas);
}

void StreamMonitor::monitorDiskInfo()
{
    //1.检测硬盘是否可以创建文件
    QFile file(QReadConfig::getInstance()->getDiskCong().tmpFilePath);
     if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))
     {
        cout<<"创建文件:"<<QReadConfig::getInstance()->getDiskCong().tmpFilePath.toStdString()<<"失败"<<endl;
        diskInfo.state = DiskStateInfo::CAN_NOT_CREATE_FILE;
        //send msg to stream Service;
        sendDiskState();
        return;
     }else
     {
         QTextStream in(&file);
         in<<"this is a test file"<<"\n";
         file.close();
         QFile::remove(QReadConfig::getInstance()->getDiskCong().tmpFilePath);
         diskInfo.state = DiskStateInfo::DISK_NORMAL;
         sendDiskState();
         return;
     }

     //2.检测硬盘是否挂载

     //3.检测硬盘是否满溢--调用张震接口

    ////////////////////////////////////////////////////////////
     diskInfo.state = DiskStateInfo::DISK_NORMAL;
     sendDiskState();
     return;
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
                //判断摄像机是否在录制文件
                //将录制状态计入内存
                //若不能录制，重启录制进程(录制进程如何获取？)
            }
        }
   }
  // printInfo();
}

void StreamMonitor::printDiskInfo()
{
    cout<<"当前硬盘状态:"<<diskErrInfoMap.value(diskInfo.state).toStdString()<<endl;
}

void StreamMonitor::printInfo()
{
}
