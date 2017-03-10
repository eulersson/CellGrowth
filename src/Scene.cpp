////////////////////////////////////////////////////////////////////////////////
/// @file Scene.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Project
#include "Scene.h"
#include <QKeyEvent>
















// Recursion subdivision algorithm from http://www.opengl.org.ru/docs/pg/0208.html
#define X .525731112119133606
#define Z .850650808352039932

void pushTriangle(float *v1, float *v2, float *v3, std::vector<GLfloat>& _data) {
  _data.push_back(v1[0]); _data.push_back(v1[1]); _data.push_back(v1[2]); // v1
  _data.push_back(v2[0]); _data.push_back(v2[1]); _data.push_back(v2[2]); // v2
  _data.push_back(v3[0]); _data.push_back(v3[1]); _data.push_back(v3[2]); // v3
}

void subdivide(float *v1, float *v2, float *v3, long depth, std::vector<GLfloat>& _data) {
  GLfloat v12[3], v23[3], v31[3]; // midpoints

  if (depth == 0) {
     pushTriangle(v1, v2, v3, _data);
     return;
  }

  for (size_t i = 0; i < 3; i++) {
    v12[i] = v1[i] + v2[i];
    v23[i] = v2[i] + v3[i];
    v31[i] = v3[i] + v1[i];
  }

  float v12_len = std::sqrt(v12[0]*v12[0] + v12[1] * v12[1] + v12[2] * v12[2]);
  float v23_len = std::sqrt(v23[0]*v23[0] + v23[1] * v23[1] + v23[2] * v23[2]);
  float v31_len = std::sqrt(v31[0]*v31[0] + v31[1] * v31[1] + v31[2] * v31[2]);

  // Normalize (push so match sphere)
  for (size_t i = 0; i < 3; i++)
  {
    v12[i] /= v12_len;
    v23[i] /= v23_len;
    v31[i] /= v31_len;
  }

  subdivide(v1, v12, v31,  depth-1, _data);
  subdivide(v2, v23, v12,  depth-1, _data);
  subdivide(v3, v31, v23,  depth-1, _data);
  subdivide(v12, v23, v31, depth-1, _data);
}

