#include "localclient.h"
#include <iostream>
using namespace std;
LocalClient* LocalClient::m_pInstance = NULL;
LocalClient::LocalClient():socket(NULL)
{
    socket = new QLocalSocket;
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(displayError(QLocalSocket::LocalSocketError)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disConnected()));
}

void LocalClient::disConnected()
{
    cout<<"connection disconnected"<<endl;
    socket->deleteLater();
    socket = NULL;
    isConnected = false;
}

void LocalClient::requestConnection()
{
    socket->connectToServer(m_servName);
    if(socket->state()==QLocalSocket::ConnectedState)
    {
        cout<<"new connection to  server "<<m_servName.toStdString()<<" build"<<endl;
        isConnected = true;
    }else
    {
        cout<<"new connection to  server "<<m_servName.toStdString()<<" fail"<<endl;
    }
}

void LocalClient::writeData(QByteArray data)
{
    //if(isConnected)
    if(socket->state()!=QLocalSocket::UnconnectedState && socket->state()!=QLocalSocket::ConnectingState)
    {
        socket->write(data);
        cout<<"发送数据："<<QString(data).toStdString()<<endl;
    }else
    {
        cout<<"LocalClient writeData err"<<socket->errorString().toStdString()<<endl;
    }
}

void LocalClient::displayError(QLocalSocket::LocalSocketError socketError)
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
        cout<<"发生如下错误:"<<socket->errorString().toStdString()<<endl;
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

