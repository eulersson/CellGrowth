#include "manipulator.h"

const int DIRECTION_X=0;
const int DIRECTION_Y=1;
const int DIRECTION_Z=2;

//static float uniqueColour;


// Manipulator move sensitivity
const GLfloat SENSITIVITY=1.0f;


std::vector<Arrow> arrows;


Manipulator::Manipulator(QVector3D _position, QOpenGLShaderProgram *_lightProgram)
{
    uniqueColour=QVector3D(0.0f, 100.0f, 0.0f);
    m_lightProgram=_lightProgram;

}


void Manipulator::setupVBO(std::vector<QVector3D> &vertices, std::vector<QVector3D> &normals, Arrow &arrow)
{

    // OpenGL wants a flat array of GLfloats
    std::vector<GLfloat> m_pointPosArray;
    int m_numberOfPoints = vertices.size();
    int n_it=0;
    for (unsigned int i = 0; i < m_numberOfPoints; i++)
    {


//        if(i%3==0 && i!=0)
//        {
//            n_it++;

//        }


        m_pointPosArray.push_back(vertices[i].x());
        m_pointPosArray.push_back(vertices[i].y());
        m_pointPosArray.push_back(vertices[i].z());

        m_pointPosArray.push_back(normals[i].x());
        m_pointPosArray.push_back(normals[i].y());
        m_pointPosArray.push_back(normals[i].z());




    }



    QOpenGLBuffer* vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    // Buffer
    vbo->create();
    vbo->bind();
    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw); //Previoulsy DynamicDraw
    // Allocate enogh place for all data
    vbo->allocate(&m_pointPosArray[0], 6*m_numberOfPoints * sizeof(GLfloat));


    arrow.vbo=vbo;

    //arrows.push_back(arrow);



}

void Manipulator::setupVAO(Arrow &arrow,QOpenGLVertexArrayObject *vao)
{

//    QOpenGLVertexArrayObject *vao;
//    vao = new QOpenGLVertexArrayObject(this);

    // Vertex Array Object
    vao->create();
    vao->bind();

    // Stride is 5* size of float because the vertex contains 6 values
    // First number states where to start (offset)
    // Second number states the size of the data to get (position = x,y,z = 3 values)
    // Third number is stride. Must be multiplied with sizeof float for some reason

    m_lightProgram->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 6*sizeof(GL_FLOAT));
    m_lightProgram->enableAttributeArray("posAttr");

    m_lightProgram->setAttributeBuffer("normAttr", GL_FLOAT, 3* sizeof(GLfloat), 3, 6*sizeof(GL_FLOAT));
    m_lightProgram->enableAttributeArray("normAttr");



    vao->release();

    arrow.vao=vao;
}

QVector3D calculateNormal(QVector3D v1, QVector3D v2, QVector3D v3)
{
    QVector3D edge1 = v2-v1;
    QVector3D edge2 = v3-v1;
    QVector3D n = QVector3D::crossProduct(edge1, edge2);
    n.normalize();
    return  n;


}


