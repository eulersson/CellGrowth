TARGET = boilerplate

QT += core gui widgets

INCLUDEPATH += \
    $$PWD/include

HEADERS += \
    include/particleswindow.h \
    include/viewport.h

SOURCES += \
    src/main.cpp \
    src/viewport.cpp \
    src/particleswindow.cpp \
    src/helpers.cpp

OBJECTS_DIR = build/obj
MOC_DIR = build/moc

OTHER_FILES = \
    shaders/vertex.glsl \
    shaders/fragment.glsl
