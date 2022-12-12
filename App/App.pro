QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle
CONFIG += DEBUG

INCLUDEPATH *= $$clean_path($$PWD/../)
include($$PWD/../source/framework/Framework.pri)

SOURCES += \
        main.cpp

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    $$PWD/../source/framework/Framework.pri


INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

LIBS += -L$$PWD/../lib/ -lglfw3
LIBS += -L$$PWD/../bin/glfw3.dll


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lglfw3
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -lglfw3d
#else:unix: LIBS += -L$$PWD/../lib/ -lglfw3


#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/libglfw3.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/libglfw3d.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/glfw3.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/glfw3d.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../lib/libglfw3.a
