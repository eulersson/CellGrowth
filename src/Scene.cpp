////////////////////////////////////////////////////////////////////////////////
/// @file Scene.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Project
#include "Scene.h"
#include <QKeyEvent>

Scene::Scene(Window *_window) : AbstractScene(_window)
{
}

Scene::~Scene()
{
}

void Scene::initialize()
{
  // Initialize so we can use OpenGL functions
  AbstractScene::initialize();

  // Projection matrix for the particles
    glViewport(0, 0, window()->width(), window()->height());
  m_projectionMatrix.setToIdentity();
  m_projectionMatrix.perspective(70.0f, (float)window()->width()/(float)window()->height(), 0.01f, 100.0f);

  // Prepare for deferred rendering
  prepareQuad();
  prepareParticles();
  setupFBO();
}

void Scene::paint()
{

  glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_FBO->bind();
    drawParticles();

  m_FBO->release();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FBO->takeTexture(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_FBO->takeTexture(1));
    drawQuad();
}

void Scene::prepareQuad()
{
  static const float quad[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
  };

  m_quad_program = new QOpenGLShaderProgram(this);
  m_quad_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/quad.vert");
  m_quad_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/quad.frag");
  m_quad_program->link();

  m_quad_program->bind();
  m_quad_program->setUniformValue("gpass0Tex", 0);
  m_quad_program->setUniformValue("gpass1Tex", 1);
  m_normalShadingIndex   = glGetSubroutineIndex(m_quad_program->programId(), GL_FRAGMENT_SHADER, "normalShading");
  m_positionShadingIndex = glGetSubroutineIndex(m_quad_program->programId(), GL_FRAGMENT_SHADER, "positionShading");
  m_activeRenderPassIndex = m_normalShadingIndex;
  m_quad_program->release();

  m_quad_vao = new QOpenGLVertexArrayObject(this);

  m_quad_vao->create();
  m_quad_vbo.create();
  m_quad_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

  m_quad_vao->bind();
  m_quad_vbo.bind();

  m_quad_vbo.allocate(quad, 5 * 6 * sizeof(GLfloat));

  m_quad_program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
  m_quad_program->enableAttributeArray("position");

  m_quad_program->setAttributeBuffer("uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
  m_quad_program->enableAttributeArray("uv");

  m_quad_vbo.release();
  m_quad_vao->release();
}

void Scene::prepareParticles()
{
  static const float particles[] = {
  // Position              Radius
     4.0f,  2.0f, -9.0f,   2.0f,
     2.0f,  5.0f, -19.0f,  2.0f,
    10.0f,  0.0f, -29.0f,  2.0f,
     0.0f, -1.5f, -3.0f,   2.0f,
    -3.0f,  0.0f, -9.0f,   2.0f
  };

  m_part_program = new QOpenGLShaderProgram(this);
  m_part_program->addShaderFromSourceFile(QOpenGLShader::Vertex  , "shaders/particles.vert");
  m_part_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/particles.frag");
  m_part_program->link();

  m_part_vao = new QOpenGLVertexArrayObject(this);

  m_part_vao->create();
  m_part_vbo.create();
  m_part_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

  m_part_vao->bind();
  m_part_vbo.bind();

  m_part_vbo.allocate(particles, (5 * 3 + 5 * 1) * sizeof(GLfloat));

  m_part_program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 4 * sizeof(GLfloat));
  m_part_program->enableAttributeArray("position");

  m_part_program->setAttributeBuffer("radius", GL_FLOAT, 3 * sizeof(GLfloat), 1, 4 * sizeof(GLfloat));
  m_part_program->enableAttributeArray("radius");

  m_part_vbo.release();
  m_part_vao->release();
}

void Scene::drawQuad()
{
  m_quad_program->bind();
  m_quad_vao->bind();

  glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_activeRenderPassIndex);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  m_quad_vao->release();
  m_quad_program->release();
}

void Scene::drawParticles()
{
  m_part_program->bind();
  m_part_program->setUniformValue("ProjectionMatrix", m_projectionMatrix);
  m_part_vao->bind();
  glEnable(GL_POINT_SPRITE);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glDrawArrays(GL_POINTS, 0, 5);
  m_part_vao->release();
  m_part_program->release();
}

void Scene::setupFBO()
{
  // The passes color attachments go as follows
  // GL_COLOR_ATTACHMENT0: gNormalPass
  // GL_COLOR_ATTACHMENT1: gPositionPass

  m_FBO = new QOpenGLFramebufferObject(720, 720); // GL_COLOR_ATTACHMENT0
  m_FBO->bind();
  m_FBO->addColorAttachment(720, 720);            // GL_COLOR_ATTACHMENT1

  const GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  glDrawBuffers(2, attachments);

  m_FBO->release();
}

void Scene::keyPressed(QKeyEvent *ev)
{
  switch(ev->key()) {
  case Qt::Key_N:
    m_activeRenderPassIndex = m_normalShadingIndex;
    break;
  case Qt::Key_P:
    m_activeRenderPassIndex = m_positionShadingIndex;
    break;
  default:
    break;
  }
}

void Scene::windowResized(int _w, int _h)
{
  qDebug("Window rezised to %d and %d", _w, _h);
}
