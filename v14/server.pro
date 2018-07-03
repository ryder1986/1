TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH +=cppjson/include
#INCLUDEPATH+=/root/source/opencv/build/__install/include
SOURCES += main.cpp tool.cpp server.cpp cppjson/json_reader.cpp cppjson/json_writer.cpp  cppjson/json_value.cpp \
    configmanager.cpp videosource.cpp c4common.cpp c4processor.cpp \
    pvdobject.cpp hogprocessor.cpp \
    camera.cpp \
    conversion.cpp \
    movidiusprocessor.cpp \
    socket.cpp \
    app.cpp
HEADERS+=tool.h server.h \
    app.h \
    configmanager.h videosource.h c4common.h \
    c4processor.h  hogprocessor.h \
    camera.h \
    conversion.h \
    movidiusprocessor.h \
    datapacket.h \
    socket.h \
    jsondatadealer.h
DEFINES+=IS_UNIX
LIBS+=-lpthread

install_files.files+=movidius.py
install_files.path=$$OUT_PWD/
INSTALLS+=install_files

install_files1.files+=res/
install_files1.path=$$OUT_PWD/
INSTALLS+=install_files1


QMAKE_CXXFLAGS+=-w
DISTFILES += \
    res/config.json \
    movidius.py

#INCLUDEPATH+=/root/opencv/include
#LIBS+=-L/root/opencv/lib -lopencv_core -lopencv_highgui \
#-lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_highgui \
# -lopencv_video  -lopencv_videostab -lopencv_videoio
LIBS+= -lopencv_core -lopencv_highgui \
-lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_highgui\
 -lopencv_video -lpthread


LIBS+=-lavcodec -lavformat -lavutil -lswresample
INCLUDEPATH+=track
SOURCES +=track/Ctracker.cpp track/HungarianAlg.cpp  track/Kalman.cpp
HEADERS += track/defines.h track/HungarianAlg.h track/Kalman.h track/Ctracker.h
INCLUDEPATH+=/usr/include/python2.7
LIBS+=-lpython2.7
