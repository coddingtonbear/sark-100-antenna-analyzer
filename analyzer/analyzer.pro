#-------------------------------------------------
#
# Project created by QtCreator 2015-02-22T00:22:07
#
#-------------------------------------------------

QT       += xml core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = analyzer
TEMPLATE = app

CONFIG += serialport

SOURCES += main.cpp\
        mainwindow.cpp \
    scandata.cpp \
    graphcanvas.cpp \
    seriallink.cpp \
    graph.cpp \
    graphcursor.cpp \
    eventreceiver.cpp \
    config.cpp \
    settingsdlg.cpp

HEADERS  += mainwindow.h \
    scandata.h \
    graphcanvas.h \
    seriallink.h \
    graph.h \
    graphcursor.h \
    eventreceiver.h \
    version.h \
    config.h \
    settingsdlg.h

FORMS    += mainwindow.ui \
    settingsdlg.ui

RESOURCES += \
    analyzer.qrc
