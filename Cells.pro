TARGET = cells
TEMPLATE = app

QT += core gui

CONFIG += c++11
CONFIG -= app_bundle

SOURCES += \
    src/main.cpp \
    src/AbstractScene.cpp \
    src/ArcBallCamera.cpp \
    src/GrowthParticle.cpp \
    src/InputManager.cpp \
    src/LinkedParticle.cpp \
    src/Manipulator.cpp \
    src/Particle.cpp \
    src/ParticleSystem.cpp \
    src/PointLight.cpp \
    src/Scene.cpp \
    src/SelectObject.cpp \
    src/SpotLight.cpp \
    src/Window.cpp

OBJECTS_DIR = build/obj
MOC_DIR = build/moc

INCLUDEPATH += include

HEADERS += \
    include/AbstractScene.h \
    include/ArcBallCamera.h \
    include/GrowthParticle.h \
    include/InputManager.h \
    include/LinkedParticle.h \
    include/Manipulator.h \
    include/Particle.h \
    include/ParticleSystem.h \
    include/PointLight.h \
    include/Scene.h \
    include/SelectObject.h \
    include/SpotLight.h \
    include/Window.h

OTHER_FILES += \
    .gitignore \
    Doxyfile \
    README.md

DISTFILES += \
    shaders/links.vert \
    shaders/links.frag \
    shaders/manip.vert \
    shaders/manip.frag \
    shaders/sun.vert \
    shaders/sun.frag \
    shaders/particles.vert \
    shaders/particles.frag \
    shaders/quad.vert \
    shaders/quad.frag
