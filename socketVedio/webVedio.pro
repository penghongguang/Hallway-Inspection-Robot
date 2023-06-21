QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    socket.cpp \
    videoSocket.cpp

HEADERS += \
    common.h \
    mainwindow.h \
    socket.h \
    videoSocket.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    sources.qrc


INCLUDEPATH +=D:\opencv\build\install\include
              D:\opencv\build\install\include\opencv2

LIBS  +=-L D:\opencv\build\install\x64\mingw\lib\libopencv_*.dll.a  #意思是将编译好的所有opencv库添加到QT中

