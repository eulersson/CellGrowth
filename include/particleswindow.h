#ifndef PARTICLESWINDOW_H
#define PARTICLESWINDOW_H

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include "viewport.h"

class ParticlesWindow : public Viewport
{
public:
    ParticlesWindow();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

private:
    unsigned int m_number_of_particles;
    std::vector<QVector3D> m_positions;

    GLfloat *m_sphere_vertices;
    GLuint *m_sphere_indices;
    GLuint m_number_of_indices;

    QOpenGLBuffer* m_VBO;
    QOpenGLBuffer* m_EBO;
    QOpenGLVertexArrayObject* m_VAO;
    QOpenGLShaderProgram *m_program;
    int m_frame;

protected:
    void keyPressEvent(QKeyEvent* ev);
};

#endif // PARTICLESWINDOW_H
