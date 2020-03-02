QT -= gui
QT += sql

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += ../

LIBS += -pthread -lz -lssl -lcrypto


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    apptest.cpp

SOURCES += ../app.cpp \
           ../club.cpp \
           ../jugador.cpp \
           ../usuari.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L../websockets/lib/release/ -lixwebsocket
else:win32:CONFIG(debug, debug|release): LIBS += -L../websockets/lib/debug/ -lixwebsocket
else:unix: LIBS += -L../websockets/lib/ -lixwebsocket

INCLUDEPATH += ../websockets/include
DEPENDPATH += ../websockets/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ../websockets/lib/release/libixwebsocket.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../websockets/lib/debug/libixwebsocket.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ../websockets/lib/release/ixwebsocket.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../websockets/lib/debug/ixwebsocket.lib
else:unix: PRE_TARGETDEPS += ../websockets/lib/libixwebsocket.a


HEADERS += \
    doctest.h \
    apptest.h

HEADERS += ../app.h \
           ../club.h \
           ../jugador.h \
           ../usuari.h
