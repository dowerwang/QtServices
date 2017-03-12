#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QDateTime>
#include <QStringList>
#include <QDir>
#include <QSettings>
#include "../QsLog/QsLog.h"
#include "../QsLog/QsLogDest.h"
#include "../QsLog/QsLogLevel.h"
#include "../CycleBuffer/CCycleBuffer.h"
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(quint16 port, QObject *parent = 0);
public:
    void incomingConnection(int socket);
    void pause();
    void resume();
private slots:
signals:
    void ClientConnect(int clientID,QTcpSocket *client);
private:
    bool disabled;
};

#endif // MYTCPSERVER_H
