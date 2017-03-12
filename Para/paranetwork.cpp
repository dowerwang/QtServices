#include "paranetwork.h"

ParaNetwork::ParaNetwork(QObject *parent) : QObject(parent)
{
    No=0;             //序号
    Guid="";       //全局ID
    Name="";       //名称
    Father="";     //父类
    Systype=0;        //系统类型
    Devtype=0;        //设备类型
    Servertype=0;     //服务类型
    Oppip=0;          //对端IP
    Oppport=0;        //对端端口
    Myip=0;           //自身IP
    muport=0;         //自身端口
    Sysprototal=0;    //系统规约
    sonprototal=0;    //分支规约
    Prototalpara=0;   //规约参数
    Sendno=0;         //发送表数
    Sendid=0;         //发送表
    Ifeffect=0;       //有效
    Ifrun=0;          //运行
    Master=0;         //主通道
    Salve=0;          //备通道
    iftime=0;         //允许校时
    Ifyaokong=0;      //允许遥控
    Yaocetype=0;      //遥测方式
    Yaoxintype=0;     //遥信方式
    Yaomaitype=0;     //遥脉方式
    Yaokongtype=0;    //遥控方式
    Yaotiaogtype=0;   //遥调方式
    Yaoshitype=0;     //遥视方式
    syn=0;            //同异步
}
ParaNetwork::~ParaNetwork()
{}
