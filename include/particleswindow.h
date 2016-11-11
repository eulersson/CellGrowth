#ifndef PARTICLESWINDOW_H
#define PARTICLESWINDOW_H

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "viewport.h"

/*
 * NOTE: Get familiarized with these classes:
 *   - QWindow http://doc.qt.io/qt-5/qwindow.html
 *   - QOpenGLBuffer http://doc.qt.io/qt-5/qopenglbuffer.html
 *   - QOpenGLShaderProgram (VBO) http://doc.qt.io/qt-5/qopenglshaderprogram.html
 *   - QOpenGLVertexArrayObject (VAO) http://doc.qt.io/qt-5/qopenglvertexarrayobject.html
 * */

// Inherit from Viewport and override the initialize and render methods.
class ParticlesWindow : public Viewport
{
public:
    ParticlesWindow();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

private:
    // Mocking up all the positions the particles will need to be drawn to.
    std::vector<QVector3D> m_positions_particles;
    // We will buffer the unfolded version of m_positions (a GLfloat array) to this VBO
    QOpenGLBuffer* m_VBO_particles;
    // This VAO is used to draw the particles, so Glenn could have another one to draw the light shape
    QOpenGLVertexArrayObject* m_VAO_particles;
    // The program we will be using. This is specific to the particles. Glenn will need to figure out
    // how to render the light as well as separate object. Maybe use another program?
    QOpenGLShaderProgram *m_program_particles;
    int m_frame;

protected:
    void keyPressEvent(QKeyEvent* ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void resizeEvent(QResizeEvent *ev);
};

#endif // PARTICLESWINDOW_H
