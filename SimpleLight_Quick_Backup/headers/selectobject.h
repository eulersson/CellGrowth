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
    virtual void createGeometry(QOpenGLContext *context) = 0;
    virtual void draw()=0;
    // Draw to a buffer with a unique colour
    virtual void drawBackBuffer()=0;
    virtual void processMouseMovement(float offsetx, float offsety, float offsetz)=0;
    virtual void setClicked(QVector3D uColour, bool state)=0;
    virtual void setHover(int id)=0;
    virtual QVector3D getPosition()=0;



    QVector3D getNewUniqueColour()
    {
        masterUniqueColour=QVector3D(masterUniqueColour.x()+1, masterUniqueColour.y(), masterUniqueColour.z());
        return masterUniqueColour;
    }

    std::vector<QVector3D> getMultipleNewUniqueColour(int amount)
    {
        std::vector<QVector3D> uColourVec;
        for(int i=0;i<amount;i++)
        {
            uColourVec.push_back(getNewUniqueColour());
        }

        return uColourVec;
    }



private:
    QVector3D masterUniqueColour=QVector3D(0.0f, 100.0f, 0.0f);
};


#endif