void Manipulator::createGeometry(QOpenGLVertexArrayObject *vao, QVector3D _position, int axis)
{
    uniqueColour=QVector3D(uniqueColour.x()+1, uniqueColour.y(), uniqueColour.z());

    // Vector of normals. To be sent to the shader.
    std::vector<QVector3D> normals;
    std::vector<QVector3D> vertices;

//    unsigned int rings = 5;
    unsigned int sectors = 80;

    float radius = 0.2f;

    float height = 1.0f;

//    float const R = 1./(float)(rings-1);
//    float const S = 1./(float)(sectors-1);




    QVector3D topPoint;


    //QVector3D topPoint (_position.x(),_position.y()+height,_position.z());


    for(unsigned int i = 0; i < sectors; i++) {

        float angle = (2*M_PI) * ((float)i / (float)sectors);
        float s = radius*sin(angle);
        float c = radius*cos(angle);

        float angleNext = (2*M_PI) * ((float)(i+1) / (float)sectors);
        float s2 = radius*sin(angleNext);
        float c2 = radius*cos(angleNext);

        //verts[index++].set(c, s, 0.f);

        float x;
        float y;
        float z;

        float x2;
        float y2;
        float z2;

        switch(axis){
        case DIRECTION_X:
            topPoint = QVector3D(_position.x()+height,_position.y(),_position.z());
            x = 0+_position.x();
            y = s+_position.y();
            z = c+_position.z();

            x2 = 0+_position.x();
            y2 = s2+_position.y();
            z2 = c2+_position.z();
            break;

        case DIRECTION_Y:
            topPoint = QVector3D(_position.x(),_position.y()+height,_position.z());
            x = c+_position.x();
            y = 0+_position.y();
            z = s+_position.z();

            x2 = c2+_position.x();
            y2 = 0+_position.y();
            z2 = s2+_position.z();
            break;

        case DIRECTION_Z:
            topPoint = QVector3D(_position.x(),_position.y(),_position.z()+height);
            x = s+_position.x();
            y = c+_position.y();
            z = 0+_position.z();

            x2 = s2+_position.x();
            y2 = c2+_position.y();
            z2 = 0+_position.z();
            break;


        }

        QVector3D normal = calculateNormal( QVector3D(x,y,z),topPoint, QVector3D(x2,y2,z2));

//        float const x = s+_position.x();
//        float const y = 0+_position.y();
//        float const z = c+_position.z();

        vertices.push_back(QVector3D(x,y,z));
        normals.push_back(normal);







//        float const x2 = s2+_position.x();
//        float const y2 = 0+_position.y();
//        float const z2 = c2+_position.z();

        vertices.push_back(QVector3D(x2,y2,z2));
        normals.push_back(normal);

        vertices.push_back(topPoint);
        normals.push_back(normal);




//        float const y = sin(  M_PI * r * R );
//        float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
//        float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

        //vertices.push_back(QVector3D(x,y,z) * radius);
    }


    Arrow arrow = Arrow();
    arrow.axis=axis;
    arrow.numberOfPoints=vertices.size()+normals.size();
    arrow.uniqueColour=uniqueColour;

    setupVBO(vertices, normals, arrow);
    setupVAO(arrow, vao);
    //setupNormals(normals, &arrow);

    arrows.push_back(arrow);



}

void Manipulator::setClicked(QVector3D uColour, bool state)
{



    for(int i=0;i<arrows.size();i++)
    {
        if(state==false)
        {
            arrows[i].clicked=false;
        }
        else if(uColour == arrows[i].uniqueColour)
        {
            arrows[i].clicked=true;

        }

    }
}

void Manipulator::processMouseMovement(float offsetx, float offsety)
{
    bool clicked=false;
    for(int i=0;i<arrows.size();i++)
    {

        if(arrows[i].clicked)
        {

            //clicked=true;
            switch(arrows[i].axis)
            {
                case DIRECTION_X:

                    currentPos = QVector3D(currentPos.x()+offsetx*SENSITIVITY,
                                           currentPos.y(),
                                           currentPos.z());
                    break;

                case DIRECTION_Y:
                    currentPos = QVector3D(currentPos.x(),
                                           currentPos.y()+offsety*SENSITIVITY,
                                           currentPos.z());
                    break;

                case DIRECTION_Z:
                    currentPos = QVector3D(currentPos.x(),
                                           currentPos.y(),
                                           currentPos.z()-offsetx*SENSITIVITY);
                    break;
            }
        }



    }
    if (clicked)
    {
        currentPos = QVector3D(currentPos.x()+offsetx*SENSITIVITY, currentPos.y()+offsety*SENSITIVITY, currentPos.z());
    }


}

QVector3D Manipulator::getPosition()
{
    return currentPos;
}

void Manipulator::draw()
{
    for(int i=0;i<arrows.size();i++)
    {
        Arrow arrow= arrows[i];
        arrow.vao->bind();
        m_lightProgram->setUniformValue("backRender", false);


        glDrawArrays(GL_TRIANGLES, 0, arrow.numberOfPoints); // Previously GL_POINTS


        arrow.vao->release();

    }
}

void Manipulator::drawBackBuffer()
{
    m_lightProgram->setUniformValue("backRender", true);
    for(int i=0;i<arrows.size();i++)
    {


        Arrow arrow= arrows[i];
        arrow.vao->bind();

        m_lightProgram->setUniformValue("baseColour", arrow.uniqueColour);


        glDrawArrays(GL_TRIANGLES, 0, arrow.numberOfPoints); // Previously GL_POINTS


        arrow.vao->release();

    }
}


void Manipulator::getArrows(std::vector<Arrow> &_arrows)
{
    _arrows=arrows;

}



bool Manipulator::compareUniqueColour(QVector3D _colour)
{

    //if(pixel[0]==(int)c.x()*255 && pixel[1]==(int)c.y()*255 && pixel[2]==(int)c.z()*255)

    for(int i=0;i<arrows.size();i++)
    {
        if(_colour == arrows[i].uniqueColour)
        {
            //qDebug() << QString::number(pixel[0]);
            return true;

        }

    }
    return false;



}
