TARGET = cells
TEMPLATE = app

QT += core gui\
        opengl
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
    src/GUI.cpp \
    src/GLWindow.cpp

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
    include/GUI.h \
    include/GLWindow.h

OTHER_FILES += \
    .gitignore \
    Doxyfile \
    README.md

DISTFILES += \
    shaders/particles.vert \
    shaders/particles.frag \
    shaders/quad.vert \
    shaders/quad.frag

FORMS += \
    ui/GUI.ui
