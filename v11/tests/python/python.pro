QT -= core
QT -= gui

TARGET = python
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \ 
    test.cpp \
    conversion.cpp


CONFIG+=c++11
install_files.files+=run.py
install_files.path=$$OUT_PWD/
INSTALLS+=install_files
INCLUDEPATH+=/usr/include/python2.7
LIBS+=-lpython2.7

HEADERS += \
    test.h \
    conversion.h
LIBS+= -lopencv_core -lopencv_highgui \
-lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_highgui\
 -lopencv_video -lpthread
