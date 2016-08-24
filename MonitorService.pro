QT += core network xml sql
QT -= gui

CONFIG += c++11

TARGET = MonitorService
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES=WITH_OPENSSL
LIBS= -lssl -lcrypto

SOURCES += main.cpp \
    config/qreadconfig.cpp \
    SysMonitor/sysmonitor.cpp \
    SysMonitor/sysmonitorthread.cpp \
    StreamMonitor/streammonitor.cpp \
    StreamMonitor/streammonitorthread.cpp \
    commandline.cpp \
    StreamMonitor/localclient.cpp \
    StreamMonitor/localserver.cpp \
    intcommon/stdsoap2.cpp \
    SysMonitor/systemresourcemonitor.cpp \
    common.cpp \
    intcommon/BaseC.cpp \
    intcommon/BaseClient.cpp

HEADERS += \
    config/qreadconfig.h \
    config/datadefine.h \
    SysMonitor/sysmonitor.h \
    SysMonitor/sysmonitorthread.h \
    StreamMonitor/streammonitor.h \
    StreamMonitor/streammonitorthread.h \
    commandline.h \
    StreamMonitor/localclient.h \
    StreamMonitor/localserver.h \
    common.h \
    intcommon/BaseInterface.h \
    intcommon/intcommon.nsmap \
    intcommon/stdsoap2.h \
    SysMonitor/systemresourcemonitor.h \
    NetMaster.h \
    intcommon/BaseH.h \
    intcommon/BaseintcommonObject.h \
    intcommon/BaseStub.h

DISTFILES += \
    config.ini

LIBS += -L $$PWD/lib/ -Wl,-rpath,./lib


unix:!macx: LIBS += -L$$PWD/lib/ -l_acl

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/lib/ -l_acl_cpp

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/lib/ -lboost_system-mt-d

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/lib/ -lboost_thread-mt-d

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

#unix:!macx: LIBS += -L$$PWD/lib/ -lFrontNetMasterd

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/lib/ -lprotobuf

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/lib/ -lprotobuf-lite

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/lib/ -lprotoc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/lib/ -l_protocol

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/lib/ -lstdc++

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/lib/ -lFrontNetMasterd

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
