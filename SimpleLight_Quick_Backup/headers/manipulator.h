#ifndef MANIPULATOR_H
#define MANIPULATOR_H

// QT
#include <QtGui/QOpenGLFunctions>
#include <QVector3D>

// OpenGL
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

struct Arrow {
public:
    bool clicked;
    int axis;
    unsigned int numberOfPoints;
    QVector3D uniqueColour;
    QVector3D renderColour;
    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;

};

class Manipulator
{


public:
    explicit Manipulator (QVector3D _position, QOpenGLShaderProgram *_lightProgram);

    int compareUniqueColour(QVector3D _colour);
    void createGeometry(QOpenGLContext *context, std::vector<QVector3D> uColourVec);
    void draw();
    void drawBackBuffer();
    QVector3D processMouseMovement(float offsetx, float offsety, float offsetz, QVector3D currentPos);
    void setClicked(QVector3D uColourIdentity, bool state);
    void setHover(int axis);


    void getArrows(std::vector<Arrow> &_arrows);


private:
    void setupVBO(std::vector<QVector3D> &vertices, std::vector<QVector3D> &normals, Arrow &arrow);
    void setupVAO(Arrow &arrow, QOpenGLVertexArrayObject *vao);
    void createArrow(QOpenGLVertexArrayObject *vao, QVector3D _position, QVector3D uniqueColour, int axis);



    QOpenGLShaderProgram *m_lightProgram;

    std::vector<Arrow> arrows;


};


#endif
