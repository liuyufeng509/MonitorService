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
    intcommon/stdsoap2.cpp

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
    intcommon/stdsoap2.h

DISTFILES += \
    config.ini \
    config.ini.bak1
