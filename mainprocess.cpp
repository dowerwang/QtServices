#include "mainprocess.h"
#include "./AppConfig/myapp.h"
MainProcess::MainProcess(QObject *parent) : QObject(parent)
{
    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("localhost");
    database.setDatabaseName("scada3000");//已建立的数据库名称
    database.setUserName("root");//用户名称
    database.setPassword("123");//密码

    //打开数据库
    if(!database.open())
    {
        QLOG_FATAL() <<"数据库文件打开错误！"+database.lastError().text();
    }
    query = new QSqlQuery(database);
    //读取分支树父辈
    if(!query->exec("select * from para_network"))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            paraNetwork.No=query->value(0).toInt();             //序号
            paraNetwork.Guid=query->value(1).toString();       //全局ID
            paraNetwork.Name=query->value(2).toString();       //名称
            paraNetwork.Father=query->value(3).toString();     //父类
            paraNetwork.Systype=query->value(4).toInt();        //系统类型
            paraNetwork.Devtype=query->value(5).toInt();        //设备类型
            paraNetwork.Servertype=query->value(6).toInt();     //服务类型
            paraNetwork.Oppip=query->value(7).toInt();          //对端IP
            paraNetwork.Oppport=query->value(8).toInt();        //对端端口
            paraNetwork.Myip=query->value(9).toInt();           //自身IP
            paraNetwork.muport=query->value(10).toInt();         //自身端口
            paraNetwork.Sysprototal=query->value(11).toInt();    //系统规约
            paraNetwork.sonprototal=query->value(12).toInt();    //分支规约
            paraNetwork.Prototalpara=query->value(13).toInt();   //规约参数
            paraNetwork.Sendno=query->value(14).toInt();         //发送表数
            paraNetwork.Sendid=query->value(15).toInt();         //发送表
            paraNetwork.Ifeffect=query->value(16).toInt();       //有效
            paraNetwork.Ifrun=query->value(17).toInt();          //运行
            paraNetwork.Master=query->value(18).toInt();         //主通道
            paraNetwork.Salve=query->value(19).toInt();          //备通道
            paraNetwork.iftime=query->value(20).toInt();         //允许校时
            paraNetwork.Ifyaokong=query->value(21).toInt();      //允许遥控
            paraNetwork.Yaocetype=query->value(22).toInt();      //遥测方式
            paraNetwork.Yaoxintype=query->value(23).toInt();     //遥信方式
            paraNetwork.Yaomaitype=query->value(24).toInt();     //遥脉方式
            paraNetwork.Yaokongtype=query->value(25).toInt();    //遥控方式
            paraNetwork.Yaotiaogtype=query->value(26).toInt();   //遥调方式
            paraNetwork.Yaoshitype=query->value(27).toInt();     //遥视方式
            paraNetwork.syn=query->value(28).toInt();            //同异步
            QLOG_INFO()<<"通信参数："+paraNetwork.Guid;
        }
    }

    /*
    tcpServer = new MyTcpServer(myApp::TcpPort.toInt(),this);
    connect(tcpServer,SIGNAL(ClientConnect(int,QTcpSocket *)),this,SLOT(ClientConnect(int,QTcpSocket *)));
    if (!tcpServer->isListening()) {
        QLOG_INFO()<<QString("Failed to bind to port %1").arg(tcpServer->serverPort());
    }
    */
}
void MainProcess::ClientConnect(int handle,QTcpSocket *client)
{
    ProcessClass* temp = new ProcessClass();
    temp->setQTcpSocket(client);
    processList.append(temp);
    QLOG_INFO()<<"接收新连接："+client->peerAddress().toString()+"端口："+QString::number(client->peerPort());
}
