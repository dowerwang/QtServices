#ifndef MAINPROCESS_H
#define MAINPROCESS_H

#include <QObject>
#include "mytcpserver.h"
#include "processclass.h"
#include "./modbus/modbus.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <qsqltablemodel.h>
#include <QSqlError>
#include <QAbstractTableModel>
#include <../Para/paranetwork.h>
class MainProcess : public QObject
{
    Q_OBJECT
public:
    explicit MainProcess(QObject *parent = 0);
    QList<ProcessClass *> processList;
private:
    MyTcpServer *tcpServer;
    //数据库
    QSqlDatabase database;
    QSqlQuery *query;
    ParaNetwork paraNetwork;
signals:

public slots:
    void ClientConnect(int handle,QTcpSocket *client);
};

#endif // MAINPROCESS_H
