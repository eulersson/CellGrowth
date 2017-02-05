////////////////////////////////////////////////////////////////////////////////
/// @file Quad.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#include "Quad.h"

Quad::Quad(QObject *parent) : QObject(parent)
{
}

Quad::~Quad()
{
}

void Quad::prepare()
{
  static const float quad[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
  };

  m_program = new QOpenGLShaderProgram(this);
  m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/quad.vert");
  m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/quad.frag");
  m_program->link();

  m_vao = new QOpenGLVertexArrayObject(this);
  QOpenGLBuffer vbo;

  m_vao->create();
  vbo.create();
  vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

  m_vao->bind();
  vbo.bind();

  vbo.allocate(quad, 5 * 6 * sizeof(GLfloat));

  m_program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
  m_program->enableAttributeArray("position");

  m_program->setAttributeBuffer("uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
  m_program->enableAttributeArray("uv");

  vbo.release();
  m_vao->release();
}

void Quad::draw()
{
  m_program->bind();
  m_vao->bind();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  m_vao->release();
  m_program->release();
}
