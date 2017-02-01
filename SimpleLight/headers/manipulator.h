#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "selectobject.h"
#include "arrow.h"

class Manipulator : public SelectObject
{


public:
    explicit Manipulator (QVector3D _uniqueColour, QOpenGLShaderProgram *_lightProgram);

    bool compareUniqueColour(QVector3D _colour) override;
    void createGeometry(QOpenGLVertexArrayObject *vao, QVector3D _position, int axis) override;
    void draw() override;
    void drawBackBuffer() override;
    void processMouseMovement(float offsetx, float offsety) override;
    void setClicked(QVector3D uColour, bool state) override;

    QVector3D getPosition();
    void getArrows(std::vector<Arrow> &_arrows);


private:
    void setupVBO(std::vector<QVector3D> &vertices, std::vector<QVector3D> &normals, Arrow &arrow);
    void setupVAO(Arrow &arrow, QOpenGLVertexArrayObject *vao);



    QVector3D currentPos;


    QOpenGLShaderProgram *m_lightProgram;
    QVector3D uniqueColour;


};


#endif
