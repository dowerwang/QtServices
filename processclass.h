#ifndef PROCESSCLASS_H
#define PROCESSCLASS_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTcpServer>
#include "../CycleBuffer/CCycleBuffer.h"
#include "../QsLog/QsLog.h"
#include "../QsLog/QsLogDest.h"
#include "../QsLog/QsLogLevel.h"
#include <QTime>
#include <QDateTime>
#include <QTimer>

class ProcessClass : public QObject
{
    Q_OBJECT
public:
    explicit ProcessClass(QObject *parent = 0);
    ~ProcessClass();
    void setQTcpSocket(QTcpSocket *tcpclient);
signals:
    void sigAnalysisData();
public slots:
private:
    QTimer *readTimeTimer;//定时读取时间定时器
    QTimer *detectDisconnectTimer;//检测断线定时器
    QTimer *reConnectTimer;//断线后重连延时定时器
    static quint16 tcpPort;//tcp连接端口
    static quint16 udpPort;//udp连接端口
    static quint64 detectDisconnectTime;//心跳包超时时间
    static quint64 reConnectTime;//延时重新连接
    static quint32 readDelayTime;//读取时间间隔，类似心跳包。不能比detectDisconnectTime大

    CCycleBuffer* comBuffer;
    CCycleBuffer* sendDataBuffer;
    void writeDataToBuffer(QByteArray *datagram);
private:
    QTcpSocket *tcpClient;
    quint16 nextBlockSize;
    quint16 blockSize;
private slots:
    void connectedToServer();
    void receiveData();
    void connectionClosedByServer();
    void error();
    void reConnect();
    void slotAnalysisData();
};

#endif // PROCESSCLASS_H
