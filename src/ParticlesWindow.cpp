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
#include "particleswindow.h"

// Pre-declaring the functions in helpers.cpp so that compiler is aware that
// they exist. Read on helpers.cpp to see how these guys work...
std::string readFile(const char *filePath);
int WindowDump(void);
void createSpherePoints(
    std::vector<QVector3D>& vertices,
    float radius,
    unsigned int rings,
    unsigned int sectors);

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

    // Depth is needed otherwise the ones in the back appear on top
    glEnable (GL_DEPTH_TEST);

    // Get the vertex data as strings
    std::string vertexShaderString = readFile("shaders/vertex.glsl");
    std::string fragmentShaderString = readFile("shaders/fragment.glsl");
    const char *vertexShaderSource = vertexShaderString.c_str();
    const char *fragmentShaderSource = fragmentShaderString.c_str();

    // Similar to NGL way of initializing, compiling and linking shaders.
    m_program_particles = new QOpenGLShaderProgram(this);
    m_program_particles->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program_particles->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program_particles->link();

    // Populate positions (QVector3D) with points layed out in a spherical way
    m_particlePos = std::vector<QVector3D>(0);
    createSpherePoints(m_particlePos, 0.4, 50, 60); // (radius, u_divisions, v_divisions)

    // OpenGL wants a flat array of GLfloats
    m_numberOfParticles = m_particlePos.size();
    for (unsigned int i = 0; i < m_numberOfParticles; i++)
    {
        m_particlePosArray.push_back(m_particlePos[i].x());
        m_particlePosArray.push_back(m_particlePos[i].y());
        m_particlePosArray.push_back(m_particlePos[i].z());
    }

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
    m_program_particles->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
    m_program_particles->enableAttributeArray("posAttr");

    // VAO unbinding (will be bound/released again in render() method, the game loop)
    m_VAO_particle->release();
}

// Rendering
void ParticlesWindow::render()
{
    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind the right program with the right VAO
    m_program_particles->bind();
    m_VAO_particle->bind();

    // Do the drawing
    glEnable(GL_POINT_SPRITE);
    glPointSize(12.0f);
    glDrawArrays(GL_POINTS, 0, m_numberOfParticles);

    // Unbound (release)
    m_VAO_particle->release();
    m_program_particles->release();

    update_stuff();

    // Increment time.
    ++m_frame;
}

// Updating whatever... Positions, velocities... Create your own methods...
void ParticlesWindow::update_stuff()
{
    // We first change the dataand after we buffer it to the right buffer
    for (unsigned int i = 0; i < m_numberOfParticles; i++)
    {
        m_particlePosArray[3 * i + 0] += 0.001f * sin((float)(m_frame+i) / 50.0f);
        m_particlePosArray[3 * i + 1] += 0.001f * cos((float)(m_frame+i) / 50.0f);
        m_particlePosArray[3 * i + 2] += 0.001f * sin((float)(m_frame+i) / 50.0f);
    }

    m_VBO_particlePos->bind();
    // For buffering you can pick the write() or allocate() method.
    // http://doc.qt.io/qt-5/qopenglbuffer.html#write
    // http://doc.qt.io/qt-5/qopenglbuffer.html#allocate
    // Remember that you can place your cursor on a function and press F1 to open the docs within Qt!
    //     m_VBO_particlePos->write(0, &m_particlePosArray[0], m_numberOfParticles * 3 * sizeof(GLfloat));
    //     m_VBO_particlePos->allocate(&m_particlePosArray[0], m_numberOfParticles * 3 * sizeof(GLfloat));
    m_VBO_particlePos->write(0, &m_particlePosArray[0], m_numberOfParticles * 3 * sizeof(GLfloat));
    m_VBO_particlePos->release();
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
