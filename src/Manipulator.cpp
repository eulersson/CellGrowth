// Standard
#include <cmath>

// Project
#include "Manipulator.h"

// Defines
#define M_PI 3.14159265358979323846

Manipulator::Manipulator(
      QVector3D _position,
      QOpenGLShaderProgram *_lightProgram)
  : m_position(_position)
  , m_manipshaderp(_lightProgram)
{
}

Manipulator::~Manipulator()
{
  for (auto& arrow: m_arrows) {
    delete arrow.vao;
  }
}

int Manipulator::compareUniqueColour(QVector3D _colour)
{
  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    if(_colour == m_arrows[i].uniqueColour)
    {
       return m_arrows[i].axis;
    }
  }

  for(size_t i = 0; i < m_circles.size(); i++)
  {
    if(_colour == m_circles[i].uniqueColour)
    {
       return m_circles[i].axis;
    }
  }
  return -1;
}

void Manipulator::createGeometry(std::vector<QVector3D> _uColourVec,
                                 GLfloat _coneangle)
{
  // X ARROW
  QOpenGLVertexArrayObject *vao_x = new QOpenGLVertexArrayObject();
  createArrow(vao_x, QVector3D(1.2, 0, 0), _uColourVec[0], DIRECTION_X);

  // Y ARROW
  QOpenGLVertexArrayObject *vao_y = new QOpenGLVertexArrayObject();
  createArrow(vao_y, QVector3D(0, 1.2, 0), _uColourVec[1], DIRECTION_Y);

  // Z ARROW
  QOpenGLVertexArrayObject *vao_z = new QOpenGLVertexArrayObject();
  createArrow(vao_z, QVector3D(0, 0, 1.2), _uColourVec[2], DIRECTION_Z);

  // Check to see if the light has a restricted angle and
  // therefore needs to rotate.
  if(_coneangle<0) {return;}
  // ROT AROUND Y
  QOpenGLVertexArrayObject *vao_rot_y = new QOpenGLVertexArrayObject();
  createRotCircle(vao_rot_y, _uColourVec[3], ROTATION_Y);

  // ROT AROUND Z
  QOpenGLVertexArrayObject *vao_rot_z = new QOpenGLVertexArrayObject();
  createRotCircle(vao_rot_z, _uColourVec[4], ROTATION_Z);

  // LIGHT CONE
  createLightConeCircle(_coneangle);

}

void Manipulator::draw()
{
  QVector3D baseColour(1.0f, 1.0f, 1.0f);
  m_manipshaderp->bind();
  m_manipshaderp->setUniformValue("backRender", false);
  m_manipshaderp->setUniformValue("flatRender", true);


  // MANIPULATOR MOVE ARROWS
  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    Geo arrow= m_arrows[i];
    if(arrow.numberOfPoints<1) {continue;}
    m_manipshaderp->setUniformValue("highlightColour", {arrow.renderColour.x(),
                                                     arrow.renderColour.y(),
                                                     arrow.renderColour.z()});
    switch(arrow.axis)
    {
      case DIRECTION_X:
        baseColour= QVector3D(0.4f, 0.0f, 0.0f);
        break;

      case DIRECTION_Y:
        baseColour= QVector3D(0.0f, 0.4f, 0.0f);
        break;

      case DIRECTION_Z:
        baseColour= QVector3D(0.0f, 0.0f, 0.4f);
        break;
    }
    m_manipshaderp->setUniformValue("baseColour", baseColour);

    arrow.vao->bind();
    arrow.vbo->bind();
    glDrawArrays(GL_TRIANGLES, 0, arrow.numberOfPoints);
    arrow.vao->release();
  }


  // MANIPULATOR ROTATION CIRCLES
  for(size_t i = 0; i < m_circles.size(); i++)
  {
      Geo circle= m_circles[i];
      if(circle.numberOfPoints<1) {continue;}
      m_manipshaderp->setUniformValue("highlightColour", {circle.renderColour.x(),
                                                       circle.renderColour.y(),
                                                       circle.renderColour.z()});
      switch(circle.axis)
      {
        case ROTATION_Y:
          baseColour= QVector3D(0.0f, 0.4f, 0.0f);
          break;

        case ROTATION_Z:
          baseColour= QVector3D(0.0f, 0.0f, 0.4f);
          break;
      }
      m_manipshaderp->setUniformValue("baseColour", baseColour);

      circle.vao->bind();
      glDrawArrays(GL_TRIANGLES, 0, circle.numberOfPoints);
      circle.vao->release();
  }

  // MANIPULATOR EFFECT CONE
  if(m_coneCircle.numberOfPoints>0)
  {

    m_manipshaderp->setUniformValue("flatRender", true);
    m_manipshaderp->setUniformValue("backRender", false);

    baseColour= QVector3D(1.0f, 1.0f, 1.0f);
    m_manipshaderp->setUniformValue("baseColour", baseColour);

    m_coneCircle.vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, m_coneCircle.numberOfPoints);
    m_coneCircle.vao->release();
    m_manipshaderp->setUniformValue("flatRender", false);
  }




  m_manipshaderp->release();
}

