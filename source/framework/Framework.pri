!FRAMEWORK_PRI{
CONFIG += FRAMEWORK_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

HEADERS += \
    $$PWD/camera.h \
    $$PWD/engine.h \
    $$PWD/filesystem.h \
    $$PWD/mesh.h \
    $$PWD/object.h \
    $$PWD/shader.h \
    $$PWD/spline.h \
    $$PWD/utils.h \
    $$PWD/objectutils.h


SOURCES += \
    $$PWD/camera.cpp \
    $$PWD/engine.cpp \
    $$PWD/filesystem.cpp \
    $$PWD/mesh.cpp \
    $$PWD/object.cpp \
    $$PWD/shader.cpp \
    $$PWD/spline.cpp \
    $$PWD/utils.cpp \
    $$PWD/objectutils.cpp \
    $$PWD/glad.c

}

HEADERS += \
    $$PWD/railway.h

SOURCES += \
    $$PWD/railway.cpp
