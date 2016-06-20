QT += core
QT -= gui

CONFIG += c++11

TARGET = MonitorService
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    config/qreadconfig.cpp \
    SysMonitor/sysmonitor.cpp \
    SysMonitor/sysmonitorthread.cpp \
    StreamMonitor/streammonitor.cpp \
    StreamMonitor/streammonitorthread.cpp \
    commandline.cpp

HEADERS += \
    config/qreadconfig.h \
    config/datadefine.h \
    SysMonitor/sysmonitor.h \
    SysMonitor/sysmonitorthread.h \
    StreamMonitor/streammonitor.h \
    StreamMonitor/streammonitorthread.h \
    commandline.h

DISTFILES += \
    config.ini \
    config.ini.bak1
