TARGET = boilerplate

QT += core gui widgets

CONFIG += c++11

INCLUDEPATH += \
  $$PWD/include

OBJECTS_DIR = build/obj
MOC_DIR = build/moc

HEADERS += \
  include/ParticlesWindow.h \
  include/Viewport.h \
  include/LinkedParticle.h \
  include/ParticleSystem.h

SOURCES += \
  src/main.cpp \
  src/Viewport.cpp \
  src/ParticlesWindow.cpp \
  src/helpers.cpp \
  src/LinkedParticle.cpp \
  src/ParticleSystem.cpp

OTHER_FILES = \
  shaders/vertex.glsl \
  shaders/fragment.glsl \
  shaders/line.glsl
