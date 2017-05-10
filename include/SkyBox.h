////////////////////////////////////////////////////////////////////////////////
/// @file SkyBox.h
/// @author Fanny Marstrom
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#ifndef SKYBOX_H
#define SKYBOX_H

// Qt
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

// Project
#include "InputManager.h"

////////////////////////////////////////////////////////////////////////////////
/// @class SkyBox
/// @brief Wraps all the code related to the sky environment.
////////////////////////////////////////////////////////////////////////////////
class SkyBox
{
public:
  SkyBox(InputManager *_input_manager);
  ~SkyBox();
  void prepare();
  void draw();
  QOpenGLTexture *GetSkyBoxTexture() {return m_texture;}

private:
  InputManager *m_input_manager;
  QOpenGLBuffer m_vbo;
  QOpenGLVertexArrayObject *m_vao;
  QOpenGLShaderProgram *m_program;
  QOpenGLTexture *m_texture;
};

#endif // SKYBOX_H
