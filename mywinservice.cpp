#include "mywinservice.h"


MyWinService::MyWinService(int argc, char* argv[])
: QtService<QCoreApplication>(argc, argv, "My Windows Service")
{
    //QtServiceBase::instance()->logMessage("服务安装");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}
QString MyWinService::ExecPath()
{
    QCoreApplication *app = application();
    return app->applicationDirPath();
}
void MyWinService::start()
{
    QLOG_INFO()<<"services start;";

    QCoreApplication *app = application();

    mainProcess = new MainProcess(app);

}

void MyWinService::stop()
{
    QLOG_INFO()<<"services stop;";
}

void MyWinService::pause()
{
    QLOG_INFO()<<"services pause;";
    //myTcpServer->pause();
}

void MyWinService::resume()
{
    QLOG_INFO()<<"services resume;";
    //myTcpServer->resume();
}
