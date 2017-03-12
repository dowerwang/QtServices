#ifndef MYWINSERVICE_H
#define MYWINSERVICE_H

#include "./qtservices/qtservice.h"
#include "mytcpserver.h"
#include "../QsLog/QsLog.h"
#include "../QsLog/QsLogDest.h"
#include "../QsLog/QsLogLevel.h"

#include "./mainprocess.h"
class MyWinService: public QtService<QCoreApplication>
{
public:
    MyWinService(int argc, char *argv[]);
    ~MyWinService(){}

public:
    void start();
    void pause();
    void resume();
    void stop();
    QString ExecPath();
    MainProcess *mainProcess;
};

#endif // MYWINSERVICE_H
