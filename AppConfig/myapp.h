#ifndef MYAPP_H
#define MYAPP_H

#include <QString>
#include "../myhelper.h"
class myApp
{
public:

    static QString AppPath;             //应用程序路径

    static QString SoftTitle ;          //软件标题
    static QString SoftVersion;         //软件版本
    static QString CompanyName;         //开发商名称
    static QString ContactName;         //联系人
    static QString ContactTel;          //联系电话
    static QString LastLoginer;         //最后一次登录用户名,用来自动登录时需要加载的用户信息
    static QString WebStation;          //网址
    static QString telephone;           //电话
    static QString cellphone;           //手机

    static QString UdpPort;
    static QString TcpPort;

    static void ReadConfig();           //读取配置文件,在main函数最开始加载程序载入
    static void WriteConfig();          //写入配置文件,在更改配置文件、程序关闭时调用
};

#endif // MYAPP_H
