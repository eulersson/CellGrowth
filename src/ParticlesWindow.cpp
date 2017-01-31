// Native
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

// External
#include <bitmap_image.hpp>

// Qt
#include <QKeyEvent>
#include <QOpenGLShaderProgram>

// Custom
#include "ParticlesWindow.h"

// Pre-declaring the functions in helpers.cpp
std::string readFile(const char *filePath);
int WindowDump(void);


// Constructor method needs to set up the format.
ParticlesWindow::ParticlesWindow()
  : m_program_particles(0)
  , m_frame(0)
{
  QSurfaceFormat format;
  format.setSamples(16);
  this->resize(720, 720);
  this->setFormat(format);
}

// OpenGL & member initialization
void ParticlesWindow::initialize()
{
  // Set the glViewport to be the same as QWindow's size
  glViewport(0, 0, width(), height());

  // Get the vertex data as strings
  std::string vertexShaderString   = readFile("shaders/vertex.glsl");
  std::string fragmentShaderString = readFile("shaders/fragment.glsl");
  std::string linesShaderString    = readFile("shaders/lines.glsl");
  const char *vertexShaderSource   = vertexShaderString.c_str();
  const char *fragmentShaderSource = fragmentShaderString.c_str();
  const char *linesShaderSource    = linesShaderString.c_str();

  // Program shader for rendering the points as spheres
  m_program_particles = new QOpenGLShaderProgram(this);
  m_program_particles->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
  m_program_particles->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
  m_program_particles->link();

  // Program to render the links between the particles
  m_program_lines = new QOpenGLShaderProgram(this);
  m_program_lines->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
  m_program_lines->addShaderFromSourceCode(QOpenGLShader::Fragment, linesShaderSource);
  m_program_lines->link();

  //
  // PARTICLES //////
  //
  m_particlePos = std::vector<QVector3D>(0);
  m_numberOfParticles = m_ps.get_size();

  for (unsigned int i = 0; i < m_numberOfParticles; i++)
  {
    QVector3D position;
    LinkedParticle* lp = m_ps.get_particle(i);
    lp->getPos(position);
    m_particlePosArray.push_back(position.x());
    m_particlePosArray.push_back(position.y());
    m_particlePosArray.push_back(position.z());
  }

  //
  // LINKS //////
  //
  std::vector<QVector3D> lines;
  m_ps.getLinksForDraw(lines);
  m_numberOfLinePositions = lines.size();

  for (unsigned int i = 0; i < m_numberOfLinePositions; i++)
  {

    m_particleLinesPosArray.push_back(lines[i].x());
    m_particleLinesPosArray.push_back(lines[i].y());
    m_particleLinesPosArray.push_back(lines[i].z());
  }

  //
  // PARTICLES //////
  //
  // Initialize VAO and VBO Qt objects
  m_VAO_particle = new QOpenGLVertexArrayObject(this);
  m_VBO_particlePos = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

  // Create the actual OpenGL VAO and VBO
  m_VAO_particle->create();
  m_VBO_particlePos->create(); m_VBO_particlePos->setUsagePattern(QOpenGLBuffer::DynamicDraw);

  // Bind them so we can buffer the data to the VBO while the VAO will remember
  // the currently bound VBO (so don't unbound the VBO)
  m_VAO_particle->bind();

  // PRESS F1 OVER THE ALLOCATE FUNCTION TO OPEN QT'S DOCUMENTATION ON THAT METHOD!
  // 1. Bind VAO
  // 2. Bind VBO
  // 3. Buffer Data
  // 4. Set attribute pointers
  // 5. Unbind VAO
  m_VBO_particlePos->bind();
  m_VBO_particlePos->allocate(&m_particlePosArray[0], 3 * m_numberOfParticles * sizeof(GLfloat));

  // Set the current bound buffer data to this attribute pointers
  m_program_particles->setUniformValue("delta", 0.0f);
  m_program_particles->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
  m_program_particles->enableAttributeArray("posAttr");

  // VAO unbinding (will be bound/released again in render() method, the game loop)
  m_VAO_particle->release();

  // Depth is needed otherwise the ones in the back appear on top
  glEnable (GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
  glDepthFunc (GL_LEQUAL);

  //
  // LINKS //////
  //
  // Initialize VAO and VBO Qt objects
  m_VAO_lines = new QOpenGLVertexArrayObject(this);
  m_VBO_particleLinePos = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

  // Create the actual OpenGL VAO and VBO
  m_VAO_lines->create();
  m_VBO_particleLinePos->create(); m_VBO_particleLinePos->setUsagePattern(QOpenGLBuffer::DynamicDraw);

  // Bind them so we can buffer the data to the VBO while the VAO will remember
  // the currently bound VBO (so don't unbound the VBO)
  m_VAO_lines->bind();
  m_VBO_particleLinePos->bind();
  m_VBO_particleLinePos->allocate(&m_particleLinesPosArray[0], 3 * m_numberOfLinePositions * sizeof(GLfloat));

  // In this case we will be reusing the same positions for the lines
  m_program_lines->setUniformValue("delta", 0.0f);
  m_program_lines->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
  m_program_lines->enableAttributeArray("posAttr");

  // VAO unbinding (will be bound/released again in render() method, the game loop)
  m_VAO_lines->release();
}

// Rendering
void ParticlesWindow::render()
{

  // Clear the buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind the right program with the right VAO
  m_VAO_lines->bind();

  // Draw the LINKS between particles for visual debugging
  m_program_lines->bind();
  m_program_lines->setUniformValue("delta", (float)m_frame / 100.0f);
  glDrawArrays(GL_LINES, 0, m_numberOfLinePositions);
  m_program_lines->release();

  m_VAO_lines->release();

  m_VAO_particle->bind();

  // Draw the POINTS as spheres
  m_program_particles->bind();
  m_program_particles->setUniformValue("delta", (float)m_frame / 100.0f);
  glEnable(GL_POINT_SPRITE);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glDrawArrays(GL_POINTS, 0, m_numberOfParticles);
  m_program_particles->release();

  // Unbind (release)
  m_VAO_particle->release();

  // Make the particle system step in this case... Create your own functions for changing state
   update_stuff();

  // Increment time.
  ++m_frame;
}

// Updating whatever... Positions, velocities... Create your own methods...
void ParticlesWindow::update_stuff()
{
  m_ps.advance();

  // POSITIONS
  m_numberOfParticles = m_ps.get_size();
  m_particlePosArray.resize(m_numberOfParticles * 3);

  //Jon says we should be using the container size instead of the particle
  for (unsigned int i = 0; i < m_numberOfParticles; i++)
  {
    QVector3D position;
    LinkedParticle* lp = m_ps.get_particle(i);
    lp->getPos(position);


    m_particlePosArray[3*i  ] = position.x();
    m_particlePosArray[3*i+1] = position.y();
    m_particlePosArray[3*i+2] = position.z();

  }
  m_VBO_particlePos->bind();
  m_VBO_particlePos->allocate(&m_particlePosArray[0], m_numberOfParticles * 3 * sizeof(GLfloat));
  m_VBO_particlePos->release();

  // LINKS
  std::vector<QVector3D> lines;
  m_ps.getLinksForDraw(lines);
  m_numberOfLinePositions = lines.size();
  m_particleLinesPosArray.resize(m_numberOfLinePositions*3);

  for (unsigned int i = 0; i < m_numberOfLinePositions; i++)
  {

    m_particleLinesPosArray[3*i  ] = lines[i].x();
    m_particleLinesPosArray[3*i+1] = lines[i].y();
    m_particleLinesPosArray[3*i+2] = lines[i].z();
  }

  m_VBO_particleLinePos->bind();
  m_VBO_particleLinePos->allocate(&m_particleLinesPosArray[0], m_numberOfLinePositions * 3 * sizeof(GLfloat));
  m_VBO_particleLinePos->release();

}

// Save current frame buffer as a .bmp file
void ParticlesWindow::saveImage()
{
  std::string filename;
  std::stringstream stream;

  stream << "image." << std::setfill('0') << std::setw(6) << m_frame << ".bmp";
  filename = stream.str();

  std::cout << "Saving image " << filename << std::endl;

  const char* filenameStr = filename.c_str();
  unsigned char* imageData = (unsigned char *)malloc((int)(width()*height()*(3)));

  glReadPixels(0, 0, width(), height(), GL_RGB, GL_UNSIGNED_BYTE, imageData);
  bitmap_image image(width(),height());
  image_drawer draw(image);

  for (unsigned int i = 0; i < image.width(); ++i)
    for (unsigned int j = 0; j < image.height(); ++j)
      image.set_pixel(i,j,*(++imageData),*(++imageData),*(++imageData));

  image.save_image(filenameStr);
}

void ParticlesWindow::keyPressEvent(QKeyEvent *ev)
{
  // READ DOCS: QKeyEvent http://doc.qt.io/qt-5/qkeyevent.html
  if (ev->key() == Qt::Key_S)
    saveImage();

  if (ev->key() == Qt::Key_A)
    std::cout << "Key A has been pressed";

  if (ev->key() == Qt::Key_Space)
  {
    m_ps.splitRandomParticle();
    update_stuff();
  }
}

void ParticlesWindow::mouseMoveEvent(QMouseEvent *ev)
{
    // READ DOCS: QMouseEvent http://doc.qt.io/qt-5/qmouseevent.html
}

void ParticlesWindow::mousePressEvent(QMouseEvent *ev)
{
    // READ DOCS: QMouseEvent http://doc.qt.io/qt-5/qmouseevent.html
}

void ParticlesWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    // READ DOCS: QMouseEvent http://doc.qt.io/qt-5/qmouseevent.html
}

void ParticlesWindow::resizeEvent(QResizeEvent *ev)
{
    // READ DOCS: QResizeEvent http://doc.qt.io/qt-4.8/qresizeevent.html
}
