////////////////////////////////////////////////////////////////////////////////
/// @file Scene.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Project
#include "Scene.h"

// Recursion subdivision algorithm from:
// http://www.opengl.org.ru/docs/pg/0208.html

#define X .525731112119133606
#define Z .850650808352039932

void subdivide(float*, float*, float*, long, std::vector<GLfloat>&);

Scene::Scene(Window *_window)
  : AbstractScene(_window)
  , m_input_manager(_window)
  , m_draw_links(true)
{
}

Scene::~Scene()
{
}

void Scene::initialize()
{
  // Allows use of OpenGL commands
  AbstractScene::initialize();

  generateSphereData(4); // Four subdivisions of an icosahedra

  m_sphere_vbo.create();
  m_sphere_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

  // Projection matrix for the particles
  m_projection_matrix.setToIdentity();
  m_projection_matrix.perspective(
        45.0f,
        (float)window()->width()/(float)window()->height(),
        0.01f,
        100.0f);

  // Prepare for deferred rendering
  prepareQuad();
  prepareParticles();
  setupFBO();
  setupLights();

  glViewport(0, 0, 720, 720);
}

void Scene::paint()
{
  updateModelMatrix();
  m_input_manager.setupCamera();
  m_input_manager.doMovement();

  m_fbo->bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawParticles();
    if (m_draw_links) { drawLinks(); }
    for(auto &s : m_object_list) { s->draw(); }

  m_fbo->release();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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

  m_quad_program->addShaderFromSourceFile(
        QOpenGLShader::Vertex,
        "shaders/quad.vert");
  m_quad_program->addShaderFromSourceFile(
        QOpenGLShader::Fragment,
        "shaders/quad.frag");

  m_quad_program->link();

  m_quad_program->bind();
  m_quad_program->setUniformValue("gpass0Tex", 0);
  m_quad_program->setUniformValue("gpass1Tex", 1);

  m_normalShadingIndex   = glGetSubroutineIndex(
        m_quad_program->programId(),
        GL_FRAGMENT_SHADER,
        "normalShading");

  m_positionShadingIndex = glGetSubroutineIndex(
        m_quad_program->programId(),
        GL_FRAGMENT_SHADER,
        "positionShading");

  m_activeRenderPassIndex = m_normalShadingIndex;
  m_quad_program->release();

  m_quad_vao = new QOpenGLVertexArrayObject(this);
  m_quad_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_quad_vao->create();
  m_quad_vbo.create();

  m_quad_vao->bind();
    m_quad_vbo.bind();

    m_quad_vbo.allocate(quad, 5 * 6 * sizeof(GLfloat));

    m_quad_program->setAttributeBuffer(
          "position",            // Name
          GL_FLOAT,              // Type
          0,                     // Offset
          3,                     // Tuple size
          5 * sizeof(GLfloat));  // Stride

    m_quad_program->enableAttributeArray("position");

    m_quad_program->setAttributeBuffer(
          "uv",                  // Name
          GL_FLOAT,              // Type
          3 * sizeof(GLfloat),   // Offset
          2,                     // Touple size
          5 * sizeof(GLfloat));  // Stride

    m_quad_program->enableAttributeArray("uv");

    m_quad_vbo.release();
  m_quad_vao->release();
}

void Scene::prepareParticles()
{
  m_part_program = new QOpenGLShaderProgram(this);

  m_part_program->addShaderFromSourceFile(
        QOpenGLShader::Vertex,
        "shaders/particles.vert");

  m_part_program->addShaderFromSourceFile(
        QOpenGLShader::Fragment,
        "shaders/particles.frag");

  m_part_program->link();

  m_links_program = new QOpenGLShaderProgram(this);

  m_links_program->addShaderFromSourceFile(
        QOpenGLShader::Vertex,
        "shaders/links.vert");

  m_links_program->addShaderFromSourceFile(
        QOpenGLShader::Fragment,
        "shaders/links.frag");

  m_links_program->link();

  m_part_vao = new QOpenGLVertexArrayObject(this);
  m_part_vao->create();

  m_links_vao = new QOpenGLVertexArrayObject(this);
  m_links_vao->create();

  m_part_vbo.create();
  m_part_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);

  m_links_ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
  m_links_ebo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  m_links_ebo.create();

  sendParticleDataToOpenGL();  // Initial batch
}

