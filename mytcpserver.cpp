#include "mytcpserver.h"

MyTcpServer::MyTcpServer(quint16 port, QObject* parent)
      : QTcpServer(parent), disabled(false)
{
    disabled = false;
    listen(QHostAddress::Any, port);
}
void MyTcpServer::incomingConnection(int socket)
{
    if (disabled)
        return;
    QTcpSocket *client=new QTcpSocket(this);
    client->setSocketDescriptor(socket);
    emit ClientConnect(socket, client);
    //QLOG_INFO()<<"New Connection:"+client->peerAddress().toString();
}

void MyTcpServer::pause()
{
    disabled = true;
}

void MyTcpServer::resume()
{
    disabled = false;
}

