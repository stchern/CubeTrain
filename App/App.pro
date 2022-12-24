QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle
CONFIG += DEBUG

INCLUDEPATH *= $$clean_path($$PWD/../)
include($$PWD/../source/framework/Framework.pri)

SOURCES += \
         $$PWD/../source/main.cpp


INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

LIBS += -L$$PWD/../lib/ -lglfw3
LIBS += -L$$PWD/../bin/glfw3.dll

