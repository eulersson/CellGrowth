
////////////////////////////////////////////////////////////////////////////////
/// @file Window.cpp
/// @author Ramon Blanquer
/// @author Fanny Marstrom
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////


// Qt
#include <QKeyEvent>

// Project
#include "GLWindow.h"

// Recursion subdivision algorithm from:
// http://www.opengl.org.ru/docs/pg/0208.html

#define X .25731112119133606
#define Z .850650808352039932

void subdivide(float*, float*, float*, long, std::vector<GLfloat>&);
float lerp(float a, float b, float f);

GLWindow::GLWindow(QWidget*_parent)
  : m_draw_links(true)
//GLWindow::GLWindow(QMainWindow*_parent)
//  : QOpenGLWidget(_parent)
//  , m_draw_links(true)
//  , m_input_manager(this)
{
  this->resize(_parent->size());
  QSurfaceFormat fmt;
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  fmt.setMajorVersion(4);
  fmt.setMinorVersion(1);
  fmt.setSamples(16);
  fmt.setSwapInterval(1);
  setFormat(fmt);

  setMouseTracking(true);
  setFocus();

  connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
  if(format().swapInterval() == -1)
  {
      // V_blank synchronization not available (tearing likely to happen)
      qDebug("Swap Buffers at v_blank not available: refresh at approx 60fps.");
      m_timer.setInterval(17);
  }
  else
  {
      // V_blank synchronization available
      m_timer.setInterval(0);
  }
  m_timer.start();
}


GLWindow::~GLWindow()
{
  qDebug("Window::~Window - Do cleanup");
}

void GLWindow::initializeGL()
{
  initializeOpenGLFunctions();

  m_input_manager = new InputManager(this);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);

  generateSphereData(4); // Four subdivisions of an icosahedra

  m_sphere_vbo.create();
  m_sphere_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

  //Initializing all matricies that will be used.
  initializeMatrix();

  // Prepare for deferred rendering
  prepareQuad();
  prepareParticles();
  setupFBO();
  setupLights();
  sampleKernel();

}

void GLWindow::paintGL()
{
  updateModelMatrix();

  m_input_manager->setupCamera(width(), height());
  m_input_manager->doMovement();

  m_fbo->bind();
    glViewport(0, 0, width(), height());
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    drawParticles();
    if (m_draw_links) { drawLinks(); }
    for(auto &s : m_object_list) { s->draw(); }


    //Update light, material and matrix data.
    loadMaterialToShader();
    loadLightToShader();
    loadMatrixToShader();

  m_fbo->release();
    glViewport(0, 0, width(), height());
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (m_activeRenderPassIndex == m_xRayIndex)
    {
      glEnable(GL_BLEND);
      glEnable(GL_CULL_FACE);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBlendEquation(GL_MAX);
    }
    else if (m_activeRenderPassIndex == m_ADSIndex)
    {
      glDisable(GL_CULL_FACE);
      glDisable(GL_BLEND);
    }
    else if (m_activeRenderPassIndex == m_AOIndex)
    {
      glDisable(GL_CULL_FACE);
      glDisable(GL_BLEND);
    }

    drawQuad();

    updateParticleSystem();
}

void GLWindow::resizeGL(int _w, int _h)
{
//  glViewport(0,0,_w,_h);
  delete m_fbo;
  setupFBO();

  m_input_manager->resized(_w, _h);
  //m_input_manager->setupCamera(_w, _h);


  m_projection_matrix.setToIdentity();
  m_projection_matrix.perspective(
        45.0f,
        (float)_w / (float)_h,
        0.1f,
        100.0f);

  //m_mv = m_view_matrix * m_model_matrix;
  //m_mvp = m_projection_matrix * m_mv;

  qDebug("Window resized to %d and %d", _w, _h);
  m_quad_program->bind();
  m_quad_program->setUniformValue("width", _w);
  m_quad_program->setUniformValue("height", _h);
  m_quad_program->release();
}

void GLWindow::initializeMatrix()
{
  m_projection_matrix.setToIdentity();
  m_projection_matrix.perspective(
        45.0f,
        (float)width() / (float)height(),
        0.1f,
        100.0f);

  m_model_matrix.setToIdentity();
  m_model_matrix.translate(0.0, 0.0, 0.0);


  m_mv = m_view_matrix * m_model_matrix;
  m_mvp = m_projection_matrix * m_mv;
}