void Scene::drawQuad()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_fbo->takeTexture(0));
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_fbo->takeTexture(1));

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
  m_part_program->setUniformValue("ProjectionMatrix", m_input_manager.getProjectionMatrix());
  m_part_program->setUniformValue("ModelMatrix", m_model_matrix);
  m_part_program->setUniformValue("ViewMatrix", m_input_manager.getViewMatrix());

  m_part_vao->bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, m_sphere_data.size() / 3, m_ps.getSize());
  m_part_vao->release();

  m_part_program->release();
}

void Scene::drawLinks()
{
  m_links_program->bind();
  m_links_program->setUniformValue("ProjectionMatrix", m_input_manager.getProjectionMatrix());
  m_links_program->setUniformValue("ModelMatrix", m_model_matrix);
  m_links_program->setUniformValue("ViewMatrix", m_input_manager.getViewMatrix());

  m_links_vao->bind();
    glDrawElements(GL_LINES, m_links_data.size(), GL_UNSIGNED_INT, 0);
  m_links_vao->release();

  m_links_program->release();
}

void Scene::setupFBO()
{
  // The passes color attachments go as follows
  // GL_COLOR_ATTACHMENT0: gNormalPass
  // GL_COLOR_ATTACHMENT1: gPositionPass

  m_fbo = new QOpenGLFramebufferObject(720, 720); // GL_COLOR_ATTACHMENT0
  m_fbo->bind();
    m_fbo->addColorAttachment(720, 720);            // GL_COLOR_ATTACHMENT1

    const GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    // Create and attach depth buffer (renderbuffer) ===========================
    GLuint rbo_depth;
    // Generate renderbuffer object names
    glGenRenderbuffers(1, &rbo_depth);
    // Bind a named renderbuffer object
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    // Establish data storage, format and dimensions of a renderbuffer object's image
    glRenderbufferStorage(
          GL_RENDERBUFFER,     // Target
          GL_DEPTH_COMPONENT,  // Internal Format
          720, 720);           // Size
    // Attach a renderbuffer object to a framebuffer object
    glFramebufferRenderbuffer(
          GL_FRAMEBUFFER,       // Target
          GL_DEPTH_ATTACHMENT,  // Attachment
          GL_RENDERBUFFER,      // Renderbuffer Target
          rbo_depth);           // Renderbuffer
    // Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      qCritical("Framebuffer not complete!");

  m_fbo->release();
}

void Scene::setupLights()
{
  m_manipulator_program = new QOpenGLShaderProgram(this);

  m_manipulator_program->addShaderFromSourceFile(
        QOpenGLShader::Vertex,
        "shaders/manip.vert");
  m_manipulator_program->addShaderFromSourceFile(
        QOpenGLShader::Fragment,
        "shaders/manip.frag");

  m_manipulator_program->link();

  m_sun_program = new QOpenGLShaderProgram(this);

  m_sun_program->addShaderFromSourceFile(
        QOpenGLShader::Vertex,
        "shaders/sun.vert");
  m_sun_program->addShaderFromSourceFile(
        QOpenGLShader::Fragment,
        "shaders/sun.frag");

  m_sun_program->link();

  QVector3D masterUniqueColour=QVector3D(0.0f, 100.0f, 0.0f);
  for(int x = -4; x < 4; x += 4) {
      for(int y = -4; y < 4; y += 4) {
          PointLight *pointlight;
          pointlight = new PointLight(
                QVector3D(x, y, 0),
                m_manipulator_program,
                m_sun_program);
          pointlight->createGeometry(context(), masterUniqueColour);
          m_object_list.push_back(std::move(std::unique_ptr<PointLight>(pointlight)));
      }
  }
  m_input_manager.addShaderProgram(m_manipulator_program);
  m_input_manager.addShaderProgram(m_sun_program);
  m_input_manager.setObjectList(m_object_list);
}

