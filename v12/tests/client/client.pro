#-------------------------------------------------
#
# Project created by QtCreator 2018-05-08T15:40:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp serverinfosearcher.cpp \
client.cpp \
    playerwidget.cpp tool.cpp  videosource.cpp \
 pvd.cpp  cppjson/json_reader.cpp \
     cppjson/json_value.cpp \
     cppjson/json_writer.cpp \
    datamanager.cpp


HEADERS  += mainwindow.h serverinfosearcher.h  client.h\
    playerwidget.h  tool.h    videosource.h   pvd.h \
    datamanager.h

FORMS    += mainwindow.ui



INCLUDEPATH+=$$PREFIX
INCLUDEPATH+= cppjson/include
CONFIG+=c++11
DEFINES+=IS_UNIX
    LIBS+= -lopencv_core -lopencv_highgui \
       -lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_highgui  -lopencv_video  -lX11

QT+=network
