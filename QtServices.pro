#-------------------------------------------------
#
# Project created by QtCreator 2017-01-26T21:42:41
#
#-------------------------------------------------
include(qtservices/qtservice.pri)

QT += core network sql
QT -= gui
MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtServices
TEMPLATE = app
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11
LIBS += -Ldll -lws2_32
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QS_LOG_LINE_NUMBERS
DEFINES += QS_LOG_SEPARATE_THREAD # messages are queued and written from a separate thread
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    mywinservice.cpp \
    QsLog/QsLogDestFunctor.cpp \
    QsLog/QsLogDestFile.cpp \
    QsLog/QsLogDestConsole.cpp \
    QsLog/QsLogDest.cpp \
    QsLog/QsLog.cpp \
    mytcpserver.cpp \
    AppConfig/myapp.cpp \
    CycleBuffer/CCycleBuffer.cpp \
    processclass.cpp \
    mainprocess.cpp \
    Para/paranetwork.cpp
HEADERS  += \
    mywinservice.h \
    QsLog/QsLogLevel.h \
    QsLog/QsLogDisableForThisFile.h \
    QsLog/QsLogDestFunctor.h \
    QsLog/QsLogDestFile.h \
    QsLog/QsLogDestConsole.h \
    QsLog/QsLogDest.h \
    QsLog/QsLog.h \
    mytcpserver.h \
    AppConfig/myapp.h \
    myhelper.h \
    CycleBuffer/CCycleBuffer.h \
    processclass.h \
    mainprocess.h \
    Para/paranetwork.h




