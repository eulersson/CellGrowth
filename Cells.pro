TARGET = cells
TEMPLATE = app

QT += core gui opengl

CONFIG += c++11
CONFIG -= app_bundle

SOURCES += \
    src/main.cpp \
    src/ArcBallCamera.cpp \
    src/GLWindow.cpp \
    src/GrowthParticle.cpp \
    src/Helpers.cpp \
    src/InputManager.cpp \
    src/LinkedParticle.cpp \
    src/Manipulator.cpp \
    src/Particle.cpp \
    src/ParticleSystem.cpp \
    src/GUI.cpp \
    src/PointLight.cpp \
    src/SelectObject.cpp \
    src/SpotLight.cpp\

OBJECTS_DIR = build/obj
MOC_DIR = build/moc

INCLUDEPATH += include

HEADERS += \
    include/ArcBallCamera.h \
    include/GLWindow.h \
    include/GrowthParticle.h \
    include/InputManager.h \
    include/LinkedParticle.h \
    include/Manipulator.h \
    include/Particle.h \
    include/ParticleSystem.h \
    include/GUI.h \
    include/PointLight.h \
    include/SelectObject.h \
    include/SpotLight.h \

win32:LIBS += opengl32.lib

OTHER_FILES += \
    .gitignore \
    Doxyfile \
    README.md

DISTFILES += \
    shaders/links.vert \
    shaders/links.frag \
    shaders/manip.vert \
    shaders/manip.frag \
    shaders/skybox.vert \
    shaders/skybox.frag \
    shaders/sun.vert \
    shaders/sun.frag \
    shaders/particles.vert \
    shaders/particles.frag \
    shaders/quad.vert \
    shaders/quad.frag

FORMS += \
    ui/GUI.ui

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

RESOURCES += \
    resources.qrc
