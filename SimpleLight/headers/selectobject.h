#ifndef SELECTOBJECT_H
#define SELECTOBJECT_H

#include <QVector3D>

// QT
#include <QtGui/QOpenGLFunctions>

// OpenGL
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class SelectObject
{

public:
    virtual bool compareUniqueColour(QVector3D _colour) = 0;
    virtual void createGeometry(QOpenGLVertexArrayObject *vao, QVector3D _position, int axis) = 0;
    virtual void draw()=0;
    // Draw to a buffer with a unique colour
    virtual void drawBackBuffer()=0;
    virtual void processMouseMovement(float offsetx, float offsety)=0;
    virtual void setClicked(QVector3D uColour, bool state)=0;



};


#endif
