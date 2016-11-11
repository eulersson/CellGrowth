#include <iostream>
#include <QOpenGLShaderProgram>
#include "particleswindow.h"

// Pre-declaring the functions in helpers.cpp so that compiler is aware of their existance.
// Read on helpers.cpp to see how it works... You don't need to.
std::string readFile(const char *filePath);
void createSpherePoints(
    std::vector<QVector3D>& vertices,
    float radius,
    unsigned int rings,
    unsigned int sectors);

// Constructor method needs to set up the format. Someone could work out how to update and resize
// the context when resizing the window.
ParticlesWindow::ParticlesWindow()
    : m_program_particles(0)
    , m_frame(0)
{
    QSurfaceFormat format;
    format.setSamples(16);
    this->resize(720, 720);
    this->setFormat(format);
}

// Initialization
void ParticlesWindow::initialize()
{
    // set the glViewport to be the same as the QWindow
    glViewport(0, 0, width(), height());

    // Depth is needed otherwise the ones in the back appear on top, interesting though...
    glEnable (GL_DEPTH_TEST);

    // Get the vertex data as strings
    std::string vertexShaderString = readFile("shaders/vertex.glsl");
    std::string fragmentShaderString = readFile("shaders/fragment.glsl");
    const char *vertexShaderSource = vertexShaderString.c_str();
    const char *fragmentShaderSource = fragmentShaderString.c_str();

    // Similar to NGL way of initializing, compiling and linking shaders. Simpler.
    m_program_particles = new QOpenGLShaderProgram(this);
    m_program_particles->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program_particles->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program_particles->link();

    // Populate the vector of positions (QVector3D) with points layed out in a spherical way
    m_positions_particles = std::vector<QVector3D>(0);
    createSpherePoints(m_positions_particles, 0.4, 50, 60);

    // OpenGL wants a flat array of GLfloats so we transverse through the positions and flatten the coordinates
    unsigned int numParticles = m_positions_particles.size();
    GLfloat verts[3*numParticles];
    for (unsigned int i = 0; i < numParticles; i++)
    {
        verts[3*i+0] = m_positions_particles[i].x();
        verts[3*i+1] = m_positions_particles[i].y();
        verts[3*i+2] = m_positions_particles[i].z();
    }

    // Initialize VAO and VBO Qt objects
    m_VAO_particles = new QOpenGLVertexArrayObject(this);
    m_VBO_particles = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    // Create the actual OpenGL VAO and VBO
    m_VAO_particles->create();
    m_VBO_particles->create(); m_VBO_particles->setUsagePattern(QOpenGLBuffer::DynamicDraw);

    // Bind them so we can buffer the data to the VBO while the VAO will remember the currently bound VBO
    m_VAO_particles->bind();
    m_VBO_particles->bind();

    // Buffering... PRESS F1 OVER THE ALLOCATE FUNCTION TO OPEN QT'S DOCUMENTATION ON THAT METHOD! IT'S GREAT!
    m_VBO_particles->allocate(verts, 3 * m_positions_particles.size() * sizeof(GLfloat));

    // Configure the right attributes that this data will be bound to
    m_program_particles->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
    m_program_particles->enableAttributeArray("posAttr");

    // Release before finishing initialization (will be bound/released again in render() method, the game loop)
    m_VBO_particles->release();
    m_VAO_particles->release();

}

// Rendering
void ParticlesWindow::render()
{
    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind the right program with the right VAO
    m_program_particles->bind();
    m_VAO_particles->bind();

    // Do the drawing
    glEnable(GL_POINT_SPRITE);
    glPointSize(12.0f);
    glDrawArrays(GL_POINTS, 0, m_positions_particles.size());

    // Unbound (release)
    m_VAO_particles->release();
    m_program_particles->release();

    // Increment time.
    ++m_frame;
}

/* FOR THE EVENTS:
 * Imagine you would like tell OpenGL that the vertices are changed on a specific event.
 * You will need to bind the right VBO and update (aka buffer) the data again.
 *   m_VBO->bind();
 *   m_VBO->allocate(pointerToVerts, numberOfVerts * sizeof(GLfloat));  // I think you could use m_VBO.write(...) as well
 *   m_VBO->release();
 * */

// Intercepting the events
void ParticlesWindow::keyPressEvent(QKeyEvent *ev)
{
    // READ DOCS: QKeyEvent http://doc.qt.io/qt-5/qkeyevent.html

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
