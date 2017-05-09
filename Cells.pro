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
    src/SkyBox.cpp \
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
    include/SkyBox.h \
    include/SpotLight.h \
    include/SelectableObject.h

win32:LIBS += opengl32.lib

OTHER_FILES += \
    .gitignore \
    Doxyfile \
    README.md

DISTFILES += \
    resources/shaders/blur.frag \
    resources/shaders/geom.vert \
    resources/shaders/geom.frag \
    resources/shaders/skybox.vert \
    resources/shaders/skybox.frag \
    resources/shaders/sun.vert \
    resources/shaders/sun.frag \
    resources/shaders/lighting.frag \
    resources/shaders/links.vert \
    resources/shaders/links.frag \
    resources/shaders/manip.vert \
    resources/shaders/manip.frag \
    resources/shaders/ssao.vert \
    resources/shaders/ssao.frag \

FORMS += \
    ui/GUI.ui

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

#RESOURCES += \
#    resources.qrc