void Manipulator::drawBackBuffer()
{
  m_manipshaderp->bind();
  m_manipshaderp->setUniformValue("backRender", true);

  // Draw arrows
  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    Geo arrow= m_arrows[i];
    arrow.vao->bind();
    m_manipshaderp->setUniformValue("baseColour", arrow.uniqueColour);
    glDrawArrays(GL_TRIANGLES, 0, arrow.numberOfPoints);
    arrow.vao->release();
  }

  // Draw rotation circles
  for(size_t i = 0; i < m_circles.size(); i++)
  {
      Geo circle= m_circles[i];
      m_manipshaderp->setUniformValue("baseColour", circle.uniqueColour);
      circle.vao->bind();
      glDrawArrays(GL_TRIANGLES, 0, circle.numberOfPoints);
      circle.vao->release();
  }
}

int Manipulator::getClickedAxis()
{
  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    if(m_arrows[i].clicked)
    {
      return m_arrows[i].axis;
    }
  }
  for(size_t i = 0; i < m_circles.size(); i++)
  {
    if(m_circles[i].clicked)
    {
      return m_circles[i].axis;
    }
  }
  return -1;
}

void Manipulator::setClicked(QVector3D uColourIdentity, bool _state)
{
  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    if(_state == false)
    {
      m_arrows[i].clicked = false;
    }
    else if(uColourIdentity == m_arrows[i].uniqueColour)
    {
      m_arrows[i].clicked = true;
    }
  }

  for(size_t i = 0; i < m_circles.size(); i++)
  {
    if(_state == false)
    {
      m_circles[i].clicked = false;
    }
    else if(uColourIdentity == m_circles[i].uniqueColour)
    {
      m_circles[i].clicked = true;
    }
  }
}

void Manipulator::setHover(int axis)
{
  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    if(axis == m_arrows[i].axis)
      m_arrows[i].renderColour=QVector3D(.6, .6, .6);
    else
      m_arrows[i].renderColour=QVector3D(0, 0, 0);
  }

  for(size_t i = 0; i < m_circles.size(); i++)
  {
    if(axis == m_circles[i].axis)
      m_circles[i].renderColour=QVector3D(.6, .6, .6);
    else
      m_circles[i].renderColour=QVector3D(0, 0, 0);
  }
}

void Manipulator::getArrows(std::vector<Geo> &_arrows)
{
  _arrows = m_arrows;
}


void Manipulator::setupRotCircleVAO(Geo &_circle, QOpenGLVertexArrayObject *_vao)
{
  // Vertex Array Object
  _vao->create();
  _vao->bind();
  m_manipshaderp->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
  m_manipshaderp->enableAttributeArray("posAttr");
  _vao->release();
  _circle.vao=_vao;

}