void Scene::generateSphereData(uint _num_subdivisions)
{
  if (_num_subdivisions < 1) {
    qWarning("Subdivision number must be greater than 0. Using 1 as default.");
    _num_subdivisions = 1;
  }

  m_sphere_data.clear();

  GLfloat vdata[12][3] = {
     {-X, 0.0f, Z}, {X, 0.0f, Z}, {-X, 0.0f, -Z}, {X, 0.0f, -Z},
     {0.0f, Z, X}, {0.0f, Z, -X}, {0.0f, -Z, X}, {0.0f, -Z, -X},
     {Z, X, 0.0}, {-Z, X, 0.0f}, {Z, -X, 0.0f}, {-Z, -X, 0.0f}
  };

  GLuint tindices[20][3] = {
     {0,  4,  1}, {0, 9,  4}, {9,  5, 4}, { 4, 5, 8}, {4, 8,  1},
     {8, 10,  1}, {8, 3, 10}, {5,  3, 8}, { 5, 2, 3}, {2, 7,  3},
     {7, 10,  3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1,  6},
     {6,  1, 10}, {9, 0, 11}, {9, 11, 2}, { 9, 2, 5}, {7, 2, 11}};

  for (size_t i = 0; i < 20; i++) {
    subdivide(
        vdata[tindices[i][0]],
        vdata[tindices[i][1]],
        vdata[tindices[i][2]],
        _num_subdivisions,
        m_sphere_data);
  }
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
  m_ps.packageDataForDrawing(m_particle_data);

  // Uncomment to see what x, y, z, radius get sent to the shader
  // for_each(m_particle_data.begin(), m_particle_data.end(), [](float f){ qDebug("%f", f);});

  m_part_vao->bind();

    // Sphere Data =============================================================
    m_sphere_vbo.bind();
    m_sphere_vbo.allocate(&m_sphere_data[0], m_sphere_data.size() * sizeof(GLfloat));
    m_part_program->enableAttributeArray("position");
    m_part_program->setAttributeBuffer("position", GL_FLOAT, 0, 3);

    // Instance Data ===========================================================
    m_part_vbo.bind();
    m_part_vbo.allocate(&m_particle_data[0], m_ps.getSize() * 4 * sizeof(GLfloat));

    m_part_program->enableAttributeArray("instances");
    m_part_program->setAttributeBuffer("instances", GL_FLOAT, 0, 4);

    m_part_vbo.release();
    glVertexAttribDivisor(m_part_program->attributeLocation("instances"), 1);

  m_part_vao->release();

  // Link Data (on request) ====================================================
  if (m_draw_links)
  {
    // Work on the links O------O
    m_ps.getLinksForDraw(m_links_data);

    // Uncomment to see what indices are being sent to ebo
    // for_each(m_links_data.begin(), m_links_data.end(), [](uint i){ qDebug("%d", i);});

    m_links_vao->bind();
      m_part_vbo.bind();
      m_links_ebo.bind();
      m_links_ebo.allocate(&m_links_data[0], m_links_data.size() * sizeof(uint));
      m_links_program->enableAttributeArray("position");
      m_links_program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 4 * sizeof(GLfloat));
    m_links_vao->release();
  }
}

void Scene::updateModelMatrix()
{
  // Insert particle system position here
  QVector3D pointPos = QVector3D(0, 0, 0);
  m_model_matrix.setToIdentity();
  m_model_matrix.translate(pointPos);
}

void Scene::keyPressed(QKeyEvent *ev)
{
  switch(ev->key()) {
  case Qt::Key_L:
    m_draw_links = !m_draw_links;
  case Qt::Key_N:
    m_activeRenderPassIndex = m_normalShadingIndex;
    break;
  case Qt::Key_P:
    m_activeRenderPassIndex = m_positionShadingIndex;
    break;
  case Qt::Key_Space:
    updateParticleSystem();
    qDebug("%d particles in the system", m_ps.getSize());
  default:
    break;
  }

  m_input_manager.keyPressEvent(ev);
}

void Scene::keyReleaseEvent(QKeyEvent *key)
{
    m_input_manager.keyReleaseEvent(key);
}

void Scene::mouseMoveEvent(QMouseEvent* event)
{
    m_input_manager.mouseMoveEvent(event);
}

void Scene::mousePressEvent(QMouseEvent *event)
{
    m_input_manager.mousePressEvent(event);
}

void Scene::mouseReleaseEvent(QMouseEvent *event)
{
    m_input_manager.mouseReleaseEvent(event);
}

void Scene::wheelEvent(QWheelEvent *event)
{
    m_input_manager.wheelEvent(event);
}

void Scene::windowResized(int _w, int _h)
{
  qDebug("Window resized to %d and %d", _w, _h);
}

void pushTriangle(float *v1, float *v2, float *v3, std::vector<GLfloat>& _data)
{
  _data.push_back(v1[0]); _data.push_back(v1[1]); _data.push_back(v1[2]); // v1
  _data.push_back(v2[0]); _data.push_back(v2[1]); _data.push_back(v2[2]); // v2
  _data.push_back(v3[0]); _data.push_back(v3[1]); _data.push_back(v3[2]); // v3
}

void subdivide(float *v1, float *v2, float *v3, long depth,std::vector<GLfloat>& _data)
{
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

  // Normalize (push to match sphere sphape)
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
