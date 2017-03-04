#include "mytcpserver.h"

MyTcpServer::MyTcpServer(quint16 port, QObject* parent)
      : QTcpServer(parent), disabled(false)
{
    comBuffer=new CCycleBuffer(1024*1);//100K缓冲区
    disabled = false;
    listen(QHostAddress::Any, port);
}
void MyTcpServer::incomingConnection(int socket)
{
    if (disabled)
        return;
    // When a new client connects, the server constructs a QTcpSocket and all
    // communication with the client is done over this QTcpSocket. QTcpSocket
    // works asynchronously, this means that all the communication is done
    // in the two slots readClient() and discardClient().
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
    QLOG_INFO()<<"New Connection:"+s->peerAddress().toString();
}

void MyTcpServer::pause()
{
    disabled = true;
}

void MyTcpServer::resume()
{
    disabled = false;
}
void MyTcpServer::readClient()
{
    if (disabled)
        return;
    static QByteArray data=NULL;
    static quint64 dataLength=0;
    QTcpSocket* tcpClient = (QTcpSocket*)sender();
    //写入缓冲区
    if (!comBuffer->isFull())
    {
        while((tcpClient->bytesAvailable()>0||(!data.isEmpty()))&&(!comBuffer->isFull()))
        {
            if(data.isEmpty())
            {
                data = tcpClient->readAll();
            }
            dataLength = comBuffer->write(data.data(), data.length());
            QLOG_INFO()<<"receive data:"<<data.toHex();
            if(dataLength!=data.length())
            {

                data.remove(0,dataLength);
                //slotAnalysisTcpData();
                QLOG_WARN()<<"buffer is full，emit signal to read buf.";
                break;
            }
            else
            {
                data.clear();
                QLOG_INFO()<<"Data written to complete. ";
            }

        }
    }
    else//如果没有这个等待，会有很多空循环，CPU会很高
    {
        //comBuffer->waitNotFull();
        tcpClient->readAll();//丢掉接收到的数据
        //emit sigAnalysisData();
        QLOG_WARN()<<"buffer is full,emit signal to read buf.";
    }
    //slotAnalysisTcpData();
    QLOG_INFO()<<"Received Message;";
}
void MyTcpServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
    QLOG_INFO()<<"Connection closed;";
}
