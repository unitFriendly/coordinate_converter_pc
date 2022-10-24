QT -= gui
QT += websockets

QT += gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        clientsocket.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    clientsocket.h


win32: LIBS += -L$$PWD/../leptonica_x86-windows/lib/ -lleptonica-1.81.1

INCLUDEPATH += $$PWD/../leptonica_x86-windows/include
DEPENDPATH += $$PWD/../leptonica_x86-windows/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../leptonica_x86-windows/lib/leptonica-1.81.1.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../leptonica_x86-windows/lib/libleptonica-1.81.1.a

win32: LIBS += -L$$PWD/../tesseract_x86-windows/lib/ -ltesseract41

INCLUDEPATH += $$PWD/../tesseract_x86-windows/include
DEPENDPATH += $$PWD/../tesseract_x86-windows/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../tesseract_x86-windows/lib/tesseract41.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../tesseract_x86-windows/lib/libtesseract41.a
