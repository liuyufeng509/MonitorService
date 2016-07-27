QT += core network xml sql
QT -= gui

CONFIG += c++11

TARGET = MonitorService
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES=WITH_OPENSSL
LIBS= -lgsoap++ -lssl -lcrypto

SOURCES += main.cpp \
    config/qreadconfig.cpp \
    SysMonitor/sysmonitor.cpp \
    SysMonitor/sysmonitorthread.cpp \
    StreamMonitor/streammonitor.cpp \
    StreamMonitor/streammonitorthread.cpp \
    commandline.cpp \
    StreamMonitor/localclient.cpp \
    StreamMonitor/localserver.cpp \
    intcommon/soapC.cpp \
    intcommon/soapintcommonProxy.cpp \
    intcommon/soapJqServiceHttpBindingProxy.cpp \
    intcommon/soapPzServiceImplServiceSoapBindingProxy.cpp \
    intcommon/stdsoap2.cpp \
    SysMonitor/systemresourcemonitor.cpp \
    common.cpp

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
    intcommon/JqServiceHttpBinding.nsmap \
    intcommon/PzServiceImplServiceSoapBinding.nsmap \
    intcommon/soapH.h \
    intcommon/soapintcommonProxy.h \
    intcommon/soapJqServiceHttpBindingProxy.h \
    intcommon/soapPzServiceImplServiceSoapBindingProxy.h \
    intcommon/soapStub.h \
    intcommon/stdsoap2.h \
    SysMonitor/systemresourcemonitor.h \
    NetMaster.h

DISTFILES += \
    config.ini

LIBS += -L $$PWD/lib/ -Wl,-rpath,./lib

unix:!macx: LIBS += -L$$PWD/../program/testFrontNet/lib/ -l_acl

INCLUDEPATH += $$PWD/../program/testFrontNet
DEPENDPATH += $$PWD/../program/testFrontNet

unix:!macx: LIBS += -L$$PWD/../program/testFrontNet/lib/ -l_acl_cpp

INCLUDEPATH += $$PWD/../program/testFrontNet
DEPENDPATH += $$PWD/../program/testFrontNet

unix:!macx: LIBS += -L$$PWD/../program/testFrontNet/lib/ -lboost_system-mt-d

INCLUDEPATH += $$PWD/../program/testFrontNet
DEPENDPATH += $$PWD/../program/testFrontNet

unix:!macx: LIBS += -L$$PWD/../program/testFrontNet/lib/ -lboost_thread-mt-d

INCLUDEPATH += $$PWD/../program/testFrontNet
DEPENDPATH += $$PWD/../program/testFrontNet

unix:!macx: LIBS += -L$$PWD/../program/testFrontNet/lib/ -lFrontNetMasterd

INCLUDEPATH += $$PWD/../program/testFrontNet
DEPENDPATH += $$PWD/../program/testFrontNet

unix:!macx: LIBS += -L$$PWD/../program/testFrontNet/lib/ -lprotobuf

INCLUDEPATH += $$PWD/../program/testFrontNet
DEPENDPATH += $$PWD/../program/testFrontNet

unix:!macx: LIBS += -L$$PWD/../program/testFrontNet/lib/ -lprotobuf-lite

INCLUDEPATH += $$PWD/../program/testFrontNet
DEPENDPATH += $$PWD/../program/testFrontNet

unix:!macx: LIBS += -L$$PWD/../program/testFrontNet/lib/ -lprotoc

INCLUDEPATH += $$PWD/../program/testFrontNet
DEPENDPATH += $$PWD/../program/testFrontNet

unix:!macx: LIBS += -L$$PWD/../program/testFrontNet/lib/ -l_protocol

INCLUDEPATH += $$PWD/../program/testFrontNet
DEPENDPATH += $$PWD/../program/testFrontNet

unix:!macx: LIBS += -L$$PWD/../program/testFrontNet/lib/ -lstdc++

INCLUDEPATH += $$PWD/../program/testFrontNet
DEPENDPATH += $$PWD/../program/testFrontNet
