#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

#include <QObject>
#include <QLocalSocket>
#include <QString>
#include "../common.h"
class LocalClient : public QObject
{
    Q_OBJECT

public:
    static LocalClient*  getInstance()
    {
        if(m_pInstance == NULL)
        {
            m_pInstance = new LocalClient();
        }
        return m_pInstance;
    }

    void setServName(QString servName)
    {
        m_servName = servName;
    }

    bool isConnect(){return isConnected;}

    ~LocalClient();
    void  requestConnection();
    void  writeData(QByteArray data);

private:
    explicit LocalClient();
signals:

public slots:
    void displayError(QLocalSocket::LocalSocketError socketError);
    void disConnected();

private:
    static LocalClient *m_pInstance;        //单例模式

    bool isConnected;

    QLocalSocket *socket;
    QString      m_servName;
};

#endif // LOCALCLIENT_H