void Scene::generateSphereData() {

  m_sphereData.clear();

  GLfloat vdata[12][3] = {
     {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
     {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
     {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
  };

  GLuint tindices[20][3] = {
     {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
     {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
     {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
     {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

  for (size_t i = 0; i < 20; i++) {
    subdivide(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], 2, m_sphereData);
  }

}




































Scene::Scene(Window *_window) : AbstractScene(_window), inputManager(_window)
{
}

Scene::~Scene()
{
}

void Scene::initialize()
{
  // Initialize so we can use OpenGL functions
  AbstractScene::initialize();

  generateSphereData();

  m_sphere_vbo.create();
  m_sphere_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);









  // Projection matrix for the particles
  m_projectionMatrix.setToIdentity();
  m_projectionMatrix.perspective(89.0f, (float)window()->width()/(float)window()->height(), 0.01f, 100.0f);

  // Prepare for deferred rendering
  prepareQuad();
  prepareParticles();
  setupFBO();

  setupLights();

  qDebug("%d", m_ps.getSize());

  timer.start();
  glViewport(0, 0, window()->width(), window()->height());


}

void Scene::paint()
{

  deltaTime=timer.elapsed()*0.001;
  timer.restart();

  glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  updateModelMatrix();
  inputManager.setupCamera();
  inputManager.doMovement();

  m_FBO->bind();
  drawParticles();
  for(auto &s : objectList) { s->draw(); }


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
  m_part_program = new QOpenGLShaderProgram(this);
  m_part_program->addShaderFromSourceFile(QOpenGLShader::Vertex  , "shaders/particles.vert");
  m_part_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/particles.frag");
  m_part_program->link();

  m_part_vao = new QOpenGLVertexArrayObject(this);

  m_part_vao->create();
  m_part_vbo.create();
  m_part_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);

  sendParticleDataToOpenGL();
}

void Scene::drawQuad()
{

  m_quad_program->bind();
  m_quad_vao->bind();

  glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_activeRenderPassIndex);

  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  m_quad_vao->release();
  m_quad_program->release();
}

void Scene::drawParticles()
{
  m_part_program->bind();
  m_part_program->setUniformValue("ProjectionMatrix", inputManager.getProjectionMatrix());
  m_part_program->setUniformValue("ModelMatrix", m_modelMatrix);
  m_part_program->setUniformValue("ViewMatrix", inputManager.getViewMatrix());
  m_part_vao->bind();
  unsigned int pointsToDraw = m_sphereData.size() / 3;

  glEnable(GL_DEPTH_TEST);
  glDrawArraysInstanced(GL_TRIANGLES, 0, pointsToDraw, m_ps.getSize());

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

void Scene::setupLights()
{
  m_manipulatorProgram = new QOpenGLShaderProgram(this);
  m_manipulatorProgram->addShaderFromSourceFile(QOpenGLShader::Vertex  , "shaders/manip.vert");
  m_manipulatorProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/manip.frag");
  m_manipulatorProgram->link();

  m_sunProgram = new QOpenGLShaderProgram(this);
  m_sunProgram->addShaderFromSourceFile(QOpenGLShader::Vertex  , "shaders/sun.vert");
  m_sunProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/sun.frag");
  m_sunProgram->link();

  QVector3D masterUniqueColour=QVector3D(0.0f, 100.0f, 0.0f);
  for(int x=-4;x<4;x+=4) {
      for(int y=-4;y<4;y+=4) {
          PointLight *pointlight;
          pointlight = new PointLight(QVector3D(x,y,0), m_manipulatorProgram, m_sunProgram);
          pointlight->createGeometry(context(), masterUniqueColour);
          objectList.push_back(std::move(std::unique_ptr<PointLight>(pointlight)));
      }
  }
  inputManager.addShaderProgram(m_manipulatorProgram);
  inputManager.addShaderProgram(m_sunProgram);
  inputManager.setObjectList(objectList);
}

void Scene::windowResized(int _w, int _h)
{
  qDebug("Window rezised to %d and %d", _w, _h);
}

QMatrix4x4 Scene::updateModelMatrix()
{
  // Insert particle system position here
  QVector3D pointPos=QVector3D(0,0,0);
  m_modelMatrix.setToIdentity();
  m_modelMatrix.translate(pointPos);
}

void Scene::updateParticleSystem()
{
  m_ps.splitRandomParticle();
  m_ps.advance();
  sendParticleDataToOpenGL();
}

void Scene::sendParticleDataToOpenGL()
{
  // Tell particle system to populate us a flattened float array for OpenGL
  m_ps.packageDataForDrawing(m_packagedParticleData);

  // Print x, y, z, radius of each particle for debugging. This is the actual
  // data that gets sent to OpenGL.
  for_each(m_packagedParticleData.begin(), m_packagedParticleData.end(), [](float f)
  {
    qDebug("%f", f);
  });


  m_part_vao->bind();

  // Sphere Data =================================================================
  m_sphere_vbo.bind();
  m_sphere_vbo.allocate(&m_sphereData[0], m_sphereData.size() * sizeof(GLfloat));
  m_part_program->enableAttributeArray("position");
  m_part_program->setAttributeBuffer("position", GL_FLOAT, 0, 3);

  // Buffering Instances= Data =================================================
  m_part_vbo.bind();
  m_part_vbo.allocate(&m_packagedParticleData[0], m_ps.getSize() * 4 * sizeof(GLfloat));

  m_part_program->enableAttributeArray("instances");
  m_part_program->setAttributeBuffer("instances", GL_FLOAT, 0, 4);

  m_part_vbo.release();
  glVertexAttribDivisor(m_part_program->attributeLocation("instances"), 1);

  m_part_vao->release();
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
  case Qt::Key_Space:
    updateParticleSystem();
  default:
    break;
  }

  inputManager.keyPressEvent(ev);
}

void Scene::keyReleaseEvent(QKeyEvent *key)
{
    inputManager.keyReleaseEvent(key);
}

void Scene::mouseMoveEvent(QMouseEvent* event)
{
    inputManager.mouseMoveEvent(event);
}

void Scene::mousePressEvent(QMouseEvent *event)
{
    inputManager.mousePressEvent(event);
}

void Scene::mouseReleaseEvent(QMouseEvent *event)
{
    inputManager.mouseReleaseEvent(event);
}

void Scene::wheelEvent(QWheelEvent *event)
{
    inputManager.wheelEvent(event);
}
