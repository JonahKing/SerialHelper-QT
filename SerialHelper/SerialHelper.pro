#-------------------------------------------------
#
# Project created by QtCreator 2016-07-28T14:26:19
#
#-------------------------------------------------

QT       += core gui serialport sql printsupport axcontainer

RC_ICONS = LanZhou.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = SerialHelper
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    serialconfig.cpp \
    autoreplywindows.cpp \
    qcustomplot.cpp \
    temperaturewindows.cpp \
    ExcelManger.cpp

HEADERS  += mainwindow.h \
    serialconfig.h \
    autoreplywindows.h \
    qcustomplot.h \
    temperaturewindows.h \
    ExcelManger.h

FORMS    += \
    mainwindow.ui \
    autoreplywindows.ui \
    temperaturewindows.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    res.qrc

