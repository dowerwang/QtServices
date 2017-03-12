#ifndef PARANETWORK_H
#define PARANETWORK_H

#include <QObject>

class ParaNetwork : public QObject
{
    Q_OBJECT
public:
    explicit ParaNetwork(QObject *parent = 0);
    ~ParaNetwork();
    int No;             //序号
    QString Guid;       //全局ID
    QString Name;       //名称
    QString Father;     //父类
    int Systype;        //系统类型
    int Devtype;        //设备类型
    int Servertype;     //服务类型
    int Oppip;          //对端IP
    int Oppport;        //对端端口
    int Myip;           //自身IP
    int muport;         //自身端口
    int Sysprototal;    //系统规约
    int sonprototal;    //分支规约
    int Prototalpara;   //规约参数
    int Sendno;         //发送表数
    int Sendid;         //发送表
    int Ifeffect;       //有效
    int Ifrun;          //运行
    int Master;         //主通道
    int Salve;          //备通道
    int iftime;         //允许校时
    int Ifyaokong;      //允许遥控
    int Yaocetype;      //遥测方式
    int Yaoxintype;     //遥信方式
    int Yaomaitype;     //遥脉方式
    int Yaokongtype;    //遥控方式
    int Yaotiaogtype;   //遥调方式
    int Yaoshitype;     //遥视方式
    int syn;            //同异步
signals:

public slots:
};

#endif // PARANETWORK_H
