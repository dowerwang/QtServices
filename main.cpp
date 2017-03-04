#include "mywinservice.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include "./AppConfig/myapp.h"

QsLogging::Logger& logger = QsLogging::Logger::instance();
int main(int argc, char *argv[])
{


    MyWinService service(argc, argv);
    myHelper::SetUTF8Code();
    //读取配置文件
    myApp::AppPath="./";
    myApp::ReadConfig();
    using namespace QsLogging;
    //init the logging mechanism
    logger.setLoggingLevel(QsLogging::InfoLevel);
    const QString sLogPath("c:\\App.log");
    QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(sLogPath, EnableLogRotation, MaxSizeBytes(1024*1000), MaxOldLogCount(2)));
    QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);

    return service.exec();
}
