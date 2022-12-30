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
    $$PWD/train.h \
    $$PWD/utils.h \
    $$PWD/railway.h


SOURCES += \
    $$PWD/camera.cpp \
    $$PWD/engine.cpp \
    $$PWD/filesystem.cpp \
    $$PWD/mesh.cpp \
    $$PWD/object.cpp \
    $$PWD/shader.cpp \
    $$PWD/spline.cpp \
    $$PWD/train.cpp \
    $$PWD/utils.cpp \
    $$PWD/railway.cpp \
    $$PWD/glad.c
}
