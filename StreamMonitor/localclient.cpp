#include "localclient.h"
#include <iostream>
using namespace std;
LocalClient* LocalClient::m_pInstance = NULL;
LocalClient::LocalClient():socket(NULL)
{
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(requestConnection()));
}

void LocalClient::disConnected()
{
    qWarning()<<"connection disconnected";
    disconnect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(displayError(QLocalSocket::LocalSocketError)));
    disconnect(socket, SIGNAL(disconnected()), this, SLOT(disConnected()));
    socket->deleteLater();
    socket = NULL;
    isConnected = false;
}

void LocalClient::requestConnection()
{
    if(socket == NULL)
    {
        socket = new QLocalSocket;
        connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
                this, SLOT(displayError(QLocalSocket::LocalSocketError)));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disConnected()));
        timer->stop();
    }
    socket->connectToServer(m_servName);
    if(socket->state()==QLocalSocket::ConnectedState)
    {
        qInfo()<<"new connection to  server "<<m_servName<<" build";
        isConnected = true;
    }else
    {
        qWarning()<<"new connection to  server "<<m_servName<<" fail";
        socket->deleteLater();
        socket = NULL;
        timer->start(5000);
    }
}

bool LocalClient::writeData(QByteArray data)
{
    //if(isConnected)
    if(isConnected)
    {
        socket->write(data);
        qInfo()<<"发送数据："<<QString(data)<<endl;
        return true;
    }else
    {
        if(socket!=NULL)
            qWarning()<<"LocalClient writeData err"<<socket->errorString()<<"请求连接";
        else
            qWarning()<<"Socket is NULL";
        requestConnection();
        return false;
    }
}

void LocalClient::displayError(QLocalSocket::LocalSocketError socketError)
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
        qCritical()<<"发生如下错误:"<<socket->errorString();
       }

    //发生错误，连接可能断开
    if(socket->state()!=QLocalSocket::ConnectedState)
    {
        isConnected = false;
    }
}

LocalClient::~LocalClient()
{
    if(socket!=NULL && socket->isOpen())
    {
        socket->close();
        delete socket;
    }
}