void GLWindow::loadMatrixToShader()
{
    m_quad_program->bind();
    m_quad_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
    m_quad_program->setUniformValue("ViewMatrix", m_view_matrix);
    m_quad_program->setUniformValue("ModelMatrix", m_model_matrix);
    m_quad_program->setUniformValue("MV", m_mv);
    m_quad_program->setUniformValue("MVP", m_mvp);
    m_quad_program->release();

    m_part_program->bind();
    m_part_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
    m_part_program->setUniformValue("ViewMatrix", m_view_matrix);
    m_part_program->setUniformValue("ModelMatrix", m_model_matrix);
    m_part_program->setUniformValue("MV", m_mv);
    m_part_program->release();

}

void GLWindow::loadCameraInfoToShader()
{
    m_quad_program->bind();
    m_viewPos = QVector3D(0.0, 0.0, 0.0);
    m_quad_program->setUniformValue("viewPos", m_viewPos);
    m_quad_program->release();
}


void GLWindow::loadLightToShader()
{
    m_quad_program->bind();


    m_lightPos = m_object_list[0]->getPosition();
    m_quad_program->setUniformValue("lightPos", m_lightPos);


    //Phong Lighting
    //The ambient lighting used as a global illumination.
    m_quad_program->setUniformValue("light.ambient", QVector3D(1.0f, 1.0f, 1.0f));

    //Diffuse light to make fragments closer to a light source more birght.
    m_quad_program->setUniformValue("light.diffuse", QVector3D(0.5f, 0.5f, 0.5f));

    //Specular lighting to change the reflective properties.
    m_quad_program->setUniformValue("light.specular", QVector3D(1.0f, 1.0f, 1.0f));

    //Specular lighting to change the reflective properties.
    m_quad_program->setUniformValue("light.colour", QVector3D(0.5f, 0.2f, 1.0f));

    m_quad_program->setUniformValue("light.Linear", 0.09f);
    m_quad_program->setUniformValue("light.Quadratic", 0.032f);

    m_quad_program->release();

}

void GLWindow::loadMaterialToShader()
{
  m_quad_program->bind();

  //Phong material properties.
  m_quad_program->setUniformValue("material.ambient", QVector3D(0.5f, 0.5f, 0.5f));
  m_quad_program->setUniformValue("material.diffuse", QVector3D(0.3f, 0.3f, 0.3f));
  m_quad_program->setUniformValue("material.specular", QVector3D(0.5f, 0.5f, 0.5f));
  m_quad_program->setUniformValue("material.shininess", 32.0f);

  //X-Ray Fall of to make only edges in full opacity.
  m_quad_program->setUniformValue("material.attenuation", 0.5f);

  m_quad_program->release();
}

void GLWindow::prepareQuad()
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

  //Passing textures to shader.
  m_quad_program->setUniformValue("depth", 0);
  m_quad_program->setUniformValue("positionTex", 1);
  m_quad_program->setUniformValue("normal", 2);
  m_quad_program->setUniformValue("diffuse", 3);
  m_quad_program->setUniformValue("ssaoNoiseTex", 4);
  m_quad_program->setUniformValue("ScreenNormals", 5);


  //Subroutine ShadingPass Index.
  m_ADSIndex    = glGetSubroutineIndex(m_quad_program->programId(), GL_FRAGMENT_SHADER, "ADSRender");
  m_AOIndex     = glGetSubroutineIndex(m_quad_program->programId(), GL_FRAGMENT_SHADER, "AORender");
  m_xRayIndex   = glGetSubroutineIndex(m_quad_program->programId(), GL_FRAGMENT_SHADER, "xRayRender");

  //Setting the active renderpass.
  m_activeRenderPassIndex = m_normalIndex;
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

void GLWindow::prepareParticles()
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

void GLWindow::drawQuad()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_fbo->takeTexture(0));
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_fbo->takeTexture(1));
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, m_fbo->takeTexture(2));
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, m_fbo->takeTexture(3));
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, m_fbo->takeTexture(4));
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D, m_fbo->takeTexture(5));


  m_quad_program->bind();
  m_quad_vao->bind();
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_activeRenderPassIndex);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
  m_quad_vao->release();
  m_quad_program->release();
}

void GLWindow::drawParticles()
{
  m_part_program->bind();
  m_part_program->setUniformValue("ProjectionMatrix", m_input_manager->getProjectionMatrix());
  m_part_program->setUniformValue("ModelMatrix", m_model_matrix);
  m_part_program->setUniformValue("ViewMatrix", m_input_manager->getViewMatrix());

  m_part_vao->bind();
  glDrawArraysInstanced(GL_TRIANGLES, 0, m_sphere_data.size() / 3, m_ps.getSize());
  m_part_vao->release();

  m_part_program->release();
}

