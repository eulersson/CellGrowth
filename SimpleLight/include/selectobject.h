#ifndef SELECTOBJECT_H
#define SELECTOBJECT_H



// QT
#include <QtGui/QOpenGLFunctions>
#include <QVector3D>
#include <QDebug>

// OpenGL
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class SelectObject
{

public:
    virtual int compareUniqueColour(QVector3D _colour) = 0;
    virtual void createGeometry(QOpenGLContext *context, QVector3D &masterUniqueColour) = 0;
    virtual void draw()=0;
    // Draw to a buffer with a unique colour
    virtual void drawBackBuffer()=0;
    virtual void processMouseMovement(float offsetx, float offsety, float offsetz)=0;
    virtual void setClicked(QVector3D uColour, bool state)=0;
    virtual void setHover(int id)=0;
    virtual QVector3D getPosition()=0;
    virtual void getMainProgram(QOpenGLShaderProgram **retshader)=0;



    QVector3D getNewUniqueColour(QVector3D &masterUniqueColour)
    {
        masterUniqueColour=QVector3D(masterUniqueColour.x()+1, masterUniqueColour.y(), masterUniqueColour.z());
        return masterUniqueColour;
    }

    std::vector<QVector3D> getMultipleNewUniqueColour(int amount, QVector3D &masterUniqueColour)
    {
        std::vector<QVector3D> uColourVec;
        for(int i=0;i<amount;i++)
        {
            uColourVec.push_back(getNewUniqueColour(masterUniqueColour));
        }

        return uColourVec;
    }



private:

};


#endif
