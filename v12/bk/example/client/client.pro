#-------------------------------------------------
#
# Project created by QtCreator 2018-06-12T14:39:54
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
PREFIX=../../
INCLUDEPATH+=$$PREFIX
INCLUDEPATH +=$$PREFIX/cppjson/include
HEADERS+=$$PREFIX/tool.h
SOURCES+=$$PREFIX/tool.cpp
CONFIG+=c++11
DEFINES+=IS_UNIX
