// HEADER FILE ONLY


// Private
#include "selectobject.h"
#include "manipulator.h"






// DEFAULT VALUES
const QVector3D POSITION=QVector3D(0,0,0);
const QVector3D LIGHT_COLOUR=QVector3D(0.8f,0.8f,0.8f);


class PointLight : public SelectObject
{
public:
    PointLight(QVector3D _position, QOpenGLShaderProgram *_manipshaderp, QOpenGLShaderProgram *_sunshaderp) :
        manip(_position, _manipshaderp)
    {
        position=_position;
        sunshaderp=_sunshaderp;

        // Set light representation position (single point)
        points[0]=_position.x(); points[1]=_position.y()+2; points[2]=_position.z();

    }

    // Convenience function for getting a QMatrix from glm mat4
//    QMatrix4x4 getQMAtrix(glm::mat4 mat)
//    {
//        float *fm=(float*)glm::value_ptr(mat);
//        return QMatrix4x4(fm[0], fm[4], fm[8], fm[12],
//                          fm[1], fm[5], fm[9], fm[13],
//                          fm[2], fm[6], fm[10], fm[14],
//                          fm[3], fm[7], fm[11], fm[15] );

//    }


    int compareUniqueColour(QVector3D _colour) override
    {
        return manip.compareUniqueColour(_colour);

    }
    void createGeometry(QOpenGLContext *context) override
    {
        // Setup light representation VBO VAO
        setupBuffer(context);

        // Setup manipulator representation
        int amountOfColours=3;
        manip.createGeometry(context, getMultipleNewUniqueColour(amountOfColours));

    }
    void draw() override
    {

        // Draw light representation
        vao->bind();
        sunshaderp->bind();
        glEnable(GL_POINT_SPRITE);
        glPointSize(34.0f);

        glDrawArrays(GL_POINTS, 0, 1); // Previously GL_POINTS
        sunshaderp->release();
        vao->release();


        glDisable(GL_POINT_SPRITE);
        glPointSize(0.0f);


        // Draw manipulator
        manip.draw();


    }
    void drawBackBuffer() override
    {
        manip.drawBackBuffer();

    }
    void processMouseMovement(float offsetx, float offsety, float offsetz) override
    {
        position=manip.processMouseMovement(offsetx, offsety, offsetz, position);

    }
    void setClicked(QVector3D uColour, bool state) override
    {
        manip.setClicked(uColour, state);

    }
    QVector3D getPosition() override { return position; }

    void setHover(int id) override
    {
        manip.setHover(id);

    }


    Manipulator getManipulator() { return manip; }


    void setupBuffer(QOpenGLContext *context)
    {

        vao = new QOpenGLVertexArrayObject(context);
        vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        // Vertex Array Object
        vao->create();
        vao->bind();

        vbo->create();
        vbo->bind();


        vbo->setUsagePattern(QOpenGLBuffer::StaticDraw); //Previoulsy DynamicDraw
        // Allocate enogh place for all data
        vbo->allocate(&points[0], 3* sizeof(GLfloat));
        sunshaderp->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
        sunshaderp->enableAttributeArray("posAttr");



        vao->release();




    }




private:
    Manipulator manip;
    QVector3D position;

    QOpenGLBuffer* vbo;
    QOpenGLVertexArrayObject *vao;
    GLfloat points[3];
    QOpenGLShaderProgram *sunshaderp;



};
