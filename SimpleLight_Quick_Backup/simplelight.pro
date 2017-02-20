TARGET = simplelight

QT += core gui widgets opengl


INCLUDEPATH += \
    $$PWD/headers

HEADERS +=  \
        headers/selectobject.h \
        headers/openglwindow.h \
        headers/manipulator.h \
        headers/camera.h \
        headers/pointlight.h \
        headers/input.h \
        headers/viewport.h


SOURCES += \
        src/main.cpp \
        src/openglwindow.cpp \
        src/manipulator.cpp \
        src/viewport.cpp

OTHER_FILES += \
        shaders/manipvert.glsl \
        shaders/manipfrag.glsl \
        shaders/sunvert.glsl \
        shaders/sunfrag.glsl


OBJECTS_DIR = build/obj
