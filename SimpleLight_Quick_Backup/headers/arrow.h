#include <QVector3D>

// OpenGL
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class Arrow2 {


public:
    bool clicked;
    int axis;
    unsigned int numberOfPoints;
    QVector3D uniqueColour;
    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;

};
