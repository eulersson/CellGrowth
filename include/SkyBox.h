////////////////////////////////////////////////////////////////////////////////
/// @file SkyBox.h
/// @author Fanny Marstrom
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////
/// Source:
/// Gerdelan, A., 2016. Cube Maps: Skyboxes and Environment Mapping. [online]
/// Antongerdelan.net. Available from: http://antongerdelan.net/opengl/cubemaps.html
/// [Accessed 13 Apr. 2017].
///////////////////////////////////////////////////////////////////////////////

#ifndef SKYBOX_H
#define SKYBOX_H

// Qt
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_1_Core>


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
  void setBackground(QString _name);
  void setBlurIterations(uint _value);
  ~SkyBox();
  void prepare(int _width, int _height, QOpenGLFunctions_4_1_Core *_funcs);
  void draw(int _width, int _height, QOpenGLFunctions_4_1_Core *_funcs);
  QOpenGLTexture *getCubeMapTexture() {return m_cubemap_texture;}

private:
  uint m_blur_iterations;
  InputManager *m_input_manager;

  QOpenGLBuffer m_skybox_vbo;
  QOpenGLVertexArrayObject *m_skybox_vao;
  QOpenGLShaderProgram *m_sky_program;
  QOpenGLTexture *m_cubemap_texture;
  QOpenGLTexture* m_painted_sky;

  QOpenGLFramebufferObject *m_fbo;
  QOpenGLVertexArrayObject *m_quad_vao;
  QOpenGLBuffer m_quad_vbo;
  QOpenGLShaderProgram *m_blur_program;
};

#endif // SKYBOX_H
