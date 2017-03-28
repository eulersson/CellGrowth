////////////////////////////////////////////////////////////////////////////////
/// @file AbstractScene.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#include "AbstractScene.h"

AbstractScene::AbstractScene(QOpenGLWidget *_window)
{
  m_window = _window;
}

AbstractScene::~AbstractScene()
{
}

QOpenGLWidget *AbstractScene::window() const
{
  return m_window;
}

QOpenGLContext *AbstractScene::context()
{
  return window() ? window()->context() : 0;
}

const QOpenGLContext *AbstractScene::context() const
{
  return window() ? window()->context() : 0;
}

void AbstractScene::initialize()
{
  initializeOpenGLFunctions();
}


