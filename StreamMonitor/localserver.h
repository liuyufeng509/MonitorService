#ifndef LOCALSERVER_H
#define LOCALSERVER_H
#include <QLocalServer>
#include <QLocalSocket>
#include <QString>

/**
 * @brief The LocalServer class
 * 该类是与流媒体服务通信的domainsocket的服务端
 * 负责监听流媒体链接，接受消息并转发给业务逻辑处理类。
 * 还负责向流媒体服务发送消息
 * 因为整个监控服务只需要一个domainsocket对象，所以可以设置成单例模式。
 *
 */

class LocalServer : public QObject
{
    Q_OBJECT
public:
    static LocalServer*  getInstance()
    {
        if(m_pInstance == NULL)
        {
            m_pInstance = new LocalServer();
        }
        return m_pInstance;
    }

    void setServName(QString servName);     //domain socket server name

    bool isConnect(){return isConnected;}

    ~LocalServer();

    bool listen();          //监听socket
    void writeData(QByteArray data);       //向流媒体服务发送消息
private:
    LocalServer();

public slots:
    void getClientConn();       //建立连接

    void readFortune();         //接受消息

    void displayError(QLocalSocket::LocalSocketError);  //捕捉错误。

    void disConnected();                                //链接断开提示

signals:
    void emitData(QString strXml);              //发送给专门的类进行处理，异步处理还是同步，看connect最后一个参数。

private:

    static LocalServer *m_pInstance;        //单例模式


    bool isConnected;
    QLocalServer *m_server;     //domainsocket 服务
    QLocalSocket *m_client;     //domainsocket
    QString       m_servName;   //domainsocket name



    QByteArray datagram;        //数据缓冲区
    int        rcvBytes;        //收到的字节数
    qint32     totalSize;       //收到的全部的字节数

};

#endif // LOCALSERVER_H
