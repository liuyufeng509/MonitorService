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
        file.remove();
}

bool LocalServer::listen()
{
    cout<<"listenning..."<<endl;
    if(!m_server->listen(m_servName))
    {
        cout<<"启动"<<m_servName.toStdString()<<"失败："<<m_server->errorString().toStdString()<<endl;
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
        cout<<"connection has been built"<<endl;
        //只支持一路客户端，所以有新的请求来后，先接收，在断开连接
        QLocalSocket *client = m_server->nextPendingConnection();
        client->close();
        return;
    }
    m_client = m_server->nextPendingConnection();
   // m_server->close();//获取链接成功，不再监听新的链接。
    cout<<"new connection build..."<<endl;
//    connect(m_client, SIGNAL(disconnected()),
//            m_client, SLOT(deleteLater()));

    connect(m_client, SIGNAL(readyRead()), this, SLOT(readFortune()));
    connect(m_client, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(displayError(QLocalSocket::LocalSocketError)));
    connect(m_client, SIGNAL(disconnected()), this, SLOT(disConnected()));
    isConnected = true;     //与客户端链接成功。
}

void LocalServer::disConnected()
{
    cout<<"connection disconnected"<<endl;
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
    cout<<"rcv data:"<<QString(dataRcv).toStdString()<<endl;
    if(dataRcv.size()<=0)
    {
       cout<<"接收的数据为长度为0"<<endl;
       return;
    }

    datagram.append(dataRcv);       //收到就放到数据报文中

    if(dataRcv[4]=='<'&&dataRcv[5]=='?')    //如果是报文第一帧，取总长度
    {
        totalSize = dataRcv[0] & 0x000000FF;
        totalSize |= ((dataRcv[1] << 8) & 0x0000FF00);
        totalSize |= ((dataRcv[2] << 16) & 0x00FF0000);
        totalSize |= ((dataRcv[3] << 24) & 0xFF000000);
        //size 单指字符串的长度
        rcvBytes = dataRcv.size()-4;       //收到的为总长度-4个字节
    }else{
        rcvBytes += dataRcv.size();         //如果不是，拼包
    }
    if(rcvBytes==totalSize)             //全部接收完毕
    {
        QByteArray tmpArr = datagram.mid(4);
        QString data(tmpArr);
        emit emitData(data);       //分发消息
        totalSize=0;
        rcvBytes=0;
        cout<<"完整报文内容："<<QString(tmpArr).toStdString()<<endl;
        datagram.clear();
    }
}

void LocalServer::displayError(QLocalSocket::LocalSocketError socketError)
{
    switch (socketError) {
    case QLocalSocket::ServerNotFoundError:
        cout<<"host 未找到，请检查hostname和端口设置"<<endl;
        break;
    case QLocalSocket::ConnectionRefusedError:
        cout<<"链接被对端拒绝，请确认远端服务器正在运行，并检查hostname和port是否设置正确"<<endl;
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        cout<<"发生如下错误:"<<m_client->errorString().toStdString()<<endl;
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
        cout<<"发送数据："<<QString(data).toStdString()<<endl;
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