void Manipulator::setupRotCircleVBO(
    std::vector<QVector3D> _vertices,
    Geo &_circle)
{
  // OpenGL wants a flat array of GLfloats
  std::vector<GLfloat> m_pointPosArray;
  GLint numberOfPoints = _vertices.size();
  for (GLint i = 0; i < numberOfPoints; i++)
  {
    m_pointPosArray.push_back(_vertices[i].x());
    m_pointPosArray.push_back(_vertices[i].y());
    m_pointPosArray.push_back(_vertices[i].z());
  }

  QOpenGLBuffer* vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

  // Buffer
  vbo->create();
  vbo->bind();
  vbo->setUsagePattern(QOpenGLBuffer::StaticDraw); //Previoulsy DynamicDraw
  // Allocate enogh place for all data
  vbo->allocate(&m_pointPosArray[0], 3 * numberOfPoints * sizeof(GLfloat));
  _circle.vbo=vbo;

}

void Manipulator::createSingleCircle(
    uint _segments,
    float _thickness,
    float _r,
    int _axis,
    int _numb,
    QVector3D _op,
    std::vector<QVector3D> &_vertices)
{

  float amt=(2*M_PI)/_segments;
  QVector3D lastPoint = QVector3D(_op.x(),_op.y(),_op.z());

  for(uint i=0; i<=_segments;i++)
  {

    float x=0;
    float y=0;
    float z=0;

    float lx=1;
    float ly=1;
    float lz=1;
    float angle = amt*i;


    float off_x=0;
    float off_y=0;
    float off_z=0;
    switch(_axis)
    {

      case ROTATION_X:
      {
        z=_r*cos(angle);
        y=_r*sin(angle);
        lz=_thickness;
        ly=_thickness;

        if(_numb==1)
        {
            x-=(_thickness-1)/2;
            off_x=(_thickness-1);
            lz=1;
            ly=1;
        }
        break;
      }

      // Around Y axis
      case ROTATION_Y:
      {
        x=_r*cos(angle);
        z=_r*sin(angle);
        lx=_thickness;
        lz=_thickness;

        if(_numb==1)
        {
            y-=(_thickness-1)/2;
            off_y=(_thickness-1);
            lx=1;
            lz=1;
        }


        break;
      }
      // Around Z axis
      case ROTATION_Z:
      {
        x=_r*cos(angle);
        y=_r*sin(angle);
        lx=_thickness;
        ly=_thickness;

        if(_numb==1)
        {
            z-=(_thickness-1)/2;
            off_z=(_thickness-1);
            lx=1;
            ly=1;
        }
        break;
      }
    }
    if(lastPoint==QVector3D(_op.x(),_op.y(),_op.z()))
    {
      lastPoint=QVector3D(x,y,z);
      continue;
    }

    _vertices.push_back(QVector3D(x,y,z)                         +QVector3D(_op.x(),_op.y(),_op.z()));
    _vertices.push_back(lastPoint                                +QVector3D(_op.x(),_op.y(),_op.z()));
    _vertices.push_back(QVector3D(lx, ly, lz)*lastPoint          +QVector3D(_op.x()+off_x,_op.y()+off_y,_op.z()+off_z));

    _vertices.push_back(QVector3D(x,y,z)                         +QVector3D(_op.x(),_op.y(),_op.z()));
    _vertices.push_back(QVector3D(lx, ly, lz)*lastPoint          +QVector3D(_op.x()+off_x,_op.y()+off_y,_op.z()+off_z));
    _vertices.push_back(QVector3D(x,y,z)*QVector3D(lx, ly, lz)   +QVector3D(_op.x()+off_x,_op.y()+off_y,_op.z()+off_z));

    lastPoint=QVector3D(x,y,z);


  }
}


