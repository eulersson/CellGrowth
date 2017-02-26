// HEADER FILE ONLY


// Private
#include "selectobject.h"
#include "manipulator.h"






// DEFAULT VALUES
const QVector3D    POSITION         = QVector3D(0,0,0);
const QVector3D    LIGHT_COLOUR     = QVector3D(0.8f,0.8f,0.8f);






class PointLight : public SelectObject
{
public:
    PointLight(QVector3D _position, QOpenGLShaderProgram *_manipshaderp, QOpenGLShaderProgram *_sunshaderp) :
        manip(_position, _manipshaderp)
    {
        position=_position;
        sunshaderp=_sunshaderp;
        manipshaderp=_manipshaderp;

        // Set light representation position (single point)
        points[0]=_position.x(); points[1]=_position.y(); points[2]=_position.z();
        updateModelMatrix(0,0,0);

    }




    int compareUniqueColour(QVector3D _colour) override
    {
        return manip.compareUniqueColour(_colour);

    }
    void createGeometry(QOpenGLContext *context, QVector3D &masterUniqueColour) override
    {
        // Setup light representation VBO VAO
        setupObject(context);
        // Setup manipulator geometry
        int amountOfColours=3;
        manip.createGeometry(context, getMultipleNewUniqueColour(amountOfColours, masterUniqueColour));

    }

    void draw() override
    {
        // LIGHT
        vao->bind();
        sunshaderp->bind();
        GLint modelLoc = sunshaderp->uniformLocation("model");
        sunshaderp->setUniformValue(modelLoc, model);
        // Setup/Draw
        glEnable(GL_POINT_SPRITE);
        glPointSize(22.0f);
        glDrawArrays(GL_POINTS, 0, 1); // Previously GL_POINTS
        // Release and reset everything
        sunshaderp->release();
        vao->release();
        glDisable(GL_POINT_SPRITE);
        glPointSize(0.0f);

        // MANIPULATOR
        manipshaderp->bind();
        modelLoc = manipshaderp->uniformLocation("model");
        manipshaderp->setUniformValue(modelLoc, model);
        // Draw manipulator
        manip.draw();


    }
    void drawBackBuffer() override
    {
        manipshaderp->bind();
        GLint modelLoc = manipshaderp->uniformLocation("model");
        manipshaderp->setUniformValue(modelLoc, model);
        manip.drawBackBuffer();

    }
    void processMouseMovement(float offsetx, float offsety, float offsetz) override
    {

        updateModelMatrix(offsetx, offsety, offsetz);



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

    void getMainProgram(QOpenGLShaderProgram **retshader)
    {
        *retshader=manipshaderp;

    }


    void setupObject(QOpenGLContext *_context_)
    {

        // VAO / VBO
        vao = new QOpenGLVertexArrayObject(_context_);
        vao->create();
        vao->bind();
        vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        vbo->create();
        vbo->bind();
        vbo->setUsagePattern(QOpenGLBuffer::StaticDraw); //Previoulsy DynamicDraw
        vbo->allocate(&points[0], 3* sizeof(GLfloat)); // Allocate enogh place for all data
        // Set shader attributes
        sunshaderp->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
        sunshaderp->enableAttributeArray("posAttr");

        vao->release();




    }

    void updateModelMatrix(float offsetx, float offsety, float offsetz)
    {
        QVector3D pointPos=position;
        model.setToIdentity();
        model.translate(pointPos);
        GLfloat angle = 0.0f;
        model.rotate(angle, QVector3D(1,0.3,0.5));
        position=manip.processMouseMovement(offsetx, offsety, offsetz, position);

    }




private:
    Manipulator manip;
    QVector3D position;

    QOpenGLBuffer* vbo;
    QOpenGLVertexArrayObject *vao;
    GLfloat points[3];
    QOpenGLShaderProgram *sunshaderp;
    QOpenGLShaderProgram *manipshaderp;

    QMatrix4x4 model;



};
