QT += core
QT -= gui

TARGET = socket
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../server.cpp

HEADERS += \
    ../../server.h

INCLUDEPATH+=../..
CONFIG+=c++11