void Manipulator::createRotCircle(QOpenGLVertexArrayObject *_vao,
                     QVector3D _uniqueColour,
                     int _axis)
{

  std::vector<QVector3D> vertices;
  int segments = 50;
  float thickness=0.12f  +1.0f;
  float r=0.8f;
  QVector3D op(0,0,0);

  // Make two circles to cower all viewing angles,
  // and avoid lines from dissapearing when viewed straight on.
  createSingleCircle(segments, thickness, r, _axis, 0, op, vertices);
  createSingleCircle(segments, thickness, r, _axis, 1, op, vertices);




  Geo circle = Geo();
  circle.axis = _axis;
  circle.numberOfPoints = vertices.size();
  circle.uniqueColour=_uniqueColour;

  setupRotCircleVBO(vertices, circle);
  setupRotCircleVAO(circle, _vao);

  m_circles.push_back(circle);


}




void Manipulator::setupVBO(
    std::vector<QVector3D> _vertices,
    std::vector<QVector3D> _normals,
    Geo &_arrow)
{
  // OpenGL wants a flat array of GLfloats
  std::vector<GLfloat> m_pointPosArray;
  GLint numberOfPoints = _vertices.size();
  for (GLint i = 0; i < numberOfPoints; i++)
  {
    m_pointPosArray.push_back(_vertices[i].x());
    m_pointPosArray.push_back(_vertices[i].y());
    m_pointPosArray.push_back(_vertices[i].z());

    m_pointPosArray.push_back(_normals[i].x());
    m_pointPosArray.push_back(_normals[i].y());
    m_pointPosArray.push_back(_normals[i].z());
  }

  QOpenGLBuffer* vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

  // Buffer
  vbo->create();
  vbo->bind();
  vbo->setUsagePattern(QOpenGLBuffer::StaticDraw); //Previoulsy DynamicDraw
  // Allocate enogh place for all data
  vbo->allocate(&m_pointPosArray[0], 6 * numberOfPoints * sizeof(GLfloat));
  _arrow.vbo=vbo;
}

void Manipulator::setupVAO(Geo &_arrow, QOpenGLVertexArrayObject *_vao)
{
  // Vertex Array Object
  _vao->create();
  _vao->bind();

  // Stride is 5* size of float because the vertex contains 6 values
  // First number states where to start (offset)
  // Second number states the size of the data to get (position = x,y,z = 3 values)
  // Third number is stride. Must be multiplied with sizeof float.
  m_manipshaderp->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 6 * sizeof(GL_FLOAT));
  m_manipshaderp->enableAttributeArray("posAttr");

  m_manipshaderp->setAttributeBuffer("normAttr", GL_FLOAT, 3 *
                                     sizeof(GLfloat), 3, 6 * sizeof(GL_FLOAT));
  m_manipshaderp->enableAttributeArray("normAttr");

  _vao->release();
  _arrow.vao=_vao;
}

QVector3D calculateNormal(QVector3D _v1, QVector3D _v2, QVector3D _v3)
{
  QVector3D edge1 = _v2 - _v1;
  QVector3D edge2 = _v3 - _v1;
  QVector3D n = QVector3D::crossProduct(edge1, edge2);
  n.normalize();
  return  n;
}


