#-------------------------------------------------
#
# Project created by QtCreator 2016-09-05T10:22:56
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = T3Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filetransserver.cpp

HEADERS  += mainwindow.h \
    filetransserver.h

FORMS    += mainwindow.ui
