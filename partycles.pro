TARGET = Partycles

QT += core gui widgets

INCLUDEPATH += \
    $$PWD/include \
    /home/i7243466/libraries/glm-0.9.8.2

HEADERS += \
    include/particleswindow.h \
    include/viewport.h

SOURCES += \
    src/main.cpp \
    src/viewport.cpp \
    src/particleswindow.cpp

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
