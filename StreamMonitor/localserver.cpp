#include "localserver.h"
#include <iostream>
#include <string>
#include <QFile>
using namespace std;
LocalServer* LocalServer::m_pInstance = NULL;
LocalServer::LocalServer():m_client(NULL)
{
    m_server = new QLocalServer;
    //m_server->setMaxPendingConnections(0);
}
void LocalServer::setServName(QString servName)
{
    m_servName = servName;
    QFile file(m_servName);
    if(file.exists())
    {
        qInfo()<<"文件"<<m_servName<<"已存在，删除";
        if(file.remove())
            qInfo()<<"删除成功";
        else
            qInfo()<<"删除失败";
    }
}

bool LocalServer::listen()
{
    LOG(INFO, "listenning...");
    if(!m_server->listen(m_servName))
    {
        qWarning()<<"启动"<<m_servName<<"失败："<<m_server->errorString();
        return false;
    }else{
        connect(m_server, SIGNAL(newConnection()), this, SLOT(getClientConn()));
        return true;
    }
}

void LocalServer::getClientConn()
{
    if(m_client!=NULL && m_client->isValid())
    {
        LOG(INFO,"connection has been built");
        //只支持一路客户端，所以有新的请求来后，先接收，在断开连接
        QLocalSocket *client = m_server->nextPendingConnection();
        client->close();
        return;
    }
    m_client = m_server->nextPendingConnection();
    LOG(INFO,"new connection build...");

    connect(m_client, SIGNAL(readyRead()), this, SLOT(readFortune()));
    connect(m_client, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(displayError(QLocalSocket::LocalSocketError)));
    connect(m_client, SIGNAL(disconnected()), this, SLOT(disConnected()));
    isConnected = true;     //与客户端链接成功。
}

void LocalServer::disConnected()
{
    qWarning()<<"connection disconnected";
    m_client->deleteLater();
    m_client = NULL;
    isConnected = false;
    datagram.clear();
    totalSize=0;
    rcvBytes=0;
}

