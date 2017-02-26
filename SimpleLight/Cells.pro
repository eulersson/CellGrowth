TARGET = cells
TEMPLATE = app

QT += core gui

CONFIG += c++11
CONFIG -= app_bundle

SOURCES += \
    src/main.cpp \
    src/AbstractScene.cpp \
    src/GrowthParticle.cpp \
    src/LinkedParticle.cpp \
    src/Particle.cpp \
    src/ParticleSystem.cpp \
    src/Scene.cpp \
    src/Window.cpp \
    src/input.cpp \
    src/manipulator.cpp

OBJECTS_DIR = build/obj
MOC_DIR = build/moc

INCLUDEPATH += include

HEADERS += \
    include/AbstractScene.h \
    include/GrowthParticle.h \
    include/LinkedParticle.h \
    include/Particle.h \
    include/ParticleSystem.h \
    include/Scene.h \
    include/Window.h \
    include/arcball.h \
    include/input.h \
    include/pointlight.h \
    include/selectobject.h \
    include/manipulator.h

OTHER_FILES += \
    .gitignore \
    Doxyfile \
    README.md

DISTFILES += \
    shaders/particles.vert \
    shaders/particles.frag \
    shaders/quad.vert \
    shaders/quad.frag
