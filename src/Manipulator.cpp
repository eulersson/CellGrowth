#include "Manipulator.h"

const int DIRECTION_X = 0;
const int DIRECTION_Y = 1;
const int DIRECTION_Z = 2;

// Manipulator move sensitivity
const GLfloat SENSITIVITY = 0.04f;


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
    delete arrow.vao;  // Free memory for VAO, we could be using smart pointers!
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
  return -1;
}

void Manipulator::createGeometry(QOpenGLContext *context, std::vector<QVector3D> uColourVec)
{
  // X ARROW
  QOpenGLVertexArrayObject *vao_x = new QOpenGLVertexArrayObject();
  createArrow(vao_x, QVector3D(1.2, 0, 0), uColourVec[0], 0);

  // Y ARROW
  QOpenGLVertexArrayObject *vao_y = new QOpenGLVertexArrayObject();
  createArrow(vao_y, QVector3D(0, 1.2, 0), uColourVec[1], 1);

  // Z ARROW
  QOpenGLVertexArrayObject *vao_z = new QOpenGLVertexArrayObject();
  createArrow(vao_z, QVector3D(0, 0, 1.2), uColourVec[2], 2);
}

void Manipulator::draw()
{
  QVector3D baseColour(0.0f, 0.0f, 1.0f);
  m_manipshaderp->bind();

  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    Arrow arrow= m_arrows[i];

    m_manipshaderp->setUniformValue("backRender", false);
    m_manipshaderp->setUniformValue("renderColour", {arrow.renderColour.x(),arrow.renderColour.y(),arrow.renderColour.z()});

    switch(arrow.axis)
    {
      case DIRECTION_X:
        baseColour= QVector3D(0.2f, 0.0f, 0.0f);
        m_manipshaderp->setUniformValue("baseColour", baseColour);
        break;

      case DIRECTION_Y:
        baseColour= QVector3D(0.0f, 0.2f, 0.0f);
        m_manipshaderp->setUniformValue("baseColour", baseColour);
        break;

      case DIRECTION_Z:
        baseColour= QVector3D(0.0f, 0.0f, 0.2f);
        m_manipshaderp->setUniformValue("baseColour", baseColour);
        break;
    }

    arrow.vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, arrow.numberOfPoints); // Previously GL_POINTS
    arrow.vao->release();
  }
  m_manipshaderp->release();
}

void Manipulator::drawBackBuffer()
{
  m_manipshaderp->bind();
  m_manipshaderp->setUniformValue("backRender", true);

  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    Arrow arrow= m_arrows[i];
    arrow.vao->bind();

    m_manipshaderp->setUniformValue("baseColour", arrow.uniqueColour);

    glDrawArrays(GL_TRIANGLES, 0, arrow.numberOfPoints); // Previously GL_POINTS

    arrow.vao->release();
  }
}

QVector3D Manipulator::processMouseMovement(
    float offsetx,
    float offsety,
    float offsetz,
    QVector3D currentPos)
{
  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    if(m_arrows[i].clicked)
    {
      switch(m_arrows[i].axis)
      {
        case DIRECTION_X:
          currentPos = QVector3D(currentPos.x() + offsetx * SENSITIVITY,
                                 currentPos.y(),
                                 currentPos.z());
          break;

        case DIRECTION_Y:
          currentPos = QVector3D(currentPos.x(),
                                 currentPos.y() + offsety * SENSITIVITY,
                                 currentPos.z());
          break;

        case DIRECTION_Z:
          currentPos = QVector3D(currentPos.x(),
                                 currentPos.y(),
                                 currentPos.z() - offsetz * SENSITIVITY);
          break;
      }
    }
  }

  return currentPos;
}

void Manipulator::setClicked(QVector3D uColourIdentity, bool state)
{
  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    if(state == false)
    {
      m_arrows[i].clicked = false;
    }
    else if(uColourIdentity == m_arrows[i].uniqueColour)
    {
      m_arrows[i].clicked = true;
    }
  }
}

