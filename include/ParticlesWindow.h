#ifndef PARTICLESWINDOW_H
#define PARTICLESWINDOW_H

// Qt
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

// Custom
#include "ParticleSystem.h"
#include "Viewport.h"

// NOTE: Get familiarized with these classes:
//   - QWindow                        http://doc.qt.io/qt-5/qwindow.html
//   - QOpenGLBuffer                  http://doc.qt.io/qt-5/qopenglbuffer.html
//   - QOpenGLShaderProgram (VBO)     http://doc.qt.io/qt-5/qopenglshaderprogram.html
//   - QOpenGLVertexArrayObject (VAO) http://doc.qt.io/qt-5/qopenglvertexarrayobject.html

// Inherit from Viewport and override the initialize and render methods.
class ParticlesWindow : public Viewport
{
public:
    // Constructor initializes a QSurfaceFormat
    ParticlesWindow();

    // Initialize OpenGL and prepare VAOs, VBOs, shader program, attributes...
    void initialize() Q_DECL_OVERRIDE;

    // Do the drawing
    void render() Q_DECL_OVERRIDE;

    // For changing data and buffering it back to the right VBO... Edit to your own needs or create your own
    void update_stuff();

    // For demonstrating how to use key events. When 's' is pressed we save a .bmp
    void saveImage();

private:
    // Note that each particle has 3 components (x,y,z)
    unsigned int m_numberOfParticles;

    // Mocking up all the positions the particles will need to be drawn to.
    // Of this form: [particle0Pos, particle1Pos, particle2Pos, particle3Pos...]
    std::vector<QVector3D> m_particlePos;

    // OpenGL friendly array for the posAttr. Basically flattening m_particlePos
    // Of this form: [particle0PosX, particle0PosY, particle0PosZ, particle1PosX, ...]
    std::vector<GLfloat> m_particlePosArray;

    // We will buffer m_particlePosArray (a GLfloat array) to this VBO
    QOpenGLBuffer* m_VBO_particlePos;

    // This VAO is used to draw the particles
    QOpenGLVertexArrayObject* m_VAO_particle;

    // The program we will be using. This is specific to the particles.
    QOpenGLShaderProgram *m_program_particles;

    // Particle System the window will be rendering
    ParticleSystem m_ps;

    // Increments by 1 each time the render loop finishes.
    int m_frame;

protected:
    // For GUI interation with the lights and camera system
    void keyPressEvent(QKeyEvent* ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void resizeEvent(QResizeEvent *ev);
};

#endif // PARTICLESWINDOW_H