void GLWindow::drawLinks()
{
  m_links_program->bind();

  m_links_program->setUniformValue("ProjectionMatrix", m_input_manager->getProjectionMatrix());
  m_links_program->setUniformValue("ModelMatrix", m_model_matrix);
  m_links_program->setUniformValue("ViewMatrix", m_input_manager->getViewMatrix());

  m_links_vao->bind();
    glDrawElements(GL_LINES, m_links_data.size(), GL_UNSIGNED_INT, 0);
  m_links_vao->release();

  m_links_program->release();
}


void GLWindow::setupFBO()
{
  m_fbo =new QOpenGLFramebufferObject(
        width(), height(),
        QOpenGLFramebufferObject::Depth);         // GL_COLOR_ATTACHMENT0

  m_fbo->bind();

  m_fbo->addColorAttachment(width(), height());            // GL_COLOR_ATTACHMENT1
  m_fbo->addColorAttachment(width(), height());            // GL_COLOR_ATTACHMENT2
  m_fbo->addColorAttachment(width(), height());            // GL_COLOR_ATTACHMENT3
  m_fbo->addColorAttachment(width(), height());            // GL_COLOR_ATTACHMENT4
  m_fbo->addColorAttachment(width(), height());            // GL_COLOR_ATTACHMENT5

  const GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
                                GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5};

  // Drawing multiple buffers.
  glDrawBuffers(6, attachments);
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
          width(), height());           // Size
    // Attach a renderbuffer object to a framebuffer object
    glFramebufferRenderbuffer(
          GL_FRAMEBUFFER,       // Target
          GL_DEPTH_ATTACHMENT,  // Attachment
          GL_RENDERBUFFER,      // Renderbuffer Target
          rbo_depth);           // Renderbuffer
    // Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      qCritical("Framebuffer not complete!");

  glViewport(0, 0, width(), height());
  m_fbo->release();
}


void GLWindow::sampleKernel()
{
  std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
  for(GLuint i = 0; i < m_ssaoKernel.size(); i++)
  {
      QVector3D sample(

              randomFloats(generator) * 2.0 - 1.0,
              randomFloats(generator) * 2.0 - 1.0,
              randomFloats(generator)
       );
       sample.normalize();
       sample *= randomFloats(generator);
       GLfloat scale = GLfloat(i) / 64.0;
       scale = lerp(0.1f, 1.0f, scale * scale);
       sample *= scale;

       m_ssaoKernel[i] = sample;

       m_quad_program->bind();
       char buffer [12];
       int n = sprintf (buffer, "samples[%d]", i);
       m_quad_program->setUniformValue(buffer, sample);
       m_quad_program->release();
  }
}

void GLWindow::randomKernel()
{
  std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
  m_ssaoNoise.clear();
  for(GLuint i = 0; i < 16; i++)
  {
      m_ssaoNoise.push_back(randomFloats(generator) * 2.0 - 1.0);
      m_ssaoNoise.push_back(randomFloats(generator) * 2.0 - 1.0);
      m_ssaoNoise.push_back(0.0f);
  }

  QOpenGLTexture* ssaoNoise_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
  ssaoNoise_texture->setMinificationFilter(QOpenGLTexture::Nearest);
  ssaoNoise_texture->setMagnificationFilter(QOpenGLTexture::Nearest);
  ssaoNoise_texture->setWrapMode(QOpenGLTexture::Repeat);

  ssaoNoise_texture->setSize(4,4);
  ssaoNoise_texture->setFormat(QOpenGLTexture::RGB16F);


  ssaoNoise_texture->allocateStorage();
  ssaoNoise_texture->setData(0, QOpenGLTexture::RGB, QOpenGLTexture::Float32, &m_ssaoNoise[0]);

  ssaoNoise_texture->bind(5);
}

void GLWindow::setupLights()
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

  PointLight *pointlight;
  pointlight = new PointLight(
        QVector3D(0, 0, 0),
        m_manipulator_program,
        m_sun_program);
  pointlight->createGeometry(context(), masterUniqueColour);
  m_object_list.push_back(std::move(std::unique_ptr<PointLight>(pointlight)));


  m_input_manager->addShaderProgram(m_manipulator_program);
  m_input_manager->addShaderProgram(m_sun_program);
  m_input_manager->setObjectList(m_object_list);
}

void GLWindow::generateSphereData(uint _num_subdivisions)
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

void GLWindow::updateParticleSystem()
{
  //m_ps.splitRandomParticle();
  m_ps.advance();
  sendParticleDataToOpenGL();
}

void GLWindow::sendParticleDataToOpenGL()
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

