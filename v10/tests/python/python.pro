QT -= core
QT -= gui

TARGET = python
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \ 
    test.cpp


CONFIG+=c++11
install_files.files+=run.py
install_files.path=$$OUT_PWD/
INSTALLS+=install_files
INCLUDEPATH+=/usr/include/python2.7
LIBS+=-lpython2.7

HEADERS += \
    test.h