void LocalServer::readFortune()
{
    QByteArray dataRcv = m_client->readAll();
    qInfo()<<"rcv data:"<<QString(dataRcv);
    if(dataRcv.size()<=0)
    {
       qInfo()<<"接收的数据为长度为0";
       return;
    }

    int rcvSize = dataRcv.size();

    if(datagram.size()==1)
    {
        datagram.append(dataRcv.mid(0, 3));          //接收上次接收的剩下的半截帧
        totalSize = datagram[0] & 0x000000FF;
        totalSize |= ((datagram[1] << 8) & 0x0000FF00);
        totalSize |= ((datagram[2] << 16) & 0x00FF0000);
        totalSize |= ((datagram[3] << 24) & 0xFF000000);
        datagram.append(dataRcv.mid(3, totalSize));          //接收上次接收的剩下的半截帧
       // qInfo()<<"size=1:"<<QString(datagram);
    }else if(datagram.size()==2)
    {
        datagram.append(dataRcv.mid(0, 2));          //接收上次接收的剩下的半截帧
        totalSize = datagram[0] & 0x000000FF;
        totalSize |= ((datagram[1] << 8) & 0x0000FF00);
        totalSize |= ((datagram[2] << 16) & 0x00FF0000);
        totalSize |= ((datagram[3] << 24) & 0xFF000000);
        datagram.append(dataRcv.mid(2, totalSize));          //接收上次接收的剩下的半截帧
        //qInfo()<<"size=2:"<<QString(datagram);
    }else if(datagram.size()==3)
    {
        datagram.append(dataRcv.mid(0, 1));          //接收上次接收的剩下的半截帧
        totalSize = datagram[0] & 0x000000FF;
        totalSize |= ((datagram[1] << 8) & 0x0000FF00);
        totalSize |= ((datagram[2] << 16) & 0x00FF0000);
        totalSize |= ((datagram[3] << 24) & 0xFF000000);
        datagram.append(dataRcv.mid(1, totalSize));          //接收上次接收的剩下的半截帧
        //qInfo()<<"size=3:"<<QString(datagram);
    }else
    {
         datagram.append(dataRcv.mid(0, rcvBytes));          //接收上次接收的剩下的半截帧
         //qInfo()<<"size="<<datagram.size()<<" :"<<QString(datagram);
    }
    if(datagram.size()>0)
    {
        qInfo()<<"完整报文内容："<<QString(datagram)<<endl;
        emit emitData(QString(datagram));
        datagram.clear();
    }
    while(1)
    {
        totalSize = 0;      //每次循环都清零
       // if(dataRcv[4+rcvBytes]=='<'&&dataRcv[5+rcvBytes]=='?')
         {
            if(rcvBytes==rcvSize-1)             //下一帧只有一个字节,
            {
                datagram.clear();
                datagram.append(dataRcv[rcvBytes]);
                qInfo()<<"rcvBytes="<<rcvBytes<<" rcvSize="<<rcvSize;
                break;

            }
            if(rcvBytes==rcvSize-2)             //下一帧只有2个字节,
            {
                datagram.clear();
                datagram.append(dataRcv[rcvBytes]);
                qInfo()<<"rcvBytes="<<rcvBytes<<" rcvSize="<<rcvSize;
                break;
            }
            if(rcvBytes==rcvSize-2)             //下一帧只有3个字节,
            {
                datagram.clear();
                datagram.append(dataRcv[rcvBytes]);
                qInfo()<<"rcvBytes="<<rcvBytes<<" rcvSize="<<rcvSize;
                break;
            }

             totalSize = dataRcv[0+rcvBytes] & 0x000000FF;
             totalSize |= ((dataRcv[1+rcvBytes] << 8) & 0x0000FF00);
             totalSize |= ((dataRcv[2+rcvBytes] << 16) & 0x00FF0000);
             totalSize |= ((dataRcv[3+rcvBytes] << 24) & 0xFF000000);
             qInfo()<<"完整报文内容："<<QString(dataRcv.mid(4+rcvBytes, totalSize))<<endl;

             emit emitData(QString(dataRcv.mid(4+rcvBytes, totalSize)));
             rcvBytes += totalSize +4;
             qInfo()<<"rcvBytes="<<rcvBytes<<" rcvSize="<<rcvSize;
         }
        if(rcvBytes==rcvSize)       //刚好接收完
        {
            rcvBytes = 0;
            qInfo()<<"rcvBytes="<<rcvBytes<<" rcvSize="<<rcvSize;
            break;
        }
        if(rcvBytes>rcvSize)        //说明还有下一个消息
        {
            datagram.clear();
            datagram.append(dataRcv.mid(rcvBytes-totalSize-4));         //把最后一帧的半截放到datagram中，下次接收，先把剩下的放到datagram中，凑成完整报文
            rcvBytes = rcvBytes-rcvSize;
            qInfo()<<"rcvBytes="<<rcvBytes<<" rcvSize="<<rcvSize;
            break;
        }
    }

    qInfo()<<"rcv data end";
}

void LocalServer::displayError(QLocalSocket::LocalSocketError socketError)
{
    switch (socketError) {
    case QLocalSocket::ServerNotFoundError:
        qCritical()<<"host 未找到，请检查hostname和端口设置";
        break;
    case QLocalSocket::ConnectionRefusedError:
        qCritical()<<"链接被对端拒绝，请确认远端服务器正在运行，并检查hostname和port是否设置正确";
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        qCritical()<<"发生如下错误:"<<m_client->errorString();
    }

    //发生错误，连接可能断开
    if(m_client->state()!=QLocalSocket::ConnectedState)
    {
        isConnected = false;
    }
}

void LocalServer::writeData(QByteArray data)
{
    //if(isConnected)
    if(m_client->state()!=QLocalSocket::UnconnectedState && m_client->state()!=QLocalSocket::ConnectingState)
    {
        m_client->write(data);
        qInfo()<<"发送数据："<<QString(data)<<endl;
    }
}

LocalServer::~LocalServer()
{
    delete m_server;
    if(m_client!=NULL && m_client->isOpen())
    {
        m_client->close();
       // delete m_client;
    }
}