void GLWindow::updateModelMatrix()
{
  // Insert particle system position here
  QVector3D pointPos = QVector3D(0, 0, 0);
  m_model_matrix.translate(pointPos);
}

void GLWindow::keyPressEvent(QKeyEvent* ev)
{
    setFocus();
    switch(ev->key())
    {
        case Qt::Key_Space:
               m_ps.splitRandomParticle();
            qDebug("%d particles in the system", m_ps.getSize());
            break;

        case Qt::Key_1:
            m_activeRenderPassIndex = m_ADSIndex;
            qDebug("ADS Render.");
            break;

        case Qt::Key_2:
            m_activeRenderPassIndex = m_xRayIndex;
            qDebug("X-Ray visualisation.");
            break;

        case Qt::Key_3:
            m_activeRenderPassIndex = m_AOIndex;
            qDebug("Ambient Occlusion.");
            break;

        default:
            break;
    }

  m_input_manager->keyPressEvent(ev);

}

void GLWindow::keyReleaseEvent(QKeyEvent *key)
{
  setFocus();
  m_input_manager->keyReleaseEvent(key);
}

void GLWindow::mouseMoveEvent(QMouseEvent* event)
{
  makeCurrent();
  setFocus();
  m_input_manager->mouseMoveEvent(event);
}

void GLWindow::mousePressEvent(QMouseEvent *event)
{
  makeCurrent();
  setFocus();
  m_input_manager->mousePressEvent(event);
}

void GLWindow::mouseReleaseEvent(QMouseEvent *event)
{
  makeCurrent();
  setFocus();
  m_input_manager->mouseReleaseEvent(event);
}

void GLWindow::wheelEvent(QWheelEvent *event)
{
  makeCurrent();
  setFocus();
  m_input_manager->wheelEvent(event);
}

// Helpers
float lerp(float a, float b, float f)
{
    return a + f * (b -a);
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

// Slots

void GLWindow::setParticleSize(double _size)
{
  m_ps.setParticleSize(_size);
  sendParticleDataToOpenGL();
}

void GLWindow::setParticleType(int _type)
{m_ps.splitRandomParticle();


  emit resetForces(true);
  emit resetCohesion(30);
  emit resetSpring(30);
  emit resetChildrenThreshold(3);
  emit resetBranchLength(3.0);

  char particleType;
  if (_type==0)
  {
    particleType='L';

    emit enableGrowthParticle(false);
    emit enableLinkedParticle(true);

  }
  else
  {
    particleType='G';

    emit enableGrowthParticle(true);
    emit enableLinkedParticle(false);

  }

  m_ps.reset(particleType);
  sendParticleDataToOpenGL();
}

void GLWindow::cancle()
{
  //stop program running
}

void GLWindow::showConnections(bool _state)
{
  //Visualisation changes
  //maybe there could be an attribute here that could be toggled and tested when rendering
  sendParticleDataToOpenGL();
}

void GLWindow::setShading(QString _type)
{
  //same here maybe have attribute that then gets passed to shader??
  sendParticleDataToOpenGL();
}

void GLWindow::toggleForces(bool _state)
{
  //only for LinkedParticles
  m_ps.toggleForces(_state);
  sendParticleDataToOpenGL();
}

void GLWindow::setCohesion(int _amount)
{
  //only for LinkedParticles
  m_ps.setCohesion(_amount);
  sendParticleDataToOpenGL();
}

void GLWindow::bulge()
{
  //only for LinkedParticles
  m_ps.bulge();
  sendParticleDataToOpenGL();
}

void GLWindow::setSpring(int _amount)
{
  //only for LinkedParticles
  m_ps.setSpring(_amount);
  sendParticleDataToOpenGL();
}

void GLWindow::setBranchLength(double _amount)
{

  //only for GrowthParticles
  m_ps.setBranchLength(_amount);
  sendParticleDataToOpenGL();
}

void GLWindow::setGrowthRadius(int _amount)
{
  //only for GrowthParticles
  m_ps.setGrowthRadius(_amount);
  sendParticleDataToOpenGL();
}

void GLWindow::restart()
{
  //restart program


  emit resetParticleSize(2);
  emit resetParticleType(0);
  emit resetParticleTap(0);
  emit resetForces(true);
  emit resetCohesion(30);
  emit resetSpring(30);
  emit resetChildrenThreshold(3);
  emit resetBranchLength(3.0);

  //add reset functions here

}

void GLWindow::setSplitType(QString _type)
{
  //not sure where to put this really
}

void GLWindow::setChildThreshold(int _amount)
{
  m_ps.setChildThreshold(_amount);
}