void Manipulator::setHover(int axis)
{
  for(size_t i = 0; i < m_arrows.size(); i++)
  {
    if(axis == m_arrows[i].axis)
    {
      m_arrows[i].renderColour=QVector3D(.6, .6, .6);
    }
    else
    {
      m_arrows[i].renderColour=QVector3D(.2, .2, .2);
    }
  }
}

void Manipulator::getArrows(std::vector<Arrow> &_arrows)
{
  _arrows = m_arrows;
}

void Manipulator::setupVBO(
    std::vector<QVector3D> vertices,
    std::vector<QVector3D> normals,
    Arrow &arrow)
{
  // OpenGL wants a flat array of GLfloats
  std::vector<GLfloat> m_pointPosArray;
  GLint m_numberOfPoints = vertices.size();
  for (size_t i = 0; i < m_numberOfPoints; i++)
  {
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
  vbo->allocate(&m_pointPosArray[0], 6 * m_numberOfPoints * sizeof(GLfloat));
  arrow.vbo=vbo;
}

void Manipulator::setupVAO(Arrow &arrow,QOpenGLVertexArrayObject *vao)
{
  // Vertex Array Object
  vao->create();
  vao->bind();

  // Stride is 5* size of float because the vertex contains 6 values
  // First number states where to start (offset)
  // Second number states the size of the data to get (position = x,y,z = 3 values)
  // Third number is stride. Must be multiplied with sizeof float for some reason
  m_manipshaderp->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 6 * sizeof(GL_FLOAT));
  m_manipshaderp->enableAttributeArray("posAttr");

  m_manipshaderp->setAttributeBuffer("normAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GL_FLOAT));
  m_manipshaderp->enableAttributeArray("normAttr");

  vao->release();

  arrow.vao=vao;
}

QVector3D calculateNormal(QVector3D v1, QVector3D v2, QVector3D v3)
{
  QVector3D edge1 = v2 - v1;
  QVector3D edge2 = v3 - v1;
  QVector3D n = QVector3D::crossProduct(edge1, edge2);
  n.normalize();
  return  n;
}

void Manipulator::createArrow(
    QOpenGLVertexArrayObject *vao,
    QVector3D offsetPos,
    QVector3D uniqueColour,
    int axis)
{

  // Vector of normals. To be sent to the shader.
  std::vector<QVector3D> normals;
  std::vector<QVector3D> vertices;

  unsigned int sectors = 20;
  float radius = 0.2f;
  float height = 0.8f;

  offsetPos+=m_position;

  QVector3D topPoint;
  for(unsigned int i = 0; i < sectors; i++) {

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

    switch(axis){
      case DIRECTION_X:
        topPoint = QVector3D(offsetPos.x()+height,offsetPos.y(),offsetPos.z());
        x = 0 + offsetPos.x();
        y = s + offsetPos.y();
        z = c + offsetPos.z();

        x2 = 0 + offsetPos.x();
        y2 = s2 + offsetPos.y();
        z2 = c2 + offsetPos.z();
        break;

      case DIRECTION_Y:
        topPoint = QVector3D(offsetPos.x(), offsetPos.y() + height, offsetPos.z());
        x = c + offsetPos.x();
        y = 0 + offsetPos.y();
        z = s + offsetPos.z();

        x2 = c2 + offsetPos.x();
        y2 = 0 + offsetPos.y();
        z2 = s2 + offsetPos.z();
        break;

      case DIRECTION_Z:
        topPoint = QVector3D(offsetPos.x(), offsetPos.y(), offsetPos.z() + height);
        x = s + offsetPos.x();
        y = c + offsetPos.y();
        z = 0 + offsetPos.z();

        x2 = s2 + offsetPos.x();
        y2 = c2 + offsetPos.y();
        z2 = 0 + offsetPos.z();
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


  Arrow arrow = Arrow();
  arrow.axis = axis;
  arrow.numberOfPoints = vertices.size() + normals.size();
  arrow.uniqueColour=uniqueColour;

  setupVBO(vertices, normals, arrow);
  setupVAO(arrow, vao);

  m_arrows.push_back(arrow);
}
