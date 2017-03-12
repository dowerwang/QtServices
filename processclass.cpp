#include "processclass.h"

ProcessClass::ProcessClass(QObject *parent) : QObject(parent)
{
    readTimeTimer = new QTimer(this);
    reConnectTimer = new QTimer(this);
    detectDisconnectTimer = new QTimer(this);
    //slaveTimeSet = new QTimer(this);
    //初始化缓冲区 1K
    comBuffer = new CCycleBuffer(1024*100);
    sendDataBuffer = new CCycleBuffer(1024*100);
    tcpClient=NULL;
    connect(this,SIGNAL(sigAnalysisData()),this,SLOT(slotAnalysisData()));
}
ProcessClass::~ProcessClass()
{

}
void ProcessClass::setQTcpSocket(QTcpSocket *tcpclient)
{
    tcpClient = tcpclient;
    if(tcpClient != NULL)
    {
        connect(tcpClient,SIGNAL(connected()),this,SLOT(connectedToServer()));
        connect(tcpClient,SIGNAL(disconnected()),this,SLOT(connectionClosedByServer()));
        connect(tcpClient,SIGNAL(readyRead()),this,SLOT(receiveData()));
        connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error()));
    }
}

void ProcessClass::connectedToServer()
{
    reConnectTimer->stop();
    QLOG_INFO()<<"tcp connected";

}
//接收数据并写入缓冲区
void ProcessClass::receiveData()
{
    static QByteArray data=NULL;
    static quint64 dataLength=0;

    while(tcpClient->bytesAvailable()>0||(!data.isEmpty()))
    {
        //写入缓冲区
        if (!comBuffer->isFull())
        {
            if(data.isEmpty())
            {
                data = tcpClient->readAll();
            }
            dataLength = comBuffer->write(data.data(), data.length());
            QLOG_INFO()<<"receive data and write buffer.数据："+data.toHex();
            if(dataLength!=data.length())
            {

                data.remove(0,dataLength);
                emit sigAnalysisData();
                QLOG_WARN()<<"buffer is full，emit signal to read buf.";
                break;
            }
            else
            {
                data.clear();
                //QLOG_INFO()<<"Data written to complete. ";
            }
        }
        else//如果没有这个等待，会有很多空循环，CPU会很高
        {
            comBuffer->waitNotFull();
            emit sigAnalysisData();
            QLOG_WARN()<<"buffer is full，emit signal to read buf.";
        }
    }
    //QLOG_INFO()<<"emit signal to read buf.";
    emit sigAnalysisData();

}
void ProcessClass::connectionClosedByServer()
{
    /*
    readTimeTimer->stop();//关闭定时读取时间（心跳）定时器
    detectDisconnectTimer->stop();//关闭断线检测定时器，否则会一直进入这个槽导致无法重新连接
    slaveTimeSet->stop();
    if(tcpClient)
    {
        if(tcpClient->isOpen())
        {
            tcpClient->close();//关闭连接
        }
    }
    reConnectTimer->start(reConnectTime);//延时重新连接*/
    QLOG_INFO()<<"TCP disconnected!.";
}
void ProcessClass::error()
{
    tcpClient->close();//关闭连接
    tcpClient->disconnectFromHost();
    //reConnectTimer->start(reConnectTime);
    QLOG_INFO()<<"tcp connection error";
}
void ProcessClass::reConnect()
{
    /*
    if(equParaData.slaveStatus.connectStatus!=1)
    {
        //reConnectTimer->stop();
        QLOG_INFO()<<"TCP reconnecting...";
        //connectToServer();

    }
    else if(equParaData.slaveStatus.connectStatus==1)
    {
        reConnectTimer->stop();
    }*/
}
//分析接收的数据
void ProcessClass::slotAnalysisData()
{
    QLOG_INFO()<<"AnalysisData...";
    /*
    //static bool flag=false;
    static quint8 startBuf=0;
    static TcpFuncCode func=func_tcp_null;
    static char dbuf[1000];//读取控制器记录最多230个字节
    static quint8 ret=1;
    while(!comBuffer->isEmpty())
    {
        if (!comBuffer->isEmpty())
        {
            //处理完一帧数据后func置0 重新读取功能码
            if(func == 0)
            {
                comBuffer->read((char*)&dbuf[0], 1);
                func=(TcpFuncCode)((quint8)dbuf[0]-KEY);

                //QLOG_INFO()<<"func:"<<func;

                //计算需要读取的数据的个数
                if(func == func_tcp_read_msg)
                {
                    comBuffer->read((char*)&dbuf[1], 1);
                    blockSize = (quint8)dbuf[1];
                    startBuf = 2;
                }
                else if(func ==func_tcp_read_led)
                {
                    comBuffer->read((char*)&dbuf[1], 3);
                    blockSize = (quint8)dbuf[3]+1;
                    startBuf = 4;
                }
                else if((func == func_tcp_read_hy_sound)||(func == func_tcp_read_syz_sound)
                        ||(func == func_tcp_read_whz_sound)||(func == func_tcp_read_timeoutWarn_sound)
                        ||(func == func_tcp_read_timeoutTiShi_sound)||(func == func_tcp_read_outDoor_sound)
                        ||(func == func_tcp_read_openDoor_sound)||(func == func_tcp_read_failLock_sound)
                        ||(func == func_tcp_read_laDoor_sound))
                {
                    comBuffer->read((char*)&dbuf[1], 6);
                    quint8 sizeH = (quint8)dbuf[6];
                    quint8 sizeL = (quint8)dbuf[5];
                    blockSize = sizeH;
                    blockSize = ((blockSize<<8)|sizeL)+5;
                    startBuf = 7;
                }
                else if(func == func_tcp_read_key)
                {
                    comBuffer->read((char*)&dbuf[1], 2);
                    blockSize = (quint16)((dbuf[2]<<8)|(dbuf[1]))*16 + 1;
                    startBuf = 3;
                }
                else
                {
                    blockSize = convertTcpFuncToReceiveDataCount(func);
                    startBuf = 1;
                }
            }
            //QLOG_INFO()<<"blockSize:"<<blockSize;
            //QLOG_INFO()<<"startBuf:"<<startBuf;
            if(comBuffer->getFreeSize()>=blockSize)
            {

                //读取剩余数据并处理
                comBuffer->read((char*)&dbuf[startBuf],blockSize);
                for(i=0;i<funcTcpCountNum;i++)
                {
                    if(func==funcTcpArray[i].ucFunctionCode)
                    {
                        ret=funcTcpArray[i].pxHandler(dbuf,&this->equParaData);
                        break;//结束循环
                    }
                }
                if(selectStatus)
                {
                    //用读取时间返回数据作为心跳包
                    switch(func)
                    {
                    case func_tcp_read_slave_para:
                        emit sigUpdateTSlave(this);
                        emit sigRefreshSlavePara(this->GetEquAll());
                        break;
                    case func_tcp_read_slave_time:
                        //停止断线检测定时器
                        detectDisconnectTimer->stop();
                        //启动时间
                        slaveTimeSet->start(Addsecond);

                        emit sigRefreshTime(this->GetEquAll());
                        break;
                    case func_tcp_openDoor:
                        //发送开门完成信号
                        emit sigOpenDoorDone(ret);
                        break;
                    case func_tcp_closeLock:
                        //发送闭锁\开锁完成信号和前一个状态值
                        emit sigCloseLockRet(ret,this->equParaData.slaveStatus.lockStatus);
                        break;
                    case func_tcp_read_work_para:
                        //更新数据库
                        emit sigRefreshFangHuCangPara(this->GetEquAll());
                        emit sigDataUpdateToDataBase(this);
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_read_lamp_Para:
                        //更新数据库
                        emit sigRefreshZhaoMingPara(this->GetEquAll());
                        emit sigDataUpdateToDataBase(this);
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_sd_slave_status:
                    {
                        //更新显示
                        emitRefreshFormAndStatusChangedSignals();
                        emit sigRefreshStatus(this->GetEquAll());
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                    }
                        break;
                    case func_tcp_read_alarmStatus:
                        emit sigRefreshStatus(this->GetEquAll());
                        break;
                    case func_tcp_sd_slave_alarmStatus:
                        emit sigRefreshStatus(this->GetEquAll());
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        emit sigSlaveWarm(this);
                        break;
                    case func_tcp_read_alarmPara:
                        emit sigRefreshAlarmPara(this->GetEquAll());
                        emit sigDataUpdateToDataBase(this);
                        break;
                    case func_tcp_sd_slave_fangQieWarning:
                        emit sigRefreshStatus(this->GetEquAll());
                        emit sigSlaveWarm(this);
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_sd_slave_buttonWarning:
                        emit sigRefreshStatus(this->GetEquAll());
                        emit sigSlaveWarm(this);
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_sd_slave_temperature:
                        emit sigRefreshStatus(this->GetEquAll());
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_read_msgCount:
                        this->equParaData.resetFlag();
                        break;
                    case func_tcp_read_msg:
                        this->equParaData.resetFlag();
                        break;
                    case func_tcp_read_led:
                        //更新数据库
                        //QLOG_INFO()<<"read led text";
                        emit sigRefreshLedPara(this->GetEquAll());
                        emit sigDataUpdateToDataBase(this);
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_set_hy_volume://
                        emit sigRefreshVoicePara(this->GetEquAll());
                        break;
                    case func_tcp_read_hy_volume:
                        emit sigRefreshVoicePara(this->GetEquAll());
                        if(useModule)
                        {useModule = 0;}
                        QLOG_INFO()<<"read sound volume end";
                        break;
                    case func_tcp_read_hy_md5://

                    case func_tcp_read_syz_md5://

                    case func_tcp_read_whz_md5://

                    case func_tcp_read_timeoutWarn_md5://

                    case func_tcp_read_timeoutTiShi_md5://

                    case func_tcp_read_outDoor_md5://

                    case func_tcp_read_openDoor_md5:

                    case func_tcp_read_failLock_md5:

                    case func_tcp_read_laDoor_md5:
                        //更新数据库
                        emit sigDataUpdateToDataBase(this);
                        emit sigRefreshVoicePara(this->GetEquAll());
                        if(useModule)
                        {useModule = 0;}
                        break;
                        break;
                    case func_tcp_read_hy_sound://

                    case func_tcp_read_syz_sound://

                    case func_tcp_read_whz_sound://

                    case func_tcp_read_timeoutWarn_sound://

                    case func_tcp_read_timeoutTiShi_sound://

                    case func_tcp_read_outDoor_sound://

                    case func_tcp_read_openDoor_sound:

                    case func_tcp_read_failLock_sound:

                    case func_tcp_read_laDoor_sound:
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_set_lineAndAdLine_para:

                        break;
                    case func_tcp_set_led:
                        QLOG_INFO()<<"set led text return";
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_set_alarmPara:
                        QLOG_INFO()<<"set alarmPara return";
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_read_key:
                        equParaData.readedKey=0;
                        break;
                    case func_tcp_sd_addkey_value:
                        QLOG_INFO()<<"slave add Key";
                        sigAddKey(equParaData.keyList.last());
                        break;
                    case func_tcp_sd_inkey_value:
                        //QLOG_INFO()<<"弹出有人进门窗口";
                        //emit sigShowIndoorWindow();
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    switch(func)
                    {
                    case func_tcp_read_slave_para:
                        emit sigUpdateTSlave(this);
                        break;
                    case func_tcp_read_slave_time:
                        //停止断线检测定时器
                        detectDisconnectTimer->stop();
                        //启动时间
                        slaveTimeSet->start(Addsecond);
                        break;
                    case func_tcp_read_work_para:
                        //更新数据库
                        emit sigDataUpdateToDataBase(this);
                        break;
                    case func_tcp_read_lamp_Para:
                        //更新数据库
                        emit sigDataUpdateToDataBase(this);
                        break;
                    case func_tcp_sd_slave_status:
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_sd_slave_alarmStatus:
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_sd_slave_temperature:
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_read_msgCount:
                        this->equParaData.resetFlag();
                        break;
                    case func_tcp_read_msg:
                        this->equParaData.resetFlag();
                        break;
                    case func_tcp_read_led:
                        //更新数据库
                        emit sigDataUpdateToDataBase(this);
                        break;
                    case func_tcp_set_hy_volume://

                    case func_tcp_read_hy_volume:

                    case func_tcp_read_hy_md5://

                    case func_tcp_read_syz_md5://

                    case func_tcp_read_whz_md5://

                    case func_tcp_read_timeoutWarn_md5://

                    case func_tcp_read_timeoutTiShi_md5://

                    case func_tcp_read_outDoor_md5://

                    case func_tcp_read_openDoor_md5:

                    case func_tcp_read_failLock_md5:

                    case func_tcp_read_laDoor_md5:
                        //更新数据库
                        emit sigDataUpdateToDataBase(this);
                        break;
                    case func_tcp_sd_slave_buttonWarning:
                    case func_tcp_sd_slave_fangQieWarning:
                        emit sigSlaveWarm(this);
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    default:
                        break;
                    }
                }
                this->GetEquAll()->tempFangHuCangLogList.clear();
                func=func_tcp_null;//表示数据处理一次完毕
            }

        }
        else//如果没有这个等待，就会有很多空循环，CPU会很高。
        {
            comBuffer->waitNotEmpty();
        }
    }*/
}