void Manipulator::createArrow(QOpenGLVertexArrayObject *_vao,
    QVector3D _offsetPos,
    QVector3D _uniqueColour,
    int _axis)
{

  // Vector of normals. To be sent to the shader.
  std::vector<QVector3D> normals;
  std::vector<QVector3D> vertices;

  unsigned int sectors = 20;
  float radius = 0.2f;
  float height = 0.8f;

  QVector3D topPoint;

  for(size_t i = 0; i < sectors; i++) {

    float angle = (2 * M_PI) * ((float)i / (float)sectors);
    float s = radius * sin(angle);
    float c = radius * cos(angle);

    float angleNext = (2 * M_PI) * ((float)(i + 1) / (float)sectors);
    float s2 = radius * sin(angleNext);
    float c2 = radius * cos(angleNext);

    float x;
    float y;
    float z;

    float x2;
    float y2;
    float z2;

    switch(_axis){
      case DIRECTION_X:
        topPoint = QVector3D(_offsetPos.x()+height,_offsetPos.y(),_offsetPos.z());
        x = 0 + _offsetPos.x();
        y = s + _offsetPos.y();
        z = c + _offsetPos.z();

        x2 = 0 + _offsetPos.x();
        y2 = s2 + _offsetPos.y();
        z2 = c2 + _offsetPos.z();
        break;

      case DIRECTION_Y:
        topPoint = QVector3D(_offsetPos.x(), _offsetPos.y() + height, _offsetPos.z());
        x = c + _offsetPos.x();
        y = 0 + _offsetPos.y();
        z = s + _offsetPos.z();

        x2 = c2 + _offsetPos.x();
        y2 = 0 + _offsetPos.y();
        z2 = s2 + _offsetPos.z();
        break;

      case DIRECTION_Z:
        topPoint = QVector3D(_offsetPos.x(), _offsetPos.y(), _offsetPos.z() + height);
        x = s + _offsetPos.x();
        y = c + _offsetPos.y();
        z = 0 + _offsetPos.z();

        x2 = s2 + _offsetPos.x();
        y2 = c2 + _offsetPos.y();
        z2 = 0 + _offsetPos.z();
        break;
    }

    QVector3D normal = calculateNormal(QVector3D(x, y, z),topPoint, QVector3D(x2, y2, z2));

    vertices.push_back(QVector3D(x, y, z));
    normals.push_back(normal);

    vertices.push_back(QVector3D(x2, y2, z2));
    normals.push_back(normal);

    vertices.push_back(topPoint);
    normals.push_back(normal);
  }

  Geo arrow = Geo();
  arrow.axis = _axis;
  arrow.numberOfPoints = vertices.size() + normals.size();
  arrow.uniqueColour=_uniqueColour;

  setupVBO(vertices, normals, arrow);
  setupVAO(arrow, _vao);

  m_arrows.push_back(arrow);
}





void Manipulator::createLightConeCircle(GLfloat _coneangle)
{
  float coneangle=90-_coneangle/2.0f;

  // Create geometry
  std::vector<QVector3D> vertices;
  unsigned int segments = 20;
  float r = _coneangle/90.0f;
  float thickness=0.04f; thickness++;
  float length = 3.0f*sin(qDegreesToRadians(coneangle));
  QVector3D op(length,0,0);

  // Make two circles to cower all viewing angles,
  // and avoid lines from dissapearing when viewed straight on.
  createSingleCircle(segments, thickness, r, ROTATION_X, 0, op, vertices);
  createSingleCircle(segments, thickness, r, ROTATION_X, 1, op, vertices);

  // Make geometry OpenGL readable
  std::vector<GLfloat> pointPosArray;
  unsigned int m_numberOfPoints = vertices.size();
  for (size_t i = 0; i < m_numberOfPoints; i++)
  {
    pointPosArray.push_back(vertices[i].x());
    pointPosArray.push_back(vertices[i].y());
    pointPosArray.push_back(vertices[i].z());
  }

  // VAO / VBO
  m_coneCircle.vao = new QOpenGLVertexArrayObject();
  m_coneCircle.vao->create();
  m_coneCircle.vao->bind();
  m_coneCircle.vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  m_coneCircle.vbo->create();
  m_coneCircle.vbo->bind();
  m_coneCircle.vbo->setUsagePattern(QOpenGLBuffer::StaticDraw); //Previoulsy DynamicDraw
  m_coneCircle.vbo->allocate(&pointPosArray[0], 3*m_numberOfPoints* sizeof(GLfloat));
  // Set shader attributes
  m_manipshaderp->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
  m_manipshaderp->enableAttributeArray("posAttr");
  m_coneCircle.vao->release();

  m_coneCircle.numberOfPoints = m_numberOfPoints;


}
