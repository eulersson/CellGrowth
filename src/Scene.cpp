////////////////////////////////////////////////////////////////////////////////
/// @file Scene.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////


// Project
#include "Scene.h"

Scene::Scene(Window *_window) : AbstractScene(_window)
{
}

Scene::~Scene()
{
}

void Scene::initialize()
{
  AbstractScene::initialize();
}

void Scene::paint()
{
  qDebug("Painting");
  glViewport(0, 0, window()->width(), window()->height());
  glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}
